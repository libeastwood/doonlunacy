#ifndef DUNE_STRUCTURES_BARRACKSCLASS_H
#define DUNE_STRUCTURES_BARRACKSCLASS_H

#include "structures/StructureClass.h"

class PlayerClass;

class BarracksClass : public StructureClass
{
public:
	BarracksClass(PlayerClass* newOwner);
	~BarracksClass();

	void doSpecificStuff();

private:

};

#endif // DUNE_STRUCTURES_BARRACKSCLASS_H

