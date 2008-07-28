#ifndef DUNE_STRUCTURES_ROCKETTURRETCLASS_H
#define DUNE_STRUCTURES_ROCKETTURRETCLASS_H

#include "structures/StructureClass.h"

class PlayerClass;

class RocketTurretClass : public StructureClass
{
public:
	RocketTurretClass(PlayerClass* newOwner);
	~RocketTurretClass();

	void doSpecificStuff();

private:

};

#endif // DUNE_STRUCTURES_ROCKETTURRETCLASS_H

