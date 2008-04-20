#ifndef DUNE_UNITS_DEVIATORCLASS_H
#define DUNE_UNITS_DEVIATORCLASS_H

class PlayerClass;
class ObjectClass;

#include "units/LauncherClass.h"

class DeviatorClass : public LauncherClass
{
public:
	DeviatorClass(PlayerClass* newOwner);
	~DeviatorClass();

	bool canAttack(ObjectClass* object);
};

#endif // DUNE_UNITS_DEVIATORCLASS_H
