#ifndef DUNE_UNITS_AIRUNIT_H
#define DUNE_UNITS_AIRUNIT_H

#include "units/UnitClass.h"

class AirUnit : public UnitClass
{
public:
	AirUnit(PlayerClass* newOwner);
	virtual ~AirUnit();

	void assignToMap(COORDTYPE* pos);
	virtual void checkPos();
	bool canPass(int xPos, int yPos);

};

#endif // DUNE_UNITS_AIRUNIT_H
