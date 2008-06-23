#ifndef DUNE_MAPCLASS_H
#define DUNE_MAPCLASS_H

#include <vector>

#include "Definitions.h"
#include "Gfx.h"
#include "Log.h"
#include "TerrainClass.h"

class MapGenerator;

typedef std::vector<TerrainClass*> Cells;

class MapClass : public Rect
{
  friend class MapGenerator;

  public:
    MapClass(UPoint size);
    ~MapClass();

	inline bool cellExists(SPoint pos) 
	{
		return ((pos.x >= 0) && (pos.x < w) && (pos.y >= 0) && (pos.y < h));
	};

    inline bool cellExists(int x, int y) { return cellExists(SPoint(x,y)); };

        
    inline TerrainClass * getCell(SPoint pos) 
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
    };
    
    inline TerrainClass * getCell(int x, int y) { return getCell(SPoint(x,y)); }
    UPoint getMapPos(int angle, UPoint source);
    int getPosAngle(UPoint source, UPoint pos);

    void fixWalls(int xPos, int yPos);

    //FIXME:Remove this later or sth.
    short *depthCheckCount,
    ***depthCheckMax;

  protected:
    void fixWall(int xPos, int yPos);
    
    Cells m_cells;
};

#endif // DUNE_MAPCLASS_H

