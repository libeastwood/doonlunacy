#ifndef DUNE_PLAYERCLASS_H
#define DUNE_PLAYERCLASS_H
#include <string>
#include <vector>
#include "Definitions.h"
#include "DuneConstants.h"
#include "Point.h"

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

	bool operator==(const PlayerClass& player) const noexcept { return this == &player; }
	bool operator!=(const PlayerClass& player) const noexcept { return this != &player; }

	//! @name setters and getters
	//@{
	
	inline int getColour() const noexcept { return m_color; }
	MapClass* getMap() const noexcept;
	inline int getPlayerNumber() const noexcept { return m_mapPlayerNum; }
	inline int getHouse() const noexcept { return m_house; }
	inline int getTeam() const noexcept { return m_team; }

	int getMapPlayerNum() const noexcept { return m_mapPlayerNum; }

	inline bool isAI() const noexcept{ return m_ai; }
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
