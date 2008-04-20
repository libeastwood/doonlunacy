#ifndef DUNE_STRUCTURES_LIGHTFACTORYCLASS_H
#define DUNE_STRUCTURES_LIGHTFACTORYCLASS_H

#include "structures/BuilderClass.h"

class PlayerClass;

class LightFactoryClass : public BuilderClass
{
public:
        LightFactoryClass(PlayerClass* newOwner);
	~LightFactoryClass();

	void checkSelectionList();
	void doSpecificStuff();

private:

};

#endif // DUNE_STRUCTURES_LIGHTFACTORYCLASS_H
