#ifndef DUNE_STRUCTURES_IXCLASS_H
#define DUNE_STRUCTURES_IXCLASS_H

#include "structures/StructureClass.h"

class PlayerClass;

class IXClass : public StructureClass
{
public:
	IXClass(PlayerClass* newOwner);
	~IXClass();

	void doSpecificStuff();

private:

};

#endif // DUNE_STRUCTURES_IXCLASS_H

