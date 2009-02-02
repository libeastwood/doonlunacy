#ifndef DUNE_UNITS_AIRUNIT_H
#define DUNE_UNITS_AIRUNIT_H

#include "units/UnitClass.h"

class AirUnit : public UnitClass
{
public:
	AirUnit(PlayerClass* newOwner, std::string unitName);
	virtual ~AirUnit();

	void checkPos();

};

#endif // DUNE_UNITS_AIRUNIT_H
