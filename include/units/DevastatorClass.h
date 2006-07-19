#ifndef DUNE_UNITS_DEVASTATORCLASS_H
#define DUNE_UNITS_DEVASTATORCLASS_H

#include "units/TrackedUnit.h"

class PlayerClass;

class DevastatorClass : public TrackedUnit
{
public:
	DevastatorClass(PlayerClass* newOwner);
	~DevastatorClass();

	void blitToScreen();
	void devastate();
	void startDevastate();
	void netDevastate();
	void update();

private:
	bool			devastated;
	int				devastateTimer;
	SDL_Surface*	turretGraphic,
					*explosion[3][3];
};

#endif // DUNE_UNITS_DEVASTATORCLASS_H
