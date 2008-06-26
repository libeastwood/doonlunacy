#include "Log.h"
#include "MapClass.h"
#include "structures/WallClass.h"
MapClass::MapClass(UPoint size)
{
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
			LOG_INFO("MapClass", "depthCheckMax[%d][%d][%d] is %d.", i, j, k, depthCheckMax[i][j][k]);
		}
	}
}

MapClass::~MapClass()
{

}

void MapClass::fixWall(int xPos, int yPos)
{
    if (getCell(xPos, yPos)->hasAGroundObject() &&
            getCell(xPos, yPos)->getGroundObject()->getItemID() == Structure_Wall)
    {
        int up, down, left, right;

        int i = xPos, j = yPos,
                          maketile = Structure_w8;

        // Walls
        up = (!cellExists(i, j - 1) || (getCell(i, j - 1)->hasAGroundObject()
                                        && (getCell(i, j - 1)->getGroundObject()->getItemID() == Structure_Wall)));
        //|| (cell[i][j-1].getGroundObject()->getItemID() == Structure_GunTurret)
        //|| (cell[i][j-1].getGroundObject()->getItemID() == Structure_RocketTurret))));
        down = (!cellExists(i, j + 1) || (getCell(i, j + 1)->hasAGroundObject()
                                          && (getCell(i, j + 1)->getGroundObject()->getItemID() == Structure_Wall)));

        left = (!cellExists(i - 1, j) || (getCell(i - 1, j)->hasAGroundObject()
                                          && (getCell(i - 1, j)->getGroundObject()->getItemID() == Structure_Wall)));

        right = (!cellExists(i + 1, j) || (getCell(i + 1, j)->hasAGroundObject()
                                           && (getCell(i + 1, j)->getGroundObject()->getItemID() == Structure_Wall)));

        // Now perform the test

        if ((left == 1) && (right == 1) && (up == 1) && (down == 1))
            maketile = Structure_w7; //solid wall

        else if ((left == 0) && (right == 1) && (up == 1) && (down == 1))
            maketile = Structure_w2; //missing left edge

        else if ((left == 1) && (right == 0) && (up == 1) && (down == 1))
            maketile = Structure_w3; //missing right edge

        else if ((left == 1) && (right == 1) && (up == 0) && (down == 1))
            maketile = Structure_w4; //missing top edge

        else if ((left == 1) && (right == 1) && (up == 1) && (down == 0))
            maketile = Structure_w5; //missing bottom edge

        else if ((left == 0) && (right == 1) && (up == 0) && (down == 1))
            maketile = Structure_w1; //missing top left edge

        else if ((left == 1) && (right == 0) && (up == 1) && (down == 0))
            maketile = Structure_w11; //missing bottom right edge

        else if ((left == 1) && (right == 0) && (up == 0) && (down == 1))
            maketile = Structure_w9; //missing top right edge

        else if ((left == 0) && (right == 1) && (up == 1) && (down == 0))
            maketile = Structure_w10; //missing bottom left edge

        else if ((left == 1) && (right == 0) && (up == 0) && (down == 0))
            maketile = Structure_w8; //missing above, right and below

        else if ((left == 0) && (right == 1) && (up == 0) && (down == 0))
            maketile = Structure_w8; //missing above, left and below

        else if ((left == 0) && (right == 0) && (up == 1) && (down == 0))
            maketile = Structure_w6; //only up

        else if ((left == 0) && (right == 0) && (up == 0) && (down == 1))
            maketile = Structure_w6; //only down

        else if ((left == 1) && (right == 1) && (up == 0) && (down == 0))
            maketile = Structure_w8; //missing above and below

        else if ((left == 0) && (right == 0) && (up == 1) && (down == 1))
            maketile = Structure_w6; //missing left and right

        ///////
        ((WallClass*)getCell(i, j)->getGroundObject())->setTile(maketile);

    }
}

void MapClass::fixWalls(int xPos, int yPos)
{
    fixWall(xPos, yPos);

    if (cellExists(xPos, yPos - 1))
        fixWall(xPos, yPos - 1);

    if (cellExists(xPos, yPos + 1))
        fixWall(xPos, yPos + 1);

    if (cellExists(xPos - 1, yPos))
        fixWall(xPos - 1, yPos);

    if (cellExists(xPos + 1, yPos))
        fixWall(xPos + 1, yPos);
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

