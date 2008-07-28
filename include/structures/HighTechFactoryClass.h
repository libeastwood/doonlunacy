#ifndef DUNE_STRUCTURES_HIGHTECHFACTORYCLASS_H
#define DUNE_STRUCTURES_HIGHTECHFACTORYCLASS_H

#include "structures/StructureClass.h"

class PlayerClass;

class HighTechFactoryClass : public StructureClass
{
public:
	HighTechFactoryClass(PlayerClass* newOwner);
	~HighTechFactoryClass();

	void doSpecificStuff();

private:

};

#endif // DUNE_STRUCTURES_HIGHTECHFACTORYCLASS_H

