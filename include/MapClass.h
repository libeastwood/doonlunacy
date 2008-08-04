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

    inline bool cellExists(SPoint pos);
    inline bool cellExists(int x, int y) { return cellExists(SPoint(x,y)); }

    inline TerrainClass * getCell(SPoint pos);    
    inline TerrainClass * getCell(int x, int y) { return getCell(SPoint(x,y)); }

    UPoint getMapPos(int angle, UPoint source);
    int getPosAngle(UPoint source, UPoint pos);

    void fixWalls(int xPos, int yPos);

    void viewMap(int playerTeam, UPoint position, int maxViewRange);

    //FIXME:Remove this later or sth.
    short *depthCheckCount,
    ***depthCheckMax;

  protected:
    void fixWall(int xPos, int yPos);
    
    Cells m_cells;
    GameState* m_gs;
};

#endif // DUNE_MAPCLASS_H

