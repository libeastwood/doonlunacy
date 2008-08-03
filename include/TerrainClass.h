#ifndef DUNE_TERRAINCLASS_H
#define DUNE_TERRAINCLASS_H

#include "DataCache.h"
#include "Gfx.h"
#include "mmath.h"
#include "ObjectClass.h"

typedef std::map <Uint32, ObjectClass*> List;

class TerrainClass  : public UPoint
{
  public:
    TerrainClass();
    ~TerrainClass();

    void draw(Image * dest, SPoint pos);

    void assignAirUnit(ObjectClass* newObjectID);
    void assignDeadObject(ObjectClass* newObjectID);
    void assignNonInfantryGroundObject(ObjectClass* newObjectID);
    void assignUndergroundUnit(ObjectClass* newObjectID);

    void unassignAirUnit(Uint32 ObjectID);
    void unassignDeadObject(Uint32 ObjectID);
    void unassignNonInfantryGroundObject(Uint32 ObjectID);
    void unassignObject(Uint32 ObjectID);
    void unassignInfantry(Uint32 ObjectID, int currentPosition);
    void unassignUndergroundUnit(Uint32 ObjectID);

    /*!
     * returns a pointer to an air unit in current cell (if there's one)
     * @return ObjectClass* pointer to air unit
     */
    ObjectClass* getAirUnit();

    /*!
     * returns a pointer to a dead object in current cell (if there's one)
     * @return ObjectClass*  pointer to dead object
     */
    ObjectClass* getDeadObject();

    /*!
     * returns a pointer to a non infantry ground object in current cell (if there's one)
     * @return ObjectClass*  pointer to non infantry ground object
     */
    ObjectClass* getNonInfantryGroundObject();
    
    /*!
     * returns a pointer to an underground object in current cell (if there's one)
     * @return ObjectClass*  pointer to underground object(sandworm?)
     */
    ObjectClass* getUndergroundUnit();

    /*!
     * returns a pointer to an ground object in current cell (if there's one)
     * @return ObjectClass*  pointer to ground object
     */
    ObjectClass* getGroundObject();

    /*!
     * returns a pointer to infantry object in current cell (if there's one)
     * @return ObjectClass*  pointer to infantry object
    */
    ObjectClass* getInfantry();
    //ObjectClass* getInfantry(int i);
    ObjectClass* getObject();
    ObjectClass* getObjectAt(UPoint pos);

    inline bool hasADeadObject()
    {
        return !m_assignedDeadObjects.empty();
    }

    inline bool hasAGroundObject()
    {
        return (hasInfantry() || hasANonInfantryGroundObject());
    }

    inline bool hasAnAirUnit()
    {
        return !m_assignedAirUnits.empty();
    }

    inline bool hasAnUndergroundUnit()
    {
        return !m_assignedUndergroundUnits.empty();
    }

    inline bool hasANonInfantryGroundObject()
    {
        return !m_assignedNonInfantryGroundObjects.empty();
    }

    inline bool hasInfantry()
    {
        return !m_assignedInfantry.empty();
    }

    inline bool hasSpice()
    {
        return (fixDouble(m_spice) > 0.0);
    }

    inline int getType()
    {
        return m_type;
    }

    inline int getTile()
    {
        return m_tile;
    }

    inline bool isMountain()
    {
        return (m_type == Terrain_Mountain);
    }

    inline bool isRock()
    {
        return ((m_type == Terrain_Rock) || (m_type == Structure_Slab1) || (m_type == Terrain_Mountain));
    }

    inline bool isSand()
    {
        return ((m_type == Terrain_Dunes) || (m_type == Terrain_Sand));
    }

    inline bool isBloom()
    {
        return ((m_type == Terrain_Sand) && ((m_tile == Terrain_a2) || (m_tile == Terrain_a3)));
    }

    inline bool isSpice()
    {
        return ((m_type == Terrain_Spice) || (m_type == Terrain_ThickSpice));
    }

    inline bool isThickSpice()
    {
        return (m_type == Terrain_ThickSpice);
    }

    inline void setTile(int newTile)
    {
        m_tile = newTile;
    }

    inline void setType(int newType)
    {
        m_type = newType;
    }

    float getDifficulty()
    {
        return m_difficulty;
    }



    //! @name Path searching variables
    //@{ 
    //! True if A* has already checked the node
    bool m_visited;
    
    float //! cost to get here from original location
           m_cost,       
           //! cost + heuristic
           m_f,          
           //! estimate for how much it will cost to get from here to dest
           m_heuristic;  

    //FIXME:What's this for?
    TerrainClass*  m_parent;
    //@}

  private:
    //! How difficult it is to move in this cell
    float m_difficulty,

    //! How much spice on this particular cell is left
    m_spice;

    /*!
     * tile assigned to current cell
     * @note need pretile because when smoothing random map, you need to know what all the tiles are before smoothed
     */
    int m_tile;

    int m_type;

    ImagePtr m_img;

    List  m_assignedAirUnits,
          m_assignedDeadObjects,
          m_assignedNonInfantryGroundObjects,
          m_assignedUndergroundUnits,
          m_assignedInfantry;


};

#endif // DUNE_TERRAINCLASS_H

