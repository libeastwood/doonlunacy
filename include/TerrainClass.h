#ifndef DUNE_TERRAINCLASS_H
#define DUNE_TERRAINCLASS_H

#include "DataCache.h"
#include "Gfx.h"
#include "mmath.h"

class TerrainClass
{
  public:
    TerrainClass();
    ~TerrainClass();

    void draw(Image * dest, SPoint pos);

    	inline int getType() { return m_type; }
    inline int getTile() { return m_tile; }

    inline bool isMountain() { return (m_type == Terrain_Mountain);}
    inline bool isRock() { return ((m_type == Terrain_Rock) || (m_type == Structure_Slab1) || (m_type == Terrain_Mountain));}

    inline bool isSand() { return ((m_type == Terrain_Dunes) || (m_type == Terrain_Sand)); }
    inline bool isBloom() { return ((m_type == Terrain_Sand) && ((m_tile == Terrain_a2) || (m_tile == Terrain_a3))); }
    inline bool isSpice() { return ((m_type == Terrain_Spice) || (m_type == Terrain_ThickSpice)); }
    inline bool isThickSpice() { return (m_type == Terrain_ThickSpice); }

    inline void setTile(int newTile) { m_tile = newTile; }
    inline void setType(int newType) { m_type = newType; }

  private:
 
    /*!
     * tile assigned to current cell
     * @note need pretile because when smoothing random map, you need to know what all the tiles are before smoothed
     */
    int m_tile;
    
    int m_type;
    
    ImagePtr m_img;
    
};

#endif // DUNE_TERRAINCLASS_H

