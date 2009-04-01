#include <math.h>

#include "Definitions.h"
#include "GCObject.h"
#include "GameMan.h"
#include "Gfx.h"
#include "Log.h"
#include "MapClass.h"
#include "Settings.h"
#include "SoundPlayer.h"

#include "objects/ObjectClass.h"
#include "objects/UnitClass.h"

UnitClass::UnitClass(PlayerClass* newOwner, std::string unitName, uint32_t attribute) : ObjectClass(newOwner, unitName, attribute | OBJECT_UNIT)
{
    DataCache *cache = DataCache::Instance();

    m_attacking = false;
    m_canAttackStuff = true;
    m_moving = false;
    m_destroyed = false;
    m_pickedUp = false;
    m_justStoppedMoving = false;
    m_turning = false;
    m_tracked = false;
    m_turreted = false;
    m_goingToRepairYard = false;
    m_nextSpotFound = false;
    m_respondable = true;
    m_attackMode = DEFENSIVE;

    try {
	std::vector<std::string> soundStrings = cache->getPyObjectVector<std::string>(m_objectName, "confirmSound");
	m_confirmSound.resize(soundStrings.size());
	for(size_t i = 0; i < soundStrings.size(); i++)
	    m_confirmSound[i] = DataCache::Instance()->getGCObject(soundStrings[i])->getSound();
	soundStrings = cache->getPyObjectVector<std::string>(m_objectName, "selectSound");
	m_selectSound.resize(soundStrings.size());
	for(size_t i = 0; i < soundStrings.size(); i++)
	    m_selectSound[i] = DataCache::Instance()->getGCObject(soundStrings[i])->getSound();
    }
    catch(python::error_already_set const &)
    {
	LOG_FATAL("UnitClass", "Error loading object: %s", m_objectName.c_str());
	PyErr_Print();
	exit(1);
    }

    m_speedCap = NONE;


    m_destination = SPoint(INVALID_POS, INVALID_POS);
    m_guardPoint = SPoint(INVALID_POS, INVALID_POS);
    m_nextSpot = SPoint(INVALID_POS, INVALID_POS);
    setAngle(LEFT);
    setActive(false);
}

/*virtual*/
UnitClass::~UnitClass()
{
    LOG_INFO("UnitClass", "Unit deleted");
}


bool UnitClass::canPass(UPoint pos)
{
    MapClass * map = m_owner->getMap();

    return (map->cellExists(pos) && !map->getCell(pos)->hasAGroundObject() && !map->getCell(pos)->isMountain());
}

/*virtual*/
void UnitClass::deploy(SPoint newPosition)
{
    MapClass* map = m_owner->getMap();
    
    if (map->cellExists(newPosition))
    {
        setPosition(newPosition);

        if ((m_guardPoint.x == INVALID_POS) || (m_guardPoint.y == INVALID_POS))
            m_guardPoint = UPoint(x, y);

        setDestination(m_guardPoint);

        //  pickedUp = false;

        setRespondable(true);

        setActive(true);

        setVisible(VIS_ALL, true);

        //FIXME: This decreases cpu consumption by about 30%-40%, but causes problems if fog of war is enabled.
        //       Need to think of sth more effective.
        map->viewMap(m_owner->getTeam(), getPosition(), getViewRange() );

    }

}

void UnitClass::destroy()
{
    GameMan* gman = GameMan::Instance();
    if (!m_destroyed)
    {
        LOG_INFO("UnitClass","Destroying unit %d (objectName=%s)... ",m_objectID, m_objectName.c_str());
        setTarget(ObjectPtr());
        gman->GetMap()->removeObjectFromMap(getObjectID()); //no map point will reference now
        //gman->GetObjectTree()->RemoveObject(getObjectID());

        m_owner->decrementUnits(m_objectName);

        m_respondable = false;
/*
        imageW = graphic->w / numDeathFrames;
        imageH = graphic->h;
        xOffset = (imageW - BLOCKSIZE) / 2;    //this is where it actually draws the graphic
        yOffset = (imageH - BLOCKSIZE) / 2;    //cause it draws at top left, not middle
*/
        //m_frameTimer = m_frameTime;
        //m_deathFrame = 0;

        //if (isVisible(getOwner()->getTeam()))
        //    PlayDestroySound();

        //gman->GetUnits()->remove(this);

        //delete this;

        //if (map->cellExists(&location))
        // map->getCell(&location)->assignDeadObject(this);
    }
    ObjectClass::destroy();

}


