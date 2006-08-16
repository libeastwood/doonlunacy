#ifndef DUNE_UNITS_TANKCLASS_H
#define DUNE_UNITS_TANKCLASS_H

#include "units/TrackedUnit.h"

class TankClass : public TrackedUnit
{
public:
	TankClass(PlayerClass* newOwner);
	~TankClass();

	virtual void blitToScreen();
	void netDoConsistancy();
	void setTurretAngle(int newAngle);
	void navigate();

	inline int getTurretAngle() { return int(round(turretAngle)); }

protected:
	void engageTarget();
	void targeting();
	void turn();
	void turnTurretLeft();
	void turnTurretRight();
	
	int	drawnTurretAngle;

	double	turretTurnSpeed,
			turretAngle;

	ObjectClass *closeTarget;	//a enemy target that can be shot at while moving
	SDL_Surface	*turretGraphic;
};

#endif // DUNE_UNITS_TANKCLASS_H
