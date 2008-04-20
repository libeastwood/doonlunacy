#ifndef DUNE_STRUCTURES_SILOCLASS_H
#define DUNE_STRUCTURES_SILOCLASS_H

#include "structures/StructureClass.h"

class PlayerClass;

class SiloClass : public StructureClass
{
public:
	SiloClass(PlayerClass* newOwner);
	~SiloClass();

	void doSpecificStuff();

private:

};

#endif // DUNE_STRUCTURES_SILOCLASS_H
