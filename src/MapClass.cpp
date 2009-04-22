#include "Application.h"
#include "GameMan.h"
#include "Log.h"
#include "MapClass.h"

using namespace std;

MapClass::MapClass(UPoint size)
{
    int lookDist[11];
    lookDist[0] = 10; 
    lookDist[1] = 10;
    lookDist[2] = 9;
    lookDist[3] = 9;
    lookDist[4] = 9;
    lookDist[5] = 8;
    lookDist[6] = 8;
    lookDist[7] = 7;
    lookDist[8] = 6;
    lookDist[9] = 4;
    lookDist[10] = 1;

    TerrainClass* cell;

    w = size.x;
    h = size.y;

    int i, j, k;

    for (i = 0; i < w*h; i++)
    {
	cell = new TerrainClass();
	cell->y = i / w;
	cell->x = i - cell->y * w;
	m_cells.push_back(cell);
    }

    depthCheckCount = new short[std::max(w, h)];

    memset(depthCheckCount, 0, sizeof(short)*std::max(w, h));
    depthCheckMax = new short**[w];

    for (i = 0; i < w; i++)
    {
	depthCheckMax[i] = new short*[h];

	for (j = 0; j < h; j++)
	{
	    depthCheckMax[i][j] = new short[std::max(w, h)+1];
	}
    }

    //arrays used by astar algorithm
    //now fill in the maximum number of cells in a square shape
    //u could look at without success around a destination i,j
    //with a specific k distance before knowing that it is
    //imposible to get to the destination.  Each time the astar
    //algorithm pushes a node with a max diff of k, 
    //depthcheckcount(k) is incremented, if it reaches the
    //value in depthcheckmax(i,j,k), we know have done a full
    //square around target, and thus it is impossible to reach
    //the target, so we should try and get closer if possible,
    //but otherwise stop
    int x, y, maxSquareDepth,
	end;

    for (i=0; i<w; i++)	//x position
	for (j=0; j<h; j++)	//y position
	{
	    //must account for edges of map
	    depthCheckMax[i][j][0] = 0;
	    maxSquareDepth = std::max(std::max((w-1) - i, i), std::max((h-1) - j, j));

	    for (k=1; k<=maxSquareDepth; k++)	//manually count number of cells
	    {									//bad
		depthCheckMax[i][j][k] = 0;
		y = j - k;
		if ((y >= 0) && (y < h))
		{
		    x = i - k;
		    if (x < 0)
			x = 0;

		    if ((i + k) < w)
			end = i + k;
		    else
			end = w - 1;

		    depthCheckMax[i][j][k] += end - x + 1;
		}
		x = i + k;
		if ((x >= 0) && (x < w))
		{
		    if (++y < 0)
			y = 0;

		    if ((j + k) < h)
			end = j + k;
		    else
			end = h - 1;

		    depthCheckMax[i][j][k] += end - y + 1;
		}
		y = j + k;
		if ((y >= 0) && (y < h))
		{
		    if (--x >= w)
			x = w - 1;

		    if ((i - k) >= 0)
			end = i - k;
		    else
			end = 0;

		    depthCheckMax[i][j][k] += x - end + 1;
		}

		x = i - k;

		if ((x >= 0) && (x < w))
		{
		    if (--y >= h)
			y = h - 1;

		    if ((j - k + 1) >= 0)
			end = j - k + 1;
		    else
			end = 0;

		    depthCheckMax[i][j][k] += y - end + 1;
		}
		//LOG_INFO("MapClass", "depthCheckMax[%d][%d][%d] is %d.", i, j, k, depthCheckMax[i][j][k]);
	    }
	}
}

MapClass::~MapClass()
{

}

/*inline*/
bool MapClass::cellExists(SPoint pos) 
{
    return ((pos.x >= 0) && (pos.x < w) && (pos.y >= 0) && (pos.y < h));
}

