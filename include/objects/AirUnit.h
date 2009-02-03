#ifndef DUNE_OBJECTS_AIRUNIT_H
#define DUNE_OBJECTS_AIRUNIT_H

#include "objects/UnitClass.h"

class AirUnit : public UnitClass
{
public:
	AirUnit(PlayerClass* newOwner, std::string unitName);
	virtual ~AirUnit();

	void checkPos();

};

#endif // DUNE_OBJECTS_AIRUNIT_H
