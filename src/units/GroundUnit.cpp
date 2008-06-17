#include "units/GroundUnit.h"

GroundUnit::GroundUnit(PlayerClass* newOwner) : UnitClass(newOwner)
{
	m_groundUnit = true;
}

GroundUnit::~GroundUnit()
{

}