BLOCKEDTYPE MapClass::cellBlocked(UPoint pos)
{
    BLOCKEDTYPE blocked = COMBLOCKED;
    if (cellExists(pos))
    {
	if (getCell(pos)->getType() == Terrain_Mountain)
	{
	    blocked = MOUNTAIN;
	}
	else if (getCell(pos)->hasAnObject())
	{
	    if (getCell(pos)->getObject()->hasAttribute(OBJECT_INFANTRY))
		blocked = INFANTRY;
	    else
		blocked = COMBLOCKED;
	}
	else
	    blocked = NOTBLOCKED;
    }

    return blocked;
}

void MapClass::createSandRegions()
{
#if 0
    int	angle,
	i, j,
	region = 0;

    UPoint pos;
    ListIterator queue;
    TerrainClass* terr;

    for (i = 0; i < w; i++)
    {
	for (j = 0; j < h; j++)
	{
	    getCell(i,j)->setSandRegion(NONE);
	    getCell(i,j)->m_visited = false;
	}
    }

    for (i = 0; i < w; i++)
    {
	for (j = 0; j < h; j++)
	{
	    if (!getCell(i,j)->isRock() && !getCell(i,j)->m_visited)
	    {
		queue.insertFirst(getCell(i,j));
		while(!queue.isEmpty())
		{
		    terr = (TerrainClass*)queue.removeFirstElement();
		    terr->setSandRegion(region);
		    for (angle = 0; angle < NUM_ANGLES; angle++)
		    {
			pos = getMapPos(angle, terr->location);
			if (cellExists(&pos) && !getCell(&pos)->isRock() && !getCell(&pos)->visited)
			{
			    queue.insertFirst(getCell(&pos));
			    getCell(&pos)->visited = true;
			}
		    }
		}
		region++;
	    }
	}
    }

    for (i = 0; i < w; i++)
    {
	for (j = 0; j < h; j++)
	{
	    getCell(i,j)->m_visited = false;
	}
    }
#endif
}

void MapClass::damage(ObjectPtr damager, PlayerClass* damagerOwner, UPoint realPos, 
	std::string objectName, int damage, int damagePiercing, int damageRadius, bool air)
{
    int i, j;
    UPoint centreCell = UPoint(realPos.x/BLOCKSIZE, realPos.y/BLOCKSIZE);
    SPoint pos;

    for (i = centreCell.x-2; i <= centreCell.x+2; i++)
	for (j = centreCell.y-2; j <= centreCell.y+2; j++)
	    if (cellExists((pos = SPoint(i, j))))
		getCell(pos)->damageCell(damager, damagerOwner, realPos, objectName, damage, damagePiercing, damageRadius, air);
}


ObjectPtr MapClass::findObjectWithID(int objectID, int lx, int ly)
{
    ObjectPtr object;
    SPoint pos = SPoint(lx, ly);

    //Units* unitList = GameMan::Instance()->GetUnits();

    if (cellExists(pos))
	object = getCell(pos)->getObjectWithID(objectID);


    if (!object)	{ //object wasn't found in expected cell
	//search surrounding cells
	for(int x = pos.x-5; x < pos.x+5 && !object; x++) {
	    for(int y = pos.y-5; y<pos.y+5 && !object; y++) {
		SPoint tmpPos = SPoint(x,y);
		if (cellExists(tmpPos)) {
		    object = getCell(tmpPos)->getObjectWithID(objectID);
		}
	    }
	}
#if 0
	if (object == NULL)	//object wasn't found in surrounding cells
	{	//search lists
	    if (!unitList->empty())
	    {
		unitList->saveCurrent();
		unitList->reset();

		while(unitList->currentNotNull() && !object)
		{
		    if (((UnitClass*)unitList->getCurrent())->hasObjectID(objectID))
			object = (UnitClass*)unitList->getCurrent();

		    unitList->nextLink();
		}

		unitList->restoreCurrent();
	    }

	    if (object == NULL)	//object wasn't found in units
	    {
		if (!structureList->isEmpty())
		{
		    structureList->saveCurrent();

		    structureList->reset();
		    while(structureList->currentNotNull() && !object)
		    {
			if (((StructureClass*)structureList->getCurrent())->hasObjectID(objectID))
			    object = (StructureClass*)structureList->getCurrent();

			structureList->nextLink();
		    }

		    structureList->restoreCurrent();
		}
	    }
	}
#endif
    }

    return object;
}

