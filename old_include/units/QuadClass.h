#ifndef DUNE_UNITS_QUADCLASS_H
#define DUNE_UNITS_QUADCLASS_H

#include "units/GroundUnit.h"

class QuadClass : public GroundUnit
{
public:
	QuadClass(PlayerClass* newOwner);
	~QuadClass();
};

#endif // DUNE_UNITS_QUADCLASS_H
