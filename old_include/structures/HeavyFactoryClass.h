#ifndef DUNE_STRUCTURES_HEAVYFACTORYCLASS_H
#define DUNE_STRUCTURES_HEAVYFACTORYCLASS_H

#include "structures/BuilderClass.h"

class HeavyFactoryClass : public BuilderClass
{
public:
        HeavyFactoryClass(PlayerClass* newOwner);
	~HeavyFactoryClass();

	void buildRandom();
	void checkSelectionList();
	void doSpecificStuff();

private:

};

#endif // DUNE_STRUCTURES_HEAVYFACTORYCLASS_H
