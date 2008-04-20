#ifndef DUNE_STRUCTURES_CONSTRUCTIONYARDCLASS_H
#define DUNE_STRUCTURES_CONSTRUCTIONYARDCLASS_H

#include "structures/BuilderClass.h"

class ConstructionYardClass : public BuilderClass
{
public:
        ConstructionYardClass(PlayerClass* newOwner);
        ~ConstructionYardClass();

	void checkSelectionList();
	void doSpecificStuff();
};

#endif // DUNE_STRUCTURES_CONSTRUCTIONYARDCLASS_H
