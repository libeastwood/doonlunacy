#ifndef DUNE_UNITS_RADERCLASS_H
#define DUNE_UNITS_RADERCLASS_H

#include "units/GroundUnit.h"

class RaiderClass : public GroundUnit
{
public:
	RaiderClass(PlayerClass* newOwner);
	~RaiderClass();
};

#endif // DUNE_UNITS_RADERCLASS_H
