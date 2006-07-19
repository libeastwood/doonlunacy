#ifndef DUNE_UNITS_SONICTANKCLASS_H
#define DUNE_UNITS_SONICTANKCLASS_H

#include "units/TrackedUnit.h"

class SonicTankClass : public TrackedUnit
{
public:
	SonicTankClass(PlayerClass* newOwner);
	~SonicTankClass();

	void blitToScreen();
	void handleDamage(int damage, ObjectClass *damager);

	bool canAttack(ObjectClass* object);

private:
	SDL_Surface*	turretGraphic;
};

#endif // DUNE_UNITS_SONICTANKCLASS_H
