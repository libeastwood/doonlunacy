#include "objects/InfantryClass.h"

#include "DataCache.h"

InfantryClass::InfantryClass(const PlayerClass& newOwner, const std::string& unitName, uint32_t attribute) : GroundUnit(newOwner, unitName, attribute | OBJECT_INFANTRY)
{
}

