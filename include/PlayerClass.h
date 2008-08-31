#ifndef DUNE_PLAYERCLASS_H
#define DUNE_PLAYERCLASS_H
#include <string>
#include <vector>
#include "DuneConstants.h"

#include "Gfx.h"

class MapClass;

class UnitClass;
class StructureClass;

class PlayerClass 
{
  public:
    PlayerClass(int newPlayerNumber, int newHouse, int newColour, int newCredits, int team);
	virtual ~PlayerClass();
	
	void assignMapPlayerNum(int newMapPlayerNum);
	
	void* placeStructure(int builderID, UPoint builderPos, int itemID, UPoint itemPos);
	virtual void update();
	
	MapClass* getMap();
	
	UnitClass* createUnit(int itemID);
	UnitClass* placeUnit(int itemID, UPoint itemPos);
	
	//! @name Functions used to fetch information from non-public attributes
	//@{
	
	inline int getCapacity() { return m_capacity; }
	inline int getColour() { return m_colour; }
	inline int getPlayerNumber() { return m_mapPlayerNum; }
	inline int getPower() { return m_power; }
	inline int getPowerRequirement() { return m_powerRequirement; }
	inline int getHouse() { return m_house; }
	inline int getTeam() { return m_team; }

	inline bool hasBarracks() { return (m_numBarracks > 0); }
	inline bool hasCarryalls() { return m_numCarryalls > 0; }
	inline bool hasIX() { return (m_numIXs > 0); }
	inline bool hasLightFactory() { return (m_numLightFactories > 0); }
	inline bool hasHeavyFactory() { return (m_numHeavyFactories > 0); }
	inline bool hasPower() { return (m_power >= m_powerRequirement); }
	inline bool hasRadar() { return (m_numRadars > 0); }
	inline bool hasRadarOn() { return (hasRadar() && hasPower()); }
	inline bool hasRefinery() { return (m_numRefineries > 0); }
	inline bool hasRepairYard() { return (m_numRepairYards > 0); }
	inline bool hasStarPort() { return (m_numStarPorts > 0); }
	inline bool hasWindTrap() { return (m_numWindTraps > 0); }
	int getMapPlayerNum() { return	m_mapPlayerNum; }
	inline bool isAI() { return m_ai; }
	
	//@}
  protected:
    //! @name Protected functions
    
    //@{
    //! Increment player's storage capacity by a certain value e.g. 1000 when building Silo
  	inline void addCapacity(int newCapacity) { m_capacity += newCapacity; }
    //@}

    //! @name Protected attributes
    //@{

	//! True if given player is computer
	bool m_ai;

    //! Capacity of player's refineries and silos
    int m_capacity,
    	m_credits,
    	m_oldCredits,
    
    //! Colour assigned to current player see HOUSETYPE
        m_colour,

    //! ATM I've no idea what these do. Probably just an identification number
        m_house,      
        
    //! Number of particulat structures a player has.
		m_numBarracks,
		m_numCarryalls,
		m_numFrigates,
		m_numHarvesters,
		m_numUnits,
		m_numIXs,
		m_numHeavyFactories,
		m_numHighTechFactories,
		m_numLightFactories,
		m_numPalaces,
		m_numRefineries,
		m_numRepairYards,
		m_numRocketTurrets,
		m_numRadars,
		m_numStarPorts,
		m_numWalls,
		m_numWindTraps,
		m_numWORs,
        
    //! Total number of structures
        m_numStructures,
          
        m_mapPlayerNum,

    //! Power currently produced in player's windtraps
        m_power,

    //! Power required by player's structures
        m_powerRequirement,
    
    //! Team to which a given player belongs.
        m_team;
    
    //! Name of player
    std::string m_name;
   
    //@}
};
#endif // DUNE_PLAYERCLASS_H
