#ifndef DUNE_STRUCTURES_LIGHTFACTORYCLASS_H
#define DUNE_STRUCTURES_LIGHTFACTORYCLASS_H

#include "structures/StructureClass.h"

class PlayerClass;

class LightFactoryClass : public StructureClass
{
public:
	LightFactoryClass(PlayerClass* newOwner);
	~LightFactoryClass();

	void doSpecificStuff();

private:

};

#endif // DUNE_STRUCTURES_LIGHTFACTORYCLASS_H

