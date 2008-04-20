#ifndef DUNE_STRUCTURES_GUNTURRETCLASS_H
#define DUNE_STRUCTURES_GUNTURRETCLASS_H

#include "structures/StructureClass.h"

class GunTurretClass : public StructureClass
{
public:
	GunTurretClass(PlayerClass* newOwner);
	~GunTurretClass();
	void doSpecificStuff();
	virtual void handleActionCommand(int xPos, int yPos);
	void setTurretAngle(int newAngle);
	void turn();
	void turnLeft();
	void turnRight();
	virtual void attack();

	inline int getTurretAngle() { return int(round(angle)); }

protected:
	int	attackSound,
		bulletType,
		findTargetTimer,
		primaryWeaponReloadTime;
};

#endif // DUNE_STRUCTURES_GUNTURRETCLASS_H
