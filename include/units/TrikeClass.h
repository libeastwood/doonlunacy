#ifndef DUNE_TRIKECLASS_H
#define DUNE_TRIKECLASS_H

#include "units/GroundUnit.h"

class TrikeClass : public GroundUnit
{
public:
	TrikeClass(PlayerClass* newOwner);
	~TrikeClass();
};

#endif // DUNE_TRIKECLASS_H
