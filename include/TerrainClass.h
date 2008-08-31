#ifndef DUNE_TERRAINCLASS_H
#define DUNE_TERRAINCLASS_H

#include "DataCache.h"
#include "Gfx.h"
#include "mmath.h"
#include "ObjectClass.h"

#include <list>

#define DAMAGEPERCELL 5
#define FOGTIMEOUT 10

typedef std::list <Uint32> List;

typedef struct
{
	Uint32 damageType;
	int	tile;
	UPoint realPos;
} DAMAGETYPE;


class TerrainClass  : public UPoint
{
  public:
    TerrainClass();
    ~TerrainClass();

    void draw(Image * dest, SPoint pos);

    void assignAirUnit(Uint32 newObjectID);
    void assignDeadObject(Uint32 newObjectID);
    void assignNonInfantryGroundObject(Uint32 newObjectID);
    void assignUndergroundUnit(Uint32 newObjectID);

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
	ObjectClass* getObjectWithID(Uint32 objectID);
	
	bool hasAnObject();
	
	//!@{
	//! @name setters and getters
    inline bool hasADeadObject() { return !m_assignedDeadObjects.empty(); }
    inline bool hasAGroundObject() { return (hasInfantry() || hasANonInfantryGroundObject()); }
    inline bool hasAnAirUnit() { return !m_assignedAirUnits.empty(); }
    inline bool hasAnUndergroundUnit() { return !m_assignedUndergroundUnits.empty(); }
    inline bool hasANonInfantryGroundObject() { return !m_assignedNonInfantryGroundObjects.empty(); }
    inline bool hasInfantry() { return !m_assignedInfantry.empty(); }
    inline bool hasSpice() { return (fixDouble(m_spice) > 0.0); }

    inline int getTile() { return m_tile; }
    inline void setTile(int newTile) { m_tile = newTile; }

    inline int getType() { return m_type; }
    inline void setType(int newType) { m_type = newType; }

    bool isMountain() { return  (m_type == Terrain_Mountain); }

    inline bool isRock() { return ((m_type == Terrain_Rock) || (m_type == Structure_Slab1) 
                           || (m_type == Terrain_Mountain)); }
    inline bool isSand() { return ((m_type == Terrain_Dunes) || (m_type == Terrain_Sand)); }
    inline bool isBloom() { return ((m_type == Terrain_Sand) && ((m_tile == Terrain_a2) 
                            || (m_tile == Terrain_a3))); }
    inline bool isSpice() { return ((m_type == Terrain_Spice) || (m_type == Terrain_ThickSpice)); }
    inline bool isThickSpice() { return (m_type == Terrain_ThickSpice); }

	inline bool isNextToHidden() { return (m_hideTile != Terrain_HiddenFull); }
	inline bool isNextToFogged() { return (m_fogTile != Terrain_HiddenFull); }

	inline int getSandRegion() { return m_sandRegion; }
	inline int getDamageType() { return m_damageType; }
	inline void setSandRegion(int newSandRegion) { m_sandRegion = newSandRegion; }

    double getDifficulty() { return m_difficulty; }

	inline bool isExplored(int player) 	{return m_explored[player];}
	inline void setExplored(int player, bool truth) { //if(truth)
		//m_lastAccess[player] = clock();
		m_explored[player] = truth; 
	}

	bool isFogged(int player);
	inline void setOwner(int newOwner) { m_owner = newOwner; }
	inline int getOwner() { return m_owner; }
	
	inline void setHideTile(int newTile) { m_hideTile = newTile; }
	
	inline void setFogTile(int newTile) { m_fogTile = newTile; }
    //@}
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

    //! This variable is used in A* search algorithm for path finding.
    TerrainClass*  m_parent;
    //@}

	void clearDamage();
	/*!
	 *  Inflict damage to all objects assigned to given cell and change
	 *  tile to a damaged one.
	 *  @param damager pointer to object that causes damage
	 *  @param damagerOwner pointer to owner of the damager
	 *  @param realPos bullets always have realPos as they are not assigned to any cells
	 *  @param bulletType can be bullet, rocket, deviator rocket, large rocket, etc.
	 *  @param bulletDamage how deadly a given bullet is
	 *  @param damagePiercing additional damage caused by byllet
	 *  @param damageRadius bullets affect only one cell, rockets can have bigger radius of destruction
	 *  @param air the bullet is aimed at an air unit
	 */
	void damageCell(ObjectClass* damager, PlayerClass* damagerOwner, UPoint realPos, int bulletType, int bulletDamage, int damagePiercing, int damageRadius, bool air);

  private:
  
  	bool	*m_explored;
			
	clock_t	*m_lastAccess;
	
    //! How difficult it is to move in this cell
    float m_difficulty,

    //! How much spice on this particular cell is left
        m_spice;

	int	m_hideTile,
		m_fogTile,
		//! Remember last colour(radar)
		m_fogColour,
		m_owner,
		m_sandRegion,
		m_type;
    /*!
     * tile assigned to current cell
     * @note need pretile because when smoothing random map, you need to know what all the tiles are before smoothed
     */
        int m_tile;		

    ImagePtr m_img;
    ImagePtr m_hiddenImg;

    List  m_assignedAirUnits,
          m_assignedDeadObjects,
          m_assignedNonInfantryGroundObjects,
          m_assignedUndergroundUnits,
          m_assignedInfantry;

    //! damage positions
	DAMAGETYPE	m_damage[DAMAGEPERCELL];
	Uint32 m_damageType;
	int	m_damagePos;
};

#endif // DUNE_TERRAINCLASS_H