void MapClass::fixWall(SPoint pos)
{
    ObjectPtr object;
    if (cellExists(pos) && (object = getCell(pos)->getGroundObject()) && object->getObjectName() == "Wall")
    {
	bool up, down, left, right;

	int maketile = Structure_w8;

	// Walls
	up = checkPos(pos, SPoint(0, -1));
	down = checkPos(pos, SPoint(0, 1));
	left = checkPos(pos, SPoint(-1, 0));
	right = checkPos(pos, SPoint(1,0));

	// Now perform the test

	if (left && right && up && down)
	    maketile = Structure_w7; //solid wall

	else if (!left && right && up && down)
	    maketile = Structure_w2; //missing left edge

	else if (left && !right && up && down)
	    maketile = Structure_w3; //missing right edge

	else if (left && right && !up && down)
	    maketile = Structure_w4; //missing top edge

	else if (left && right && up && !down)
	    maketile = Structure_w5; //missing bottom edge

	else if (!left && right && !up && down)
	    maketile = Structure_w1; //missing top left edge

	else if (left && !right && up && !down)
	    maketile = Structure_w11; //missing bottom right edge

	else if (left && !right && !up && down)
	    maketile = Structure_w9; //missing top right edge

	else if (!left && right && up && !down)
	    maketile = Structure_w10; //missing bottom left edge

	else if (left && !right && !up && !down)
	    maketile = Structure_w8; //missing above, right and below

	else if (!left && right && !up && !down)
	    maketile = Structure_w8; //missing above, left and below

	else if (!left && !right && up && !down)
	    maketile = Structure_w6; //only up

	else if (!left && !right && !up && down)
	    maketile = Structure_w6; //only down

	else if (left && right && !up && !down)
	    maketile = Structure_w8; //missing above and below

	else if (!left && !right && up && down)
	    maketile = Structure_w6; //missing left and right

	///////
	//((WallClass*)getCell(i, j)->getGroundObject())->setTile(maketile);

    }
}

void MapClass::fixWalls(SPoint pos)
{
    fixWall(pos);

    fixWall(pos + SPoint(0, -1));
    fixWall(pos + SPoint(0, 1));
    fixWall(pos + SPoint(-1, 0));
    fixWall(pos + SPoint(1, 0));
}


/*inline*/        
TerrainClass * MapClass::getCell(SPoint pos) 
{
    if (cellExists(pos))
    {
	return m_cells[pos.x + pos.y * w];
    }
    else
    {
	LOG_ERROR("MapClass", "Cell[%d][%d] does not exist", pos.x,pos.y);
	return NULL;
    }   
}

UPoint MapClass::getMapPos(int angle, UPoint source)
{
    UPoint pos;

    switch (angle)
    {

	case (RIGHT):
	    pos.x = source.x + 1;
	    pos.y = source.y;
	    break;

	case (RIGHTUP):
	    pos.x = source.x + 1;
	    pos.y = source.y - 1;
	    break;

	case (UP):
	    pos.x = source.x;
	    pos.y = source.y - 1;
	    break;

	case (LEFTUP):
	    pos.x = source.x - 1;
	    pos.y = source.y - 1;
	    break;

	case (LEFT):
	    pos.x = source.x - 1;
	    pos.y = source.y;
	    break;

	case (LEFTDOWN):
	    pos.x = source.x - 1;
	    pos.y = source.y + 1;
	    break;

	case (DOWN):
	    pos.x = source.x;
	    pos.y = source.y + 1;
	    break;

	case (RIGHTDOWN):
	    pos.x = source.x + 1;
	    pos.y = source.y + 1;
	    break;

	default:
	    pos.x = source.x;
	    pos.y = source.y;
	    break;
    }

    return pos;
}

