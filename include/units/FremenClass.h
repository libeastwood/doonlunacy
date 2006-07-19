#ifndef DUNE_UNITS_FREMENCLASS_H
#define DUNE_UNITS_FREMENCLASS_H

#include "units/TrooperClass.h"
class PlayerClass;

class FremenClass : public TrooperClass
{
public:
	FremenClass(PlayerClass* newOwner);
	~FremenClass();

protected:
};

#endif // DUNE_UNITS_FREMENCLASS_H
