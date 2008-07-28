#ifndef DUNE_STRUCTURES_WORCLASS_H
#define DUNE_STRUCTURES_WORCLASS_H

#include "structures/StructureClass.h"

class PlayerClass;

class WORClass : public StructureClass
{
public:
	WORClass(PlayerClass* newOwner);
	~WORClass();

	void doSpecificStuff();

private:

};

#endif // DUNE_STRUCTURES_WORCLASS_H

