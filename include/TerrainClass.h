#ifndef DUNE_TERRAINCLASS_H
#define DUNE_TERRAINCLASS_H

#include "DataCache.h"
#include "Gfx.h"
#include "mmath.h"
//#include "ObjectClass.h"

class TerrainClass
{
  public:
    TerrainClass();
    ~TerrainClass();

    void draw(Image * dest, SPoint pos);
/*    
   	inline bool hasADeadObject() { return !m_assignedDeadObjects.empty(); }
	inline bool hasAGroundObject() { return (hasInfantry() || hasANonInfantryGroundObject()); }
	inline bool hasAnAirUnit() { return !m_assignedAirUnits.empty(); }
	inline bool hasAnUndergroundUnit() { return !m_assignedUndergroundUnits.empty(); }
	inline bool hasANonInfantryGroundObject() { return !m_assignedNonInfantryGroundObjects.empty(); }
	inline bool hasInfantry() { return !m_assignedInfantry.empty(); }
	inline bool hasSpice() { return (fixDouble(m_spice) > 0.0); }

    ObjectClass* getAirUnit();
	ObjectClass* getDeadObject();
    ObjectClass* getNonInfantryGroundObject();
	ObjectClass* getUndergroundUnit();
	ObjectClass* getGroundObject();
	ObjectClass* getInfantry();
*/

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

    double	m_difficulty,

    //! How much spice on this particular cell is left    			
			m_spice;	
 
    /*!
     * tile assigned to current cell
     * @note need pretile because when smoothing random map, you need to know what all the tiles are before smoothed
     */
    int m_tile;
    
    int m_type;
    
    ImagePtr m_img;
/*    
    List    m_assignedAirUnits,
            m_assignedDeadObjects,
            m_assignedNonInfantryGroundObjects,
            m_assignedUndergroundUnits,
            m_assignedInfantry;
*/    
};

#endif // DUNE_TERRAINCLASS_H

