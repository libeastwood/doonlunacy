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
    
	virtual void update();
	
	void decrementUnits(std::string itemName);

	//! @name setters and getters
	//@{
	
	inline int getColour() { return m_color; }
	MapClass* getMap();
	inline int getPlayerNumber() { return m_mapPlayerNum; }
	inline int getHouse() { return m_house; }
	inline int getTeam() { return m_team; }

	int getMapPlayerNum() { return	m_mapPlayerNum; }

	inline bool isAI() { return m_ai; }
	//@}
	
	ObjectPtr createUnit(std::string itemName);
	ObjectPtr placeUnit(std::string itemName, UPoint itemPos);	
	ObjectPtr placeStructure(int builderID, UPoint builderPos, std::string itemName, UPoint itemPos);
  protected:
    //! @name Protected functions
    

    //! @name Protected attributes
    //@{

	//! True if given player is computer
	bool m_ai;

	int m_color,
	    m_house,
	    m_team,
	    m_mapPlayerNum;
    //! Name of player
    std::string m_name;
   
    //@}
};
#endif // DUNE_PLAYERCLASS_H