/*virtual*/
void UnitClass::draw(Image * dest, SPoint off, SPoint view)
{
    ObjectClass::draw(dest, off, view);

    // Show path on the screen
    #if 1

    if (m_selected && !m_pathList.empty())
    {
        Path::iterator iter = m_pathList.begin();
        Rect rect;

        while (iter != m_pathList.end())
        {
            rect.x = off.x + (*iter).x * BLOCKSIZE - view.x * BLOCKSIZE + BLOCKSIZE / 2;
            rect.y = off.y + (*iter).y * BLOCKSIZE - view.y * BLOCKSIZE + BLOCKSIZE / 2;
            rect.w = 2;
            rect.h = 2;
            dest->drawRect(rect, houseColour[m_owner->getColour()]);
            iter++;
        }
    }
    #endif

}

/*virtual*/
void UnitClass::move()
{
    MapClass* map = m_owner->getMap();
    // if(!m_moving && getRandomInt(0,40) == 0)
    //TODO:Not implemented yet.
    if (m_moving)
    {
        m_oldPosition = UPoint(x, y);

        if (!m_badlyDamaged || isAFlyingUnit())
        {
            m_realPos.x += m_xSpeed * m_adjust;
            m_realPos.y += m_ySpeed * m_adjust;
        }

        else
        {
            m_realPos.x += (m_xSpeed / 2) * m_adjust;
            m_realPos.y += (m_ySpeed / 2) * m_adjust;
        }

        // if vehicle is half way out of old cell

        if ((abs(x*BLOCKSIZE - (int)m_realPos.x + BLOCKSIZE / 2) > BLOCKSIZE / 2)
                || (abs(y*BLOCKSIZE - (int)m_realPos.y + BLOCKSIZE / 2) > BLOCKSIZE / 2))
        {
            unassignFromMap(m_oldPosition); //let something else go in

            // if vehicle is out of old cell

            if ((abs(x*BLOCKSIZE - (int)m_realPos.x + BLOCKSIZE / 2) > BLOCKSIZE)
                    || (abs(y*BLOCKSIZE - (int)m_realPos.y + BLOCKSIZE / 2) > BLOCKSIZE))
            {
                x = m_nextSpot.x;
                y = m_nextSpot.y;

                if (getPosition() == m_destination)
                    setForced(false);

                m_moving = false;

                m_justStoppedMoving = true;

                map->viewMap(m_owner->getTeam(), getPosition(), getViewRange() );
            }
        }
    }

    else
    {
        m_justStoppedMoving = false;
    }

    checkPos();
}

