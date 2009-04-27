#include "objects/AirUnit.h"
#include "PlayerClass.h"

AirUnit::AirUnit(PlayerClass *newOwner, std::string unitName, uint32_t attribute) : UnitClass(newOwner, unitName, attribute | OBJECT_AIRUNIT)
{
}

void AirUnit::checkPos()
{
	/*
	if (goingToRepairYard)
	{
                COORDTYPE cp = target->getClosestPoint(&location);
		if ((blockDistance(&location, &cp) <= 1.5) && ((RepairYardClass*)target)->isFree())
		{
			if (health*100/maxHealth < 100)
				setGettingRepaired();
			else
			{
				setTarget(NULL);
				setDestination(&guardPoint);
			}
		}
	}
	*/
}
