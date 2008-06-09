#include "DataCache.h"
#include "Definitions.h"
#include "Gfx.h"
#include "TerrainClass.h"

TerrainClass::TerrainClass()
{
    m_tile = Terrain_a1;
    m_type = Terrain_Sand;
    m_img = DataCache::Instance()->getObjPic(ObjPic_Terrain);
}

TerrainClass::~TerrainClass()
{

}

void TerrainClass::draw(Image * dest, SPoint pos)
{
    Rect source(m_tile*BLOCKSIZE, 0, BLOCKSIZE, BLOCKSIZE);
    m_img->blitTo(dest, source, pos);
}

ObjectClass* TerrainClass::getAirUnit()
{
    return (ObjectClass*)m_assignedAirUnits.front();
}

ObjectClass* TerrainClass::getDeadObject()
{
    return (ObjectClass*)m_assignedDeadObjects.front();
}

ObjectClass* TerrainClass::getGroundObject()
{
    if (hasANonInfantryGroundObject())
        return getNonInfantryGroundObject();
    else if (hasInfantry())
        return getInfantry();
    else
        return NULL;
}

ObjectClass* TerrainClass::getInfantry()
{
    return (ObjectClass*)m_assignedInfantry.front();
}

ObjectClass* TerrainClass::getNonInfantryGroundObject()
{
    return (ObjectClass*)m_assignedNonInfantryGroundObjects.front();
}

ObjectClass* TerrainClass::getUndergroundUnit()
{
    return (ObjectClass*)m_assignedUndergroundUnits.front();
}

void TerrainClass::assignAirUnit(ObjectClass* newObjectID) {
	m_assignedAirUnits.push_back(newObjectID);
}

void TerrainClass::assignDeadObject(ObjectClass* newObjectID) {
	m_assignedDeadObjects.push_back(newObjectID);
}

void TerrainClass::assignNonInfantryGroundObject(ObjectClass* newObjectID) {
	m_assignedNonInfantryGroundObjects.push_back(newObjectID);
}

void TerrainClass::assignUndergroundUnit(ObjectClass* newObjectID) {
	m_assignedUndergroundUnits.push_back(newObjectID);
}