int MapClass::getPosAngle(UPoint source, UPoint pos)
{
    int angle;

    if (pos.x > source.x)
    {
	if (pos.y > source.y)
	    angle = RIGHTDOWN;
	else if (pos.y < source.y)
	    angle = RIGHTUP;
	else
	    angle = RIGHT;
    }

    else if (pos.x < source.x)
    {
	if (pos.y > source.y)
	    angle = LEFTDOWN;
	else if (pos.y < source.y)
	    angle = LEFTUP;
	else
	    angle = LEFT;
    }

    else
    {
	if (pos.y > source.y)
	    angle = DOWN;
	else if (pos.y < source.y)
	    angle = UP;
	else
	    angle = -1;
    }

    return angle;
}

void MapClass::removeObjectFromMap(Uint32 ObjectID)
{
    // TODO: This has still room for optimization..
    for(int y = 0; y < h ; y++) 
	for(int x = 0 ; x < w ; x++) 
	    if(getCell(SPoint(x, y))->unassignObject(ObjectID))
		return;
}

void MapClass::selectObjects(int playerNum, int x1, int y1, int x2, int y2, int realX, int realY, bool objectARGMode)
{


#if 0
    ObjectClass	*lastCheckedObject = NULL,
		*lastSelectedObject = NULL;

    //if selection rectangle is checking only one cell and has shift selected we want to add/ remove that unit from the selected group of units
    if (!objectARGMode)
    {
	unselectAll(selectedList);
	selectedList->clearList();
    }

    if ((x1 == x2) && (y1 == y2) && cellExists(x1, y1)) 
    {
	lastCheckedObject = cell[x1][y1].getObjectAt(realX, realY);
	if ((lastCheckedObject != NULL) && (lastCheckedObject->getOwner()->getPlayerNumber() == playerNum))
	{
	    if ((lastCheckedObject == lastSinglySelectedObject) && ( !lastCheckedObject->isAStructure()))
	    {
		for (int i = dborder->minX/BLOCKSIZE; i <= dborder->maxX/BLOCKSIZE; i++)
		    for (int j = dborder->minY/BLOCKSIZE; j <= dborder->maxY/BLOCKSIZE; j++)
			if (cellExists(i, j) && cell[i][j].hasAnObject())
			    cell[i][j].selectAllPlayersUnitsOfType(playerNum, lastSinglySelectedObject->getItemID(), &lastCheckedObject, &lastSelectedObject);
		lastSinglySelectedObject = NULL;
	    }
	    else if (!lastCheckedObject->isSelected())
	    {
		lastCheckedObject->setSelected(true);
		selectedList->insertFirst(lastCheckedObject);
		lastSelectedObject = lastCheckedObject;
		lastSinglySelectedObject = lastSelectedObject;
	    }
	    else if (objectARGMode)	//holding down shift, unselect this unit
	    {
		lastCheckedObject->setSelected(false);
		selectedList->removeElement(lastCheckedObject);
	    }
	}
	else
	    lastSinglySelectedObject = NULL;
    }
    else
    {
	lastSinglySelectedObject = NULL;
	for (int i = min(x1, x2); i <= max(x1, x2); i++)
	    for (int j = min(y1, y2); j <= max(y1, y2); j++)
		if (cellExists(i, j) && getCell(i,j)->hasAnObject() 
			&& getCell(i,j)->isExplored(playerNum) 
			&& !getCell(i,j)->.isFogged(playerNum) )
		    getCell(i,j)->selectAllPlayersUnits(playerNum, &lastCheckedObject, &lastSelectedObject);
    }

    //select an enemy unit if none of your units found
    if (selectedList->isEmpty() && (lastCheckedObject != NULL) && !lastCheckedObject->isSelected())
    {
	lastCheckedObject->setSelected(true);
	lastSelectedObject = lastCheckedObject;
	selectedList->insertFirst(lastCheckedObject);
    }
    else if (lastSelectedObject != NULL)
	lastSelectedObject->playSelectSound();	//we only want one unit responding

    /*
       if ((selectedList->getNumElements() == 1) && lastSelectedObject && lastSelectedObject->isAStructure() && ((StructureClass*)lastSelectedObject)->isABuilder())
       ((BuilderClass*)lastSelectedObject)->checkSelectionList();*/
#endif
}

