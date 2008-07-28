#ifndef DUNE_STRUCTURES_STARPORTCLASS_H
#define DUNE_STRUCTURES_STARPORTCLASS_H

#include "structures/StructureClass.h"

class PlayerClass;

class StarPortClass : public StructureClass
{
public:
	StarPortClass(PlayerClass* newOwner);
	~StarPortClass();

	void doSpecificStuff();

private:

};

#endif // DUNE_STRUCTURES_STARPORTCLASS_H