/*virtual*/
void UnitClass::navigate()
{
    if (!m_moving)
    {
        if ((x != m_destination.x) || (y != m_destination.y))
        {
            if (!m_nextSpotFound)
            {
                if (m_nextSpotAngle == m_drawnAngle)
                {
                    if (m_pathList.empty() && (m_checkTimer == 0))
                    {
                        m_checkTimer = 100;

                        if (!AStarSearch() && (++m_noCloserPointCount >= 3)
                                && ((x != m_oldPosition.x) || (y != m_oldPosition.y)))
                        { //try searching for a path a number of times then give up
                            if (m_target && m_targetFriendly
                                    && (m_target->getObjectName() != "Repair Yard")
                                    && ((m_target->getObjectName() != "Refinery")
                                        || (getObjectName() != "Harvester")))
                            {

                                setTarget(ObjectPtr());
                            }

                            setDestination(UPoint(x, y)); //can't get any closer, give up

                            m_forced = false;
                            m_speedCap = NONE;
                        }
                    }

                    if (!m_pathList.empty())
                    {
                        m_nextSpot = m_pathList.front();
                        m_pathList.pop_front();
                        m_nextSpotFound = true;
                        m_checkTimer = 0;
                        m_noCloserPointCount = 0;
                    }
                }
            }

            else
            {
                int tempAngle;

                if ((tempAngle = m_owner->getMap()->getPosAngle(UPoint(x, y), m_nextSpot)) != -1)
                    m_nextSpotAngle = tempAngle;

                if (!canPass(m_nextSpot))
                {
                    m_nextSpotFound = false;
                    m_pathList.clear();
                }

                else if (m_drawnAngle == m_nextSpotAngle)
                {
                    m_moving = true;
                    m_nextSpotFound = false;
                    assignToMap(m_nextSpot);
                    m_angle = m_drawnAngle;
                    setSpeeds();

                }
            }
        }

        else if (!m_target) //not moving and not wanting to go anywhere, do some random turning
        {
            if (getRandomInt(0, RANDOMTURNTIMER) == 0)
                m_nextSpotAngle = getRandomInt(0, 7); //choose a random one of the eight possible angles
        }
    }
}

void UnitClass::setAngle(int newAngle)
{
    if (!m_moving && (newAngle >= 0) && (newAngle < NUM_ANGLES))
    {
        m_angle = m_drawnAngle = newAngle;
        m_nextSpotAngle = m_drawnAngle;
        m_nextSpotFound = false;
    }
}


void UnitClass::playConfirmSound() {
    if(!m_confirmSound.empty())
    	SoundPlayer::Instance()->playSound(m_confirmSound[getRandomInt(0,m_confirmSound.size()-1)].get());
}

void UnitClass::playSelectSound() {
    if(!m_selectSound.empty())
    	SoundPlayer::Instance()->playSound(m_selectSound[getRandomInt(0,m_selectSound.size()-1)].get());
}
/*virtual*/
void UnitClass::setDestination(SPoint destination)
{
    m_pathList.clear();
    if(m_guardPoint != destination)
	playConfirmSound();
    ObjectClass::setDestination(destination);
}

void UnitClass::setSelected(bool value) {
    ObjectClass::setSelected(value);
    if(value)
	playSelectSound();
}

void UnitClass::setGuardPoint(UPoint newGuardPoint)
{
	setGuardPoint(newGuardPoint.x, newGuardPoint.y);
}

void UnitClass::setGuardPoint(int newX, int newY)
{
	MapClass* map = m_owner->getMap();
	
	if (map->cellExists(newX, newY) || ((newX == INVALID_POS) && (newY == INVALID_POS)))
	{
		m_guardPoint.x = newX;
		m_guardPoint.y = newY;
	}
}

void UnitClass::setPosition(SPoint pos)
{
    if ((pos.x == INVALID_POS) && (pos.y == INVALID_POS))
    {
        ObjectClass::setPosition(pos);
    }
    else if (m_owner->getMap()->cellExists(pos))
    {
        ObjectClass::setPosition(pos);
        m_realPos.x += BLOCKSIZE / 2;
        m_realPos.y += BLOCKSIZE / 2;
    }

    m_moving = false;

    m_nextSpotFound = false;
    m_nextSpotAngle = m_drawnAngle;
    m_pickedUp = false;
    setTarget(ObjectPtr());
    // clearPath
    m_pathList.clear();
    m_noCloserPointCount = 0;
}

