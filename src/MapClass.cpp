#include "MapClass.h"

MapClass::MapClass(UPoint size)
{
    w = size.x;
    h = size.y;
    
    int i;
    for (i = 0; i < w*h; i++)
    {
        m_cells.push_back(new TerrainClass());
    }
}

MapClass::~MapClass()
{

}
/*
void MapClass::fixWall(int xPos, int yPos)
{
	if (getCell(xPos,yPos)->hasAGroundObject() && getCell(xPos,yPos)->getGroundObject()->getItemID() == Structure_Wall)
	{
		int up, down, left, right;

		int i = xPos, j = yPos,
			maketile = Structure_w8;
			
		// Walls
		up = (!cellExists(i, j-1) || (getCell(i,j-1)->hasAGroundObject()
			&& (getCell(i,j-1)->getGroundObject()->getItemID() == Structure_Wall)));
				//|| (cell[i][j-1].getGroundObject()->getItemID() == Structure_GunTurret)
				//|| (cell[i][j-1].getGroundObject()->getItemID() == Structure_RocketTurret))));
		down = (!cellExists(i, j+1) || (getCell(i,j+1)->hasAGroundObject()
			&& (getCell(i,j+1)->getGroundObject()->getItemID() == Structure_Wall)));
		left = (!cellExists(i-1, j) || (getCell(i-1,j)->hasAGroundObject()
			&& (getCell(i-1,j)->getGroundObject()->getItemID() == Structure_Wall)));
		right = (!cellExists(i+1, j) || (getCell(i+1,j)->hasAGroundObject()
			&& (getCell(i+1,j)->getGroundObject()->getItemID() == Structure_Wall)));

			// Now perform the test
		if ((left == 1) && (right == 1) && (up == 1) && (down == 1))
				maketile = Structure_w7; //solid wall

		else if ((left == 0) && (right == 1) && (up == 1) && (down == 1))
				maketile = Structure_w2; //missing left edge

		else if ((left == 1) && (right == 0)&& (up == 1) && (down == 1))
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
		((WallClass*)getCell(i,j)->getGroundObject())->setTile(maketile);
	}	
}

void MapClass::fixWalls(int xPos, int yPos)
{
	fixWall(xPos, yPos);

	if (cellExists(xPos, yPos-1))
		fixWall(xPos, yPos-1);
	if (cellExists(xPos, yPos+1))
		fixWall(xPos, yPos+1);
	if (cellExists(xPos-1, yPos))
		fixWall(xPos-1, yPos);
	if (cellExists(xPos+1, yPos))
		fixWall(xPos+1, yPos);
}
*/
