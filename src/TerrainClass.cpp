#include "DataCache.h"
#include "Definitions.h"
#include "GameState.h"
#include "Log.h"
#include "Gfx.h"
#include "TerrainClass.h"

using namespace std;

TerrainClass::TerrainClass() : UPoint(0,0)
{
    m_difficulty = 1;
    m_spice = 0;
    m_tile = Terrain_a1;
    m_type = Terrain_Sand;
    m_img = DataCache::Instance()->getObjPic(ObjPic_Terrain);
    m_hiddenImg = DataCache::Instance()->getObjPic(ObjPic_Terrain_Hidden);
    m_visited = false;

    m_explored = new bool[MAX_PLAYERS];
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
    	m_explored[i] = false;
	}

}

TerrainClass::~TerrainClass()
{

}

void TerrainClass::draw(Image * dest, SPoint pos)
{
    Rect source(m_tile*BLOCKSIZE, 0, BLOCKSIZE, BLOCKSIZE);

	if(isExplored(GameState::Instance()->LocalPlayer()->getPlayerNumber()))
	{
        m_img->blitTo(dest, source, pos);
    }
    else
    {
        m_img->blitTo(dest, source, pos);
        m_hiddenImg->blitTo(dest, source, pos);
    }

}

ObjectClass* TerrainClass::getAirUnit()
{
    return (ObjectClass*)(*m_assignedAirUnits.begin()).second;
}

ObjectClass* TerrainClass::getDeadObject()
{
    return (ObjectClass*)(*m_assignedDeadObjects.begin()).second;
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
    return (ObjectClass*)(*m_assignedInfantry.begin()).second;
}

ObjectClass* TerrainClass::getNonInfantryGroundObject()
{
    return (ObjectClass*)(*m_assignedNonInfantryGroundObjects.begin()).second;
}

ObjectClass* TerrainClass::getUndergroundUnit()
{
    return (ObjectClass*)(*m_assignedUndergroundUnits.begin()).second;
}

void TerrainClass::assignAirUnit(ObjectClass* object) {
	m_assignedAirUnits.insert(make_pair(object->getObjectID(), object));
}

void TerrainClass::assignDeadObject(ObjectClass* object) {
	m_assignedDeadObjects.insert(make_pair(object->getObjectID(), object));
}

void TerrainClass::assignNonInfantryGroundObject(ObjectClass* object) {
	m_assignedNonInfantryGroundObjects.insert(make_pair(object->getObjectID(), object));
}

void TerrainClass::assignUndergroundUnit(ObjectClass* object) {
	m_assignedUndergroundUnits.insert(make_pair(object->getObjectID(), object));
}

ObjectClass* TerrainClass::getObject() {
	ObjectClass* temp = NULL;
	if (hasAnAirUnit())
		temp = getAirUnit();
	else if (hasANonInfantryGroundObject())
		temp = getNonInfantryGroundObject();
	else if (hasInfantry())
		temp = getInfantry();
	else if (hasAnUndergroundUnit())
		temp = getUndergroundUnit();
	return temp;
}


ObjectClass* TerrainClass::getObjectAt(UPoint pos) {
	ObjectClass* temp = NULL;

	if (hasAnAirUnit())
		temp = getAirUnit();
	else if (hasANonInfantryGroundObject())
		temp = getNonInfantryGroundObject();
	else if (hasInfantry())	
	{
/*		double closestDistance = NONE;
		COORDTYPE atPos, centrePoint;
		InfantryClass* infantry;
		atPos.x = x;
		atPos.y = y;
				
		std::list<Uint32>::const_iterator iter;
		for(iter = assignedInfantryList.begin(); iter != assignedInfantryList.end() ;iter++) {
			infantry = (InfantryClass*) currentGame->getObjectTree().getObject(*iter);
			if(infantry == NULL)
				continue;	
			
			infantry->getCentrePoint(&centrePoint);
			if ((closestDistance == NONE) || (distance_from(&atPos, &centrePoint) < closestDistance)) {
				closestDistance = distance_from(&atPos, &centrePoint);
				temp = infantry;
			}
		}
		*/
	}
	else if (hasAnUndergroundUnit())
		temp = getUndergroundUnit();

	return temp;
}

void TerrainClass::unassignAirUnit(Uint32 ObjectID) 
{
    List::iterator iter = m_assignedAirUnits.find(ObjectID);
    if( iter != m_assignedAirUnits.end() ) 
    {
	    m_assignedAirUnits.erase(iter);
        LOG_INFO("TerrainClass", "Unassigned air unit.");
	}
}

void TerrainClass::unassignDeadObject(Uint32 ObjectID) 
{
    List::iterator iter = m_assignedDeadObjects.find(ObjectID);
    if( iter != m_assignedDeadObjects.end() ) 
    {
	    LOG_INFO("TerrainClass", "Unassigned dead object.");
	    m_assignedDeadObjects.erase(iter);
	}
    
}

void TerrainClass::unassignNonInfantryGroundObject(Uint32 ObjectID) 
{
    List::iterator iter = m_assignedNonInfantryGroundObjects.find(ObjectID);
    if( iter != m_assignedNonInfantryGroundObjects.end() ) 
    {
        LOG_INFO("TerrainClass", "Unassigned non-infantry ground object.");
	    m_assignedNonInfantryGroundObjects.erase(iter);
	}
}

void TerrainClass::unassignUndergroundUnit(Uint32 ObjectID) 
{
    List::iterator iter = m_assignedUndergroundUnits.find(ObjectID);
    if( iter != m_assignedUndergroundUnits.end() ) 
    {
        LOG_INFO("TerrainClass", "Unassigned underground object.");
    	m_assignedUndergroundUnits.erase(iter);
	}
}

void TerrainClass::unassignInfantry(Uint32 ObjectID, int currentPosition) 
{
    List::iterator iter = m_assignedInfantry.find(ObjectID);
    if( iter != m_assignedInfantry.end() ) 
    {
        LOG_INFO("TerrainClass", "Unassigned infantry.");
    	m_assignedInfantry.erase(iter);
	}
}

void TerrainClass::unassignObject(Uint32 ObjectID) {
    LOG_INFO("TerrainClass", "Unassigning object with ID: %d", ObjectID);
	unassignInfantry(ObjectID,-1);
	unassignUndergroundUnit(ObjectID);
	unassignNonInfantryGroundObject(ObjectID);
	unassignAirUnit(ObjectID);
}



bool TerrainClass::isFogged(int player)
{
#if 0 

#ifdef	_DEBUG
		if(debug) 
		return false;
#endif

/*	if(!fog_wanted)
		return false;
	else
*/
#define FOGTIMEOUT 10
	if((int)((clock() - m_lastAccess[player])/CLOCKS_PER_SEC) >= FOGTIMEOUT)
		return true;
	else 
	return false; 
	
#endif	
}
