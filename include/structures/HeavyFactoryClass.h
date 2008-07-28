#ifndef DUNE_STRUCTURES_HEAVYFACTORYCLASS_H
#define DUNE_STRUCTURES_HEAVYFACTORYCLASS_H

#include "structures/StructureClass.h"

class PlayerClass;

class HeavyFactoryClass : public StructureClass
{
public:
	HeavyFactoryClass(PlayerClass* newOwner);
	~HeavyFactoryClass();

	void doSpecificStuff();

private:

};

#endif // DUNE_STRUCTURES_HEAVYFACTORYCLASS_H

