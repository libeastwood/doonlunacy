#ifndef DUNE_UNITS_MCVCLASS_H
#define DUNE_UNITS_MCVCLASS_H

#include "units/GroundUnit.h"

class MCVClass : public GroundUnit
{
public:
	MCVClass(PlayerClass* newOwner);
	~MCVClass();

	void deploy();
	void netDeploy();
};

#endif // DUNE_UNITS_MCVCLASS_H
