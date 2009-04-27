#ifndef DUNE_OBJECTS_INFANTRYCLASS_H
#define DUNE_OBJECTS_INFANTRYCLASS_H

#include "GroundUnit.h"

class InfantryClass : public GroundUnit
{

public:
	InfantryClass(PlayerClass* newOwner, std::string unitName, uint32_t attribute = 0);
};

#endif // DUNE_OBJECTS_INFANTRYCLASS_H
