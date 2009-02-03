#include "objects/InfantryClass.h"

#include "DataCache.h"
#include "GCObject.h"

InfantryClass::InfantryClass(PlayerClass* newOwner, std::string unitName) : GroundUnit(newOwner, unitName)
{
	m_attributes |= OBJECT_INFANTRY;
}

InfantryClass::~InfantryClass()
{

}

