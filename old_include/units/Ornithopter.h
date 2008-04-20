#ifndef DUNE_UNITS_ORNITHOPTER_H
#define DUNE_UNITS_ORNITHOPTER_H

#include "units/AirUnit.h"

class Ornithopter : public AirUnit
{
public:
	Ornithopter(PlayerClass* newOwner);
	~Ornithopter();

	void checkPos();
	bool canAttack(ObjectClass* object);

protected:
};

#endif // DUNE_UNITS_ORNITHOPTER_H
