#ifndef DUNE_UNITS_TROOPERCLASS_H
#define DUNE_UNITS_TROOPERCLASS_H

#include "units/InfantryClass.h"

class TrooperClass : public InfantryClass
{
public:
	TrooperClass(PlayerClass* newOwner);
	~TrooperClass();

	bool canAttack(ObjectClass* object);

protected:
};

#endif // DUNE_UNITS_TROOPERCLASS_H
