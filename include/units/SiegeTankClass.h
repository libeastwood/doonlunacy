#ifndef DUNE_UNITS_SIEGETANKCLASS_H
#define DUNE_UNITS_SIEGETANKCLASS_H

#include "units/TankClass.h"

class SiegeTankClass : public TankClass
{
public:
	SiegeTankClass(PlayerClass* newOwner);
	~SiegeTankClass();

	void blitToScreen();
};

#endif // DUNE_UNITS_SIEGETANKCLASS_H
