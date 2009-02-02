#include "units/AirUnit.h"
#include "structures/RepairYardClass.h"
#include "PlayerClass.h"

AirUnit::AirUnit(PlayerClass *newOwner, std::string unitName) : UnitClass(newOwner, unitName)
{
	m_attributes |= OBJECT_AIRUNIT;

	m_deathFrame = "Object_AirUnitExplosion";
	m_numDeathFrames = 5;
}

AirUnit::~AirUnit()
{
}

/*
void AirUnit::checkPos()
{
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
}
*/
