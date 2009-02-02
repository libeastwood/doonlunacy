#ifndef INFANTRYCLASS_H_INCLUDED
#define INFANTRYCLASS_H_INCLUDED

#include "GroundUnit.h"

class InfantryClass : public GroundUnit
{

public:
	InfantryClass(PlayerClass* newOwner, std::string unitName);
	~InfantryClass();
};

#endif // INFANTRYCLASS_H_INCLUDED
