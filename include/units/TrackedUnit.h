#ifndef DUNE_UNITS_TRACKEDUNIT_H
#define DUNE_UNITS_TRACKEDUNIT_H

#include "units/GroundUnit.h"

class PlayerClass;

class TrackedUnit : public GroundUnit
{
public:
	TrackedUnit(PlayerClass* newOwner);
	~TrackedUnit();
	void checkPos();
	bool canPass(int xPos, int yPos);
};

#endif // DUNE_UNITS_TRACKEDUNIT_H
