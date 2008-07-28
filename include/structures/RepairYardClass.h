#ifndef DUNE_STRUCTURES_REPAIRYARDCLASS_H
#define DUNE_STRUCTURES_REPAIRYARDCLASS_H

#include "structures/StructureClass.h"

class PlayerClass;

class RepairYardClass : public StructureClass
{
public:
	RepairYardClass(PlayerClass* newOwner);
	~RepairYardClass();

	void doSpecificStuff();

private:

};

#endif // DUNE_STRUCTURES_REPAIRYARDCLASS_H

