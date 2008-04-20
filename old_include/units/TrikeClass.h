#ifndef DUNE_UNITS_TRIKECLASS_H
#define DUNE_UNITS_TRIKECLASS_H

#include "units/GroundUnit.h"

class TrikeClass : public GroundUnit
{
public:
	TrikeClass(PlayerClass* newOwner);
	~TrikeClass();
};

#endif // DUNE_UNITS_TRIKECLASS_H
