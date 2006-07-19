#ifndef DUNE_STRUCTURES_ROCKETTURRRETCLASS_H
#define DUNE_STRUCTURES_ROCKETTURRRETCLASS_H

#include "structures/GunTurretClass.h"

class ObjectClass;
class PlayerClass;

class RocketTurretClass : public GunTurretClass
{
public:
	RocketTurretClass(PlayerClass* newOwner);
	~RocketTurretClass();

	void doSpecificStuff();
	bool canAttack(ObjectClass* object);

private:
};

#endif // DUNE_STRUCTURES_ROCKETTURRRETCLASS_H
