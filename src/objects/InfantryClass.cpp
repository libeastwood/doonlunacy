#include "objects/InfantryClass.h"

#include "DataCache.h"

InfantryClass::InfantryClass(PlayerClass* newOwner, std::string unitName, uint32_t attribute) : GroundUnit(newOwner, unitName, attribute | OBJECT_INFANTRY)
{
}

