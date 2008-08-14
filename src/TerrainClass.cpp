#include "ConcatIterator.h"
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
    m_hideTile = Terrain_HiddenFull;
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
	if(isExplored(GameState::Instance()->LocalPlayer()->getPlayerNumber()))
	{
        Rect source(m_tile*BLOCKSIZE, 0, BLOCKSIZE, BLOCKSIZE);
        m_img->blitTo(dest, source, pos);
    }
    else
    {
        if (m_hideTile != Terrain_HiddenFull)
        {
            Rect source(m_tile*BLOCKSIZE, 0, BLOCKSIZE, BLOCKSIZE);
            m_img->blitTo(dest, source, pos);
            Rect sourceHidden(m_hideTile*BLOCKSIZE, 0, BLOCKSIZE, BLOCKSIZE);
            m_hiddenImg->blitTo(dest, sourceHidden, pos);

        }
    }

}

ObjectClass* TerrainClass::getAirUnit()
{
	GameState* gs = GameState::Instance();
    return gs->GetObjectTree()->getObject(m_assignedAirUnits.front());
}

ObjectClass* TerrainClass::getDeadObject()
{
	GameState* gs = GameState::Instance();
    return gs->GetObjectTree()->getObject(m_assignedDeadObjects.front());
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
	GameState* gs = GameState::Instance();
    return gs->GetObjectTree()->getObject(m_assignedInfantry.front());
}

ObjectClass* TerrainClass::getNonInfantryGroundObject()
{
	GameState* gs = GameState::Instance();
    return gs->GetObjectTree()->getObject(m_assignedNonInfantryGroundObjects.front());
}

ObjectClass* TerrainClass::getUndergroundUnit()
{
	GameState* gs = GameState::Instance();
    return gs->GetObjectTree()->getObject(m_assignedUndergroundUnits.front());
}

void TerrainClass::assignAirUnit(Uint32 newObjectID) 
{
	m_assignedAirUnits.push_back(newObjectID);
}

void TerrainClass::assignDeadObject(Uint32 newObjectID) 
{
	m_assignedDeadObjects.push_back(newObjectID);
}

void TerrainClass::assignNonInfantryGroundObject(Uint32 newObjectID) 
{
	m_assignedNonInfantryGroundObjects.push_back(newObjectID);
}

