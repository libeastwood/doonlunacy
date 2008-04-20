#ifndef DUNE_UNIT_LAUNCHERCLASS_H
#define DUNE_UNIT_LAUNCHERCLASS_H

#include "units/TrackedUnit.h"

class PlayerClass;

class LauncherClass : public TrackedUnit
{
public:
	LauncherClass(PlayerClass* newOwner);
	~LauncherClass();

	void blitToScreen();
	virtual bool canAttack(ObjectClass* object);

private:
	SDL_Surface*	turretGraphic;
};

#endif // DUNE_UNIT_LAUNCHERCLASS_H
