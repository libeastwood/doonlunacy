#ifndef SOLDIERCLASS_H_INCLUDED
#define SOLDIERCLASS_H_INCLUDED

#include "GroundUnit.h"

class SoldierClass : public GroundUnit
{

public:
	SoldierClass(PlayerClass* newOwner, std::string unitName);
	~SoldierClass();
};

#endif // SOLDIERCLASS_H_INCLUDED