void TerrainClass::assignUndergroundUnit(Uint32 newObjectID) 
{
	m_assignedUndergroundUnits.push_back(newObjectID);
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
/*		float closestDistance = NONE;
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

bool TerrainClass::hasAnObject() {
	return (hasAGroundObject() || hasAnAirUnit() || hasInfantry() || hasAnUndergroundUnit());
}

void TerrainClass::unassignAirUnit(Uint32 ObjectID) 
{
	m_assignedAirUnits.remove(ObjectID);
	LOG_INFO("TerrainClass", "Unassigned air unit.");
}

void TerrainClass::unassignDeadObject(Uint32 ObjectID) 
{
	m_assignedDeadObjects.remove(ObjectID); 
    LOG_INFO("TerrainClass", "Unassigned dead object.");
}

void TerrainClass::unassignNonInfantryGroundObject(Uint32 ObjectID) 
{
	m_assignedNonInfantryGroundObjects.remove(ObjectID);
    LOG_INFO("TerrainClass", "Unassigned non-infantry ground object.");
}

void TerrainClass::unassignUndergroundUnit(Uint32 ObjectID) 
{
	m_assignedUndergroundUnits.remove(ObjectID);
    LOG_INFO("TerrainClass", "Unassigned underground object.");
}

void TerrainClass::unassignInfantry(Uint32 ObjectID, int currentPosition) 
{
	m_assignedInfantry.remove(ObjectID);
	LOG_INFO("TerrainClass", "Unassigned infantry.");
}

void TerrainClass::unassignObject(Uint32 ObjectID) {
	unassignInfantry(ObjectID,-1);
	unassignUndergroundUnit(ObjectID);
	unassignNonInfantryGroundObject(ObjectID);
	unassignAirUnit(ObjectID);
    LOG_INFO("TerrainClass", "Unassigning object with ID: %d", ObjectID);
}

ObjectClass* TerrainClass::getObjectWithID(Uint32 objectID) 
{
	ConcatIterator<Uint32> iterator;
	iterator.addList(m_assignedInfantry);
	iterator.addList(m_assignedNonInfantryGroundObjects);
	iterator.addList(m_assignedUndergroundUnits);
	iterator.addList(m_assignedAirUnits);
	iterator.addList(m_assignedDeadObjects);

	while(!iterator.IterationFinished()) 
	{
		if(*iterator == objectID) 
		{
			return GameState::Instance()->GetObjectTree()->getObject(*iterator);
		}
	
		++iterator;
	}

	return NULL;
}


void TerrainClass::clearDamage() {
#if 0
	m_damagePos = 0;
	for(int i=0; i<DAMAGEPERCELL; i++)
		m_damage[i].damageType = NONE;
#endif
}


void TerrainClass::damageCell(ObjectClass* damager, PlayerClass* damagerOwner, UPoint realPos, int bulletType, int bulletDamage, int damagePiercing, int damageRadius, bool air) {
#if 0
	TerrainClass* cell;
	
	if (bulletType == Unit_Sandworm) {
		ConcatIterator<Uint32> iterator;
		iterator.addList(assignedDeadObjectsList);
		iterator.addList(assignedInfantryList);
		iterator.addList(assignedNonInfantryGroundObjectList);
		
		ObjectClass* object;
		while(!iterator.IterationFinished()) {

			object = currentGame->getObjectTree().getObject(*iterator);
			if ((object->getX() == location.x) && (object->getY() == location.y)) {
				object->setVisible(VIS_ALL, false);	
				object->handleDamage(bulletDamage, damager);
			}
			++iterator;
		}
	} else {
		int			distance;
		double		damageProp;
		COORDTYPE	centrePoint;

		if (air == true) {
			// air damage
			if (hasAnAirUnit() && ((bulletType == Bullet_DRocket) || (bulletType == Bullet_Rocket) || (bulletType == Bullet_SmallRocket)))
			{
				AirUnit*	airUnit;
			
				std::list<Uint32>::const_iterator iter;
				for(iter = assignedAirUnitList.begin(); iter != assignedAirUnitList.end() ;iter++) {
					airUnit = (AirUnit*) currentGame->getObjectTree().getObject(*iter);
			
					if(airUnit == NULL)
						continue;
					
					airUnit->getCentrePoint(&centrePoint);
					distance = lround(distance_from(&centrePoint, realPos));
					if (distance <= 0) {
						distance = 1;
					}
					
					if ((distance - airUnit->getRadius()) <= damageRadius) {
						if ((bulletType == Bullet_DRocket) && (getRandomInt(0, 100) <= 30)) {
							((UnitClass*)airUnit)->netDeviate(damagerOwner);
						}

						damageProp = ((double)(damageRadius + airUnit->getRadius() - distance))/((double)distance);
						if (damageProp > 0)	{
							if (damageProp > 1.0) {
								damageProp = 1.0;
							}
							airUnit->handleDamage(lround((double)(bulletDamage + damagePiercing) * damageProp) - airUnit->getArmour(), damager);
						}
					}
				}
			}
		} else {
			// non air damage
			ConcatIterator<Uint32> iterator;
			iterator.addList(assignedNonInfantryGroundObjectList);
			iterator.addList(assignedInfantryList);
			iterator.addList(assignedUndergroundUnitList);
		
			ObjectClass* object;
			while(!iterator.IterationFinished()) {

				object = currentGame->getObjectTree().getObject(*iterator);
				
				object->getClosestCentrePoint(&centrePoint, &location);
				distance = lround(distance_from(&centrePoint, realPos));
				if (distance <= 0) {
					distance = 1;
				}
				
				if (distance - object->getRadius() <= damageRadius)	{
					if ((bulletType == Bullet_DRocket) && (object->isAUnit()) && (getRandomInt(0, 100) <= 30)) {
						((UnitClass*)object)->netDeviate(damagerOwner);
					}
					
					damageProp = ((double)(damageRadius + object->getRadius() - distance))/((double)distance);
					if (damageProp > 0)	{
						if (damageProp > 1.0) {
							damageProp = 1.0;
						}

						object->handleDamage(lround((double)(bulletDamage + damagePiercing) * damageProp) - object->getArmour(), damager);
					}
				}
				
				++iterator;
			}

			if ((getType() == Terrain_Sand)
				&& ((getTile() == Terrain_a2) || (getTile() == Terrain_a3))) {
				//a spice bloom
				int i, j;
				setType(Terrain_Spice);
				soundPlayer->playSoundAt(Sound_Bloom, getLocation());
				for (i = -6; i <= 6; i++) {
					for (j = -6; j <= 6; j++) {
						if (map->cellExists(location.x + i, location.y + j)
							&& (distance_from(location.x, location.y, location.x + i, location.y + j) <= 6))
						{
							cell = map->getCell(location.x + i, location.y + j);
							if (cell->isSand())
								cell->setType(Terrain_Spice);
						}
					}
				}
				
				for(i = location.x-8; i <= location.x+8; i++)
					for(j = location.y-8; j <= location.y+8; j++)
						if (map->cellExists(i, j))
							smooth_spot(i, j);
			}
			
			if (map->cellExists(realPos->x/BLOCKSIZE, realPos->y/BLOCKSIZE))
			{
				cell = map->getCell(realPos->x/BLOCKSIZE, realPos->y/BLOCKSIZE);
				if (((bulletType == Bullet_Rocket) || (bulletType == Bullet_SmallRocket) || (bulletType == Bullet_LargeRocket) || (bulletType == Unit_Devastator))
					&& (!hasAGroundObject() || !getGroundObject()->isAStructure())
					&& ((realPos->x <= (location.x*BLOCKSIZE + BLOCKSIZE/2))//if hasn't been assigned an object or the assigned object isnt a structure
						&& (realPos->y <= (location.y*BLOCKSIZE + BLOCKSIZE/2))))
				{
					if (!cell->hasAGroundObject() || !cell->getGroundObject()->isAStructure())
					{
						if (((cell->getType() == Terrain_Rock) && (cell->getTile() == Terrain_t1))
							|| (cell->getType() == Structure_Slab1))
						{
							if (cell->getType() == Structure_Slab1)
							{
								cell->setType(Terrain_Rock);
								cell->setOwner(NONE);
							}

							if (bulletType == Bullet_SmallRocket)
								damage[damagePos%DAMAGEPERCELL].tile = Terrain_td1;
							else
								damage[damagePos%DAMAGEPERCELL].tile = Terrain_td2;
							damage[damagePos%DAMAGEPERCELL].damageType = Terrain_RockDamage;
							damage[damagePos%DAMAGEPERCELL].realPos.x = realPos->x;
							damage[damagePos%DAMAGEPERCELL].realPos.y = realPos->y;
							damagePos++;
						}
						else if ((cell->getType() == Terrain_Sand) || (cell->getType() == Terrain_Spice))
						{
							if (bulletType == Bullet_SmallRocket)
								damage[damagePos%DAMAGEPERCELL].tile = getRandomInt(Terrain_sd1, Terrain_sd2);
							else
								damage[damagePos%DAMAGEPERCELL].tile = Terrain_sd3;
							damage[damagePos%DAMAGEPERCELL].damageType = Terrain_SandDamage;
							damage[damagePos%DAMAGEPERCELL].realPos.x = realPos->x;
							damage[damagePos%DAMAGEPERCELL].realPos.y = realPos->y;
							damagePos++;
						}
					}
				}
			}
		}
	}
#endif
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