void MapClass::viewMap(int playerTeam, UPoint location, int maxViewRange)
{
    int			i;
    SPoint	pos,
		check;
    pos.x = location.x,
	pos.y = location.y;

    GameMan* gman = GameMan::Instance();
    int number = gman->LocalPlayer()->getPlayerNumber();
    int team = gman->LocalPlayer()->getTeam();
    maxViewRange++;
    //makes map viewable in an area like as shown below 

    //				       *****
    //                   *********
    //                  *****T*****
    //                   *********
    //                     *****

    check.x = pos.x - maxViewRange;
    if (check.x < 0)
	check.x = 0;

    check.y = location.y;

    while ((check.x < w) && ((check.x - pos.x) <=  maxViewRange))
    {
	check.y = (pos.y - lookDist[abs(check.x - pos.x)]);
	if (check.y < 0) 
	    check.y = 0;

	while ((check.y < h) && ((check.y - pos.y) <= lookDist[abs(check.x - pos.x)]))
	{
	    if (distance_from(location, check) <= maxViewRange)
		for (i = 0; i < MAX_PLAYERS; i++)
		    if (gman->GetPlayer(i) && gman->GetPlayer(i)->getTeam() == playerTeam)
			getCell(SPoint(check.x, check.y))->setExplored(i, true);
	    check.y++;
	}

	check.x++;
	check.y = pos.y;
    }

    ///////////////smooth the hidden shade/hide/fogged area
    if (playerTeam == team)
    {
	bool	up = false, upEdge = false,
		down = false, downEdge = false,
		left = false, leftEdge = false,
		right = false, rightEdge = false;

	bool    upFog = false,
		downFog = false,
		leftFog = false,
		rightFog = false;

	int		hideTile = Terrain_HiddenFull;
	int		fogTile = Terrain_HiddenFull;

	check.x = pos.x - maxViewRange;
	if (check.x < 0)
	    check.x = 0;
	while ((check.x < w) && ((check.x - pos.x) <=  maxViewRange))
	{
	    check.y = (pos.y - lookDist[abs(check.x - pos.x)]);
	    if (check.y < 0) check.y = 0;

	    while ((check.y < h) && ((check.y - pos.y) <=  lookDist[abs(check.x - pos.x)]))
	    {
		if (distance_from(location, check) <= maxViewRange)
		{
		    hideTile = Terrain_HiddenFull;

		    upEdge = !cellExists(check.x, check.y-1);
		    downEdge = !cellExists(check.x, check.y+1);
		    leftEdge = !cellExists(check.x-1, check.y);
		    rightEdge = !cellExists(check.x+1, check.y);

		    up = !upEdge && (!getCell(SPoint(check.x,check.y-1))->isExplored(number));
		    down = !downEdge && (!getCell(SPoint(check.x,check.y+1))->isExplored(number));
		    left = !leftEdge && (!getCell(SPoint(check.x-1,check.y))->isExplored(number));
		    right = !rightEdge && (!getCell(SPoint(check.x+1,check.y))->isExplored(number));

#if 0
		    upFog = !upEdge && (getCell(check.x,check.y-1)->isFogged(number));
		    downFog = !downEdge && (getCell(check.x,check.y+1)->isFogged(number));
		    leftFog = !leftEdge && (getCell(check.x-1,check.y)->isFogged(number));
		    rightFog = !rightEdge && (getCell(check.x+1,check.y)->isFogged(number));
#endif 

		    // Now perform the test
		    if (left && right && up && down)
			hideTile = Terrain_HiddenFull;

		    else if (!left && right && up && down)
			hideTile = Terrain_HiddenNotLeft; //missing left edge

		    else if (left && !right && up && down)
			hideTile = Terrain_HiddenNotRight; //missing right edge

		    else if (left && right && !up && down)
			hideTile = Terrain_HiddenNotUp; //missing top edge

		    else if (left && right && up && !down)
			hideTile = Terrain_HiddenNotDown; //missing bottom edge

		    else if (!left && right && !up && down)
			hideTile = Terrain_HiddenDownRight; //missing top left edge

		    else if (left && !right && up && !down)
			hideTile = Terrain_HiddenUpLeft; //missing bottom right edge

		    else if (left && !right && !up && down)
			hideTile = Terrain_HiddenDownLeft; //missing top right edge

		    else if (!left && right && up && !down)
			hideTile = Terrain_HiddenUpRight; //missing bottom left edge

		    else if (left && right && !up && !down)
			hideTile = Terrain_HiddenLeftRight; //missing bottom up and down

		    else if (left && !right && !up && !down)
			hideTile = Terrain_HiddenLeft; //missing above, right and below 

		    else if (!left && right && !up && !down)
			hideTile = Terrain_HiddenRight; //missing above, left and below

		    else if (!left && !right && up && !down)
			hideTile = Terrain_HiddenUp; //only up

		    else if (!left && !right && !up && down)
			hideTile = Terrain_HiddenDown; //only down

		    else if (!left && !right && up && down)
			hideTile = Terrain_HiddenUpDown; //missing left and right

		    else if (!getCell(check)->isExplored(number) && !left && !right && !up && !down)
			hideTile = Terrain_HiddenIsland; //missing left and right
		    ///////
		    getCell(check)->setHideTile(hideTile);

		    //FIXME: Implement fog.
#if 0
		    if(!getCell(check.x,check.y)->isFogged(number))
		    {
			// do it again with fog
			fogTile = Terrain_HiddenFull;
			if (leftFog && rightFog && upFog && downFog)
			    fogTile = Terrain_HiddenFull;

			else if (!leftFog && rightFog && upFog && downFog)
			    fogTile = Terrain_HiddenNotLeft; //missing left edge

			else if (leftFog && !rightFog && upFog && downFog)
			    fogTile = Terrain_HiddenNotRight; //missing right edge

			else if (leftFog && rightFog && !upFog && downFog)
			    fogTile = Terrain_HiddenNotUp; //missing top edge

			else if (leftFog && rightFog && upFog && !downFog)
			    fogTile = Terrain_HiddenNotDown; //missing bottom edge

			else if (!leftFog && rightFog && !upFog && downFog)
			    fogTile = Terrain_HiddenDownRight; //missing top left edge

			else if (leftFog && !rightFog && upFog && !downFog)
			    fogTile = Terrain_HiddenUpLeft; //missing bottom right edge

			else if (leftFog && !rightFog && !upFog && downFog)
			    fogTile = Terrain_HiddenDownLeft; //missing top right edge

			else if (!leftFog && rightFog && upFog && !downFog)
			    fogTile = Terrain_HiddenUpRight; //missing bottom left edge

			else if (leftFog && rightFog && !upFog && !downFog)
			    fogTile = Terrain_HiddenLeftRight; //missing bottom up and down

			else if (leftFog && !rightFog && !upFog && !downFog)
			    fogTile = Terrain_HiddenLeft; //missing above, right and below 

			else if (!leftFog && rightFog && !upFog && !downFog)
			    fogTile = Terrain_HiddenRight; //missing above, left and below

			else if (!leftFog && !rightFog && upFog && !downFog)
			    fogTile = Terrain_HiddenUp; //only up

			else if (!leftFog && !rightFog && !upFog && downFog)
			    fogTile = Terrain_HiddenDown; //only down

			else if (!leftFog && !rightFog && upFog && downFog)
			    fogTile = Terrain_HiddenUpDown; //missing left and right

			/*else if (cell[check.x][check.y].isFogged(number) && leftFog && rightFog 
			  && upFog && downFog)
			  fogTile = Terrain_HiddenIsland; //missing left and right*/
			///////
			getCell(check)->setFogTile(fogTile);
		    }
		    else
			getCell(check)->setFogTile(Terrain_HiddenFull);
#endif 
		}

		check.y++;
	    }

	    check.x++;
	    check.y = pos.y;
	}
    }
}

void MapClass::viewMap(int playerTeam, int x, int y, int maxViewRange)
{
    UPoint pos;

    pos.x = x;
    pos.y = y;

    viewMap(playerTeam, pos, maxViewRange);
}