/*virtual*/
void UnitClass::setSpeeds()
{
    float maxSpeed = m_speed;

    if (!isAFlyingUnit())
    {
        m_speed += m_speed * (1.0 - m_owner->getMap()->getCell(SPoint(x, y))->getDifficulty());
        m_speed *= HEAVILYDAMAGEDSPEEDMULTIPLIER;
    }

    if ((m_speedCap > 0) && (m_speedCap < m_speed))
        m_speed = m_speedCap;

    switch (m_drawnAngle)
    {

        case (LEFT):
            m_xSpeed = -m_speed;
            m_ySpeed = 0;
            break;

        case (LEFTUP):
            m_xSpeed = -m_speed * DIAGONALSPEEDCONST;
            m_ySpeed = m_xSpeed;
            break;

        case (UP):
            m_xSpeed = 0;
            m_ySpeed = -m_speed;
            break;

        case (RIGHTUP):
            m_xSpeed = m_speed * DIAGONALSPEEDCONST;
            m_ySpeed = -m_xSpeed;
            break;

        case (RIGHT):
            m_xSpeed = m_speed;
            m_ySpeed = 0;
            break;

        case (RIGHTDOWN):
            m_xSpeed = m_speed * DIAGONALSPEEDCONST;
            m_ySpeed = m_xSpeed;
            break;

        case (DOWN):
            m_xSpeed = 0;
            m_ySpeed = m_speed;
            break;

        case (LEFTDOWN):
            m_xSpeed = -m_speed * DIAGONALSPEEDCONST;
            m_ySpeed = -m_xSpeed;
    }

    m_speed = maxSpeed;
}


/*virtual*/

void UnitClass::setTarget(ObjectPtr newTarget)
{
#if 0
	if (goingToRepairYard && target && (target.getObjPointer()->getItemID() == Structure_RepairYard))
	{
		((RepairYardClass*)target.getObjPointer())->unBook();
		goingToRepairYard = false;
	}

	ObjectClass::setTarget(newTarget);

	if(target
		&& (target.getObjPointer()->getOwner() == getOwner())
		&& (target.getObjPointer()->getItemID() == Structure_RepairYard)
		&& (itemID != Unit_Carryall) && (itemID != Unit_Frigate)
		&& (itemID != Unit_Ornithopter))
	{
		((RepairYardClass*)target.getObjPointer())->book();
		goingToRepairYard = true;
	}
#endif
}

void UnitClass::targeting()
{
#if 0
	if (!target && !moving && !forced && (attackMode != SCOUT) && (findTargetTimer == 0) && (currentGame->playerType != CLIENT))
	{
		target.PointTo(findTarget());

		if (target && isInAttackModeRange(target.getObjPointer()))
		{
			netDoAttack(target.getObjPointer());
			pathList.clearList();
			nextSpotFound = false;
			speedCap = NONE;
		}
		else
		{
			target.PointTo(NONE);

			if (attacking && (currentGame->playerType != CLIENT))
			{
				StructureClass* closestStructure = (StructureClass*)findClosestTargetStructure(this);
				if (closestStructure) {
					COORDTYPE closestPoint = closestStructure->getClosestPoint(&location);
					netDoCommand(NULL, &closestPoint, false);
				} else {
					UnitClass* closestUnit = (UnitClass*)findClosestTargetUnit(this);
					if (closestUnit)
						netDoCommand(NULL, closestUnit->getLocation(), false);
				}
			}
		}

		findTargetTimer = 100;
	}

	if (target)
		engageTarget();
#endif
}

void UnitClass::turnLeft()
{
    m_angle += m_turnSpeed * m_adjust;

    if (m_angle >= 7.5)
        m_angle -= 8.0;

    m_drawnAngle = lround(m_angle);
}

void UnitClass::turnRight()
{
    m_angle -= m_turnSpeed * m_adjust;

    if (m_angle < -0.5)
        m_angle += 8;

    m_drawnAngle = lround(m_angle);
}

/*virtual*/
void UnitClass::turn()
{
    if (!m_moving)
    {
        int wantedAngle;
        
        if (m_target && (!m_targetFriendly || (m_targetDistance < 1.0)) && (m_targetDistance <= m_weaponRange))
            wantedAngle = m_targetAngle;
        else
            wantedAngle = m_nextSpotAngle;

        if (wantedAngle != -1)
        {
            if (m_justStoppedMoving)
            {
                m_angle = wantedAngle;
                m_drawnAngle = lround(m_angle);
            }

            else
            {
                float angleLeft = 0,
                                   angleRight = 0;

                if (m_angle > wantedAngle)
                {
                    angleRight = m_angle - wantedAngle;
                    angleLeft = fabs(8 - m_angle) + wantedAngle;
                }

                else if (m_angle < wantedAngle)
                {
                    angleRight = abs(8 - wantedAngle) + m_angle;
                    angleLeft = wantedAngle - m_angle;
                }

                if (angleLeft <= angleRight)
                    turnLeft();
                else
                    turnRight();
            }
        }
    }
}

