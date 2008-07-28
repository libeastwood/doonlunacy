#ifndef DUNE_STRUCTURES_REFINERYCLASS_H
#define DUNE_STRUCTURES_REFINERYCLASS_H

#include "structures/StructureClass.h"

class PlayerClass;

class RefineryClass : public StructureClass
{
public:
	RefineryClass(PlayerClass* newOwner);
	~RefineryClass();

	void doSpecificStuff();

private:

};

#endif // DUNE_STRUCTURES_REFINERYCLASS_H

