#ifndef DUNE_STRUCTURES_PALACECLASS_H
#define DUNE_STRUCTURES_PALACECLASS_H

#include "structures/StructureClass.h"

class PlayerClass;

class PalaceClass : public StructureClass
{
public:
	PalaceClass(PlayerClass* newOwner);
	~PalaceClass();

	void doSpecificStuff();

private:

};

#endif // DUNE_STRUCTURES_PALACECLASS_H