/*virtual*/
void UnitClass::update(float dt)
{
    ObjectClass::update(dt);
    if (!m_destroyed)
    {
        if (m_active)
        {
            targeting();
            navigate();
            move();

            if (m_active)
                turn();
        }

        if (m_badlyDamaged)
        {
            if (m_health <= 0)
            {
                destroy();
                return;
            }
    }
    }

#if 0 
    if (m_badlyDamaged)
    {
        if (m_health <= 0)
        {
            //FIXME: Network game
            //netDestroy();
            return;
        }

        else if (!m_goingToRepairYard
                 && m_owner->isAI()
                 && m_owner->hasRepairYard()
                 && !m_forced
                 && !m_target)
                 //FIXME: What's this supposed to do?
                 && (GamecurrentGame->playerType != CLIENT))
            repair();
    }
}

if (m_smokeFrame != -1)
{
    if (++m_smokeFrame > 2)
        m_smokeFrame = 0;
}

if (m_frameTimer > 0)  //death frame has started
{
    if (m_frameTimer == 1)
        frameChange();

    m_frameTimer--;
}



if (!m_destroyed)
{
    if (m_checkTimer > 0) m_checkTimer--;


    if (m_findTargetTimer > 0) findTargetTimer--;

    if (m_primaryWeaponTimer > 0) primaryWeaponTimer--;

    if (m_secondaryWeaponTimer > 0) secondaryWeaponTimer--;

    if (m_deviationTimer > 0)
    {
        if (--m_deviationTimer == 0)
        { //revert back to real owner
            removeFromSelectionLists();
            setTarget(NULL);
            setGuardPoint(&location);
            setDestination(&location);
            m_owner = m_realOwner;
            m_graphic = DataCache::Instance->getObjPic(GraphicID, getOwner()->getColour());
        }
    }

}
#endif 

}

/* search algorithmns */
void UnitClass::nodePushSuccesors(PriorityQ* open, TerrainClass* parent_node)
{
    int dx1, dy1, dx2, dy2;
    float cost,
    cross,
    heuristic,
    f;
    
    MapClass* map = m_owner->getMap();

    UPoint  checkedPoint = m_destination,
                           tempLocation;
    TerrainClass* node;

    //push a node for each direction we could go

    for (int angle = 0; angle <= 7; angle++) //going from angle 0 to 7 inc
    {
        tempLocation  = map->getMapPos(angle, UPoint(parent_node->x, parent_node->y));

        if (canPass(tempLocation))
        {
            node = map->getCell(tempLocation);
            cost = parent_node->m_cost;

            if ((x != parent_node->x) && (tempLocation.y != parent_node->y))
                cost += DIAGONALCOST * (isAFlyingUnit() ? 1.0 : (float)node->getDifficulty()); //add diagonal movement cost
            else
                cost += (isAFlyingUnit() ? 1.0 : (float)node->getDifficulty());

            /*if (parent_node->parent) //add cost of turning time
            {
             int posAngle = map->getPosAngle(parent_node->parent->getLocation(), parent_node->getLocation());
             if (posAngle != angle)
              cost += (1.0/turnSpeed * (float)min(abs(angle - posAngle), NUM_ANGLES - max(angle, posAngle) + min(angle, posAngle)))/((float)BLOCKSIZE);
            }*/

            if (m_target)
                checkedPoint = m_target->getClosestPoint(tempLocation);

            dx1 = tempLocation.x - checkedPoint.x;

            dy1 = tempLocation.y - checkedPoint.y;

            dx2 = x - checkedPoint.x;

            dy2 = y - checkedPoint.y;

            cross = (float)(dx1 * dy2 - dx2 * dy1);

            if ( cross < 0 )
                cross = -cross;

            heuristic = blockDistance(tempLocation, checkedPoint);// + cross*0.1;//01;

            f = cost + heuristic;

            if (node->m_visited) //if we have already looked at this node before
                if (node->m_f <= f) //if got here with shorter travel time before
                    continue;

            TerrainClass* tempNode;

            if ((tempNode = open->find(tempLocation)))
            {
                if (tempNode->m_f <= f)
                    continue;

                open->removeNodeWithKey(tempLocation);
            }

            node->m_cost = cost;

            node->m_heuristic = heuristic;
            node->m_f = f;
            node->m_parent = parent_node;
            open->push_back(node);
        }
    }
}

