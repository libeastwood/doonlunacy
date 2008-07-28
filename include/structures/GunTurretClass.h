#ifndef DUNE_STRUCTURES_GUNTURRETCLASS_H
#define DUNE_STRUCTURES_GUNTURRETCLASS_H

#include "structures/StructureClass.h"

class PlayerClass;

class GunTurretClass : public StructureClass
{
public:
	GunTurretClass(PlayerClass* newOwner);
	~GunTurretClass();

	void doSpecificStuff();

private:

};

#endif // DUNE_STRUCTURES_GUNTURRETCLASS_H

