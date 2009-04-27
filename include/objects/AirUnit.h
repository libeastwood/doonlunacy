#ifndef DUNE_OBJECTS_AIRUNIT_H
#define DUNE_OBJECTS_AIRUNIT_H

#include "objects/UnitClass.h"

class AirUnit : public UnitClass
{
public:
	AirUnit(PlayerClass* newOwner, std::string unitName, uint32_t attribute = 0);

	void checkPos();

};

#endif // DUNE_OBJECTS_AIRUNIT_H
