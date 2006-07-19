#ifndef DUNE_STRUCTURES_HIGHTECHFACTORYCLASS_H
#define DUNE_STRUCTURES_HIGHTECHFACTORYCLASS_H

#include "structures/BuilderClass.h"

class PlayerClass;

class HighTechFactoryClass : public BuilderClass
{
public:
        HighTechFactoryClass(PlayerClass* newOwner);
	~HighTechFactoryClass();

	void checkSelectionList();
	void doSpecificStuff();

private:

};

#endif // DUNE_STRUCTURES_HIGHTECHFACTORYCLASS_H
