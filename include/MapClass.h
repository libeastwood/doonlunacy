#ifndef DUNE_MAPCLASS_H
#define DUNE_MAPCLASS_H

#include <vector>

#include "Definitions.h"
#include "Gfx.h"
#include "Log.h"
#include "TerrainClass.h"

class MapGenerator;
class GameState;

typedef std::vector<TerrainClass*> Cells;

class MapClass : public Rect
{
  friend class MapGenerator;

  public:
    MapClass(UPoint size);
    ~MapClass();

    bool cellExists(SPoint pos);
    bool cellExists(int x, int y) { return cellExists(SPoint(x,y)); }

    TerrainClass * getCell(SPoint pos);    
    TerrainClass * getCell(int x, int y) { return getCell(SPoint(x,y)); }

    UPoint getMapPos(int angle, UPoint source);
    int getPosAngle(UPoint source, UPoint pos);

    void fixWalls(int xPos, int yPos);

	ObjectClass* findObjectWithID(int objectID, int lx, int ly);
    //FIXME:Remove this later or sth.
    short *depthCheckCount,
    ***depthCheckMax;
    
    void createSandRegions();
    void selectObjects(int playerNum, int x1, int y1, int x2, int y2, int realX, int realY, bool objectARGMode);

	BLOCKEDTYPE cellBlocked(UPoint pos);
	void viewMap(int playerTeam, UPoint location, int maxViewRange);
	void viewMap(int playerTeam, int x, int y, int maxViewRange);
    void fixWall(int xPos, int yPos);	
  protected:

    Cells m_cells;
    
  private: 
	ObjectClass* lastSinglySelectedObject;
};

#endif // DUNE_MAPCLASS_H