bool UnitClass::AStarSearch()
{
    MapClass* map = m_owner->getMap();
    int numNodesChecked = 0;
    UPoint checkedPoint;

    TerrainClass *node = map->getCell(UPoint(x, y));//initialise the current node the object is on

    if (m_target)
        checkedPoint = m_target->getClosestPoint(UPoint(x, y));
    else
        checkedPoint = m_destination;

    node->m_f = node->m_heuristic = blockDistance(UPoint(x, y), checkedPoint);

    /*for (int i=0; i<max(map->sizeX, map->sizeY)-1; i++)
     if (map->depthCheckCount[i] != 0) //very very bad if this happens, check if its in visited list and being reset to not visited
      selected = true;*/

    //if the unit is not directly next to its dest, or it is and the dest is unblocked
    if ((node->m_heuristic > 1.5) || canPass(checkedPoint))
    {
        float smallestHeuristic = node->m_heuristic;
        PriorityQ open(map->w*map->h);
        std::list<TerrainClass*> visitedList;
        TerrainClass *bestDest = NULL; //if we dont find path to destination, we will head here instead

        node->m_parent = NULL;
        node->m_cost = 0.0;
        open.push_back(node);

        //short maxDepth = max(map->sizeX, map->sizeY),
        short   depth;

        while (!open.empty())
        {
            //take the closest node to target out of the queue
            node = open.pop_front();

            if (node->m_heuristic < smallestHeuristic)
            {
                smallestHeuristic = node->m_heuristic;
                bestDest = node;

                if (node->m_heuristic == 0.0)   //if the distance from this node to dest is zero, ie this is the dest node
                    break; //ive found my dest!
            }

            if (numNodesChecked < Settings::Instance()->GetMaxSearchPath())
                nodePushSuccesors(&open, node);

            if (!node->m_visited)
            {
                depth = std::max(abs(node->x - checkedPoint.x), abs(node->y - checkedPoint.y));

                if (++map->depthCheckCount[depth] >= map->depthCheckMax[checkedPoint.x][checkedPoint.y][depth])
                    break; //we have searched a whole square around destination, it cant be reached

                visitedList.push_front(node); //so know which ones to reset to unvisited

                node->m_visited = true;

                numNodesChecked++;

                //if (debug) //see all spots checked
                // owner->placeUnit(Unit_Carryall, node->location.x, node->location.y);
            }
        }

        while (!visitedList.empty())
        {
            node = visitedList.front();
            visitedList.pop_front();
            node->m_visited = false;
            depth = std::max(abs(node->x - checkedPoint.x), abs(node->y - checkedPoint.y));
            map->depthCheckCount[depth] = 0;
        }

        //go to closest point to dest if is one

        if (bestDest != NULL)
        {
            node = bestDest;

            while (node->m_parent != NULL)
            {
                m_nextSpot = UPoint(node->x, node->y);
                m_pathList.push_front(UPoint(node->x, node->y));
                //if (debug) //see final path
                //getOwner()->placeUnit(Unit_Carryall, nextSpot.x, nextSpot.y);
                node = node->m_parent;
            }

            LOG_INFO("UnitClass", "%s at %d,%d to %d, %d: %d", m_objectName.c_str(), x, y, m_destination.x, m_destination.y, numNodesChecked);

            return true;
        }
    }

    //no closer point found
    return false;
}

