#include "TerrainClass.h"

#include "ConcatIterator.h"
#include "DataCache.h"
#include "GameData.h"
#include "Definitions.h"
#include "GameMan.h"
#include "Log.h"
#include "Gfx.h"

#include <math.h>

using namespace std;

TerrainClass::TerrainClass() : UPoint(0,0)
{
    m_difficulty = 1;
    m_spice = 0;
    m_tile = Terrain_a1;
    m_type = Terrain_Sand;
    m_hideTile = Terrain_HiddenFull;
    m_img = DataCache::Instance()->getGameData("ObjPic_AllTerrain")->getImage();
    m_hiddenImg = DataCache::Instance()->getGameData("ObjPic_Terrain_Hidden")->getImage();
    m_visited = false;

    for (int i = 0; i < MAX_PLAYERS; i++)
    	m_explored[i] = false;

}

TerrainClass::~TerrainClass()
{

}

void TerrainClass::draw(Image& dest, SPoint pos)
{
    if (isExplored(GameMan::Instance()->LocalPlayer().getPlayerNumber()))
    {
        Rect source(m_tile*BLOCKSIZE, 0, BLOCKSIZE, BLOCKSIZE);
        m_img->blitTo(dest, source, pos);
        
        if (m_hideTile != Terrain_HiddenFull)
        {
            Rect sourceHidden(m_hideTile*BLOCKSIZE, 0, BLOCKSIZE, BLOCKSIZE);
            m_hiddenImg->blitTo(dest, sourceHidden, pos);
        }
    }
   
}

/*
ObjectClass* TerrainClass::getAirUnit()
{
	GameMan* gman = GameMan::Instance();
    return gman->GetObjectTree()->getObject(m_assignedAirUnits.front());
}

ObjectClass* TerrainClass::getDeadObject()
{
	GameMan* gman = GameMan::Instance();
    return gman->GetObjectTree()->getObject(m_assignedDeadObjects.front());
}
*/
ObjectPtr TerrainClass::getGroundObject()
{
    ObjectMap::const_iterator iter;
    ObjectPtr object;
    for(iter = m_assignedObjects.begin(); iter != m_assignedObjects.end(); iter++)
    {
	ObjectPtr tmp = (*iter).second;
	//FIXME: Checking that tmp isn't NULL first shouldn't be necessary..
	if(tmp && tmp->hasAttribute(OBJECT_GROUNDUNIT))
	    return tmp;
    }
    return object;
}

/*
ObjectClass* TerrainClass::getInfantry()
{
	GameMan* gman = GameMan::Instance();
    return gman->GetObjectTree()->getObject(m_assignedInfantry.front());
}

ObjectClass* TerrainClass::getNonInfantryGroundObject()
{
	GameMan* gman = GameMan::Instance();
    return gman->GetObjectTree()->getObject(m_assignedNonInfantryGroundObjects.front());
}

ObjectClass* TerrainClass::getUndergroundUnit()
{
	GameMan* gman = GameMan::Instance();
    return gman->GetObjectTree()->getObject(m_assignedUndergroundUnits.front());
}
*/
void TerrainClass::assignObject(uint32_t newObjectID)
{
    if(newObjectID == (uint32_t)-1)
	throw "object lacks objectID";
    m_assignedObjects.insert(ObjectPair(newObjectID, GameMan::Instance()->getObject(newObjectID)));
}

ObjectPtr TerrainClass::getObject() {
    ObjectPtr object;
    if(!m_assignedObjects.empty())
	object = m_assignedObjects.begin()->second;

    return object;
}


ObjectPtr TerrainClass::getObjectAt(UPoint pos) {
	return getObject();
#if 0
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
				
		std::list<uint32_t>::const_iterator iter;
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
#endif
}

bool TerrainClass::unassignObject(uint32_t objectID) {
    return m_assignedObjects.erase(objectID);
}

ObjectPtr TerrainClass::getObjectWithID(uint32_t objectID) 
{
	return m_assignedObjects.find(objectID)->second;
}


void TerrainClass::clearDamage() 
{
	m_damagePos = 0;
	
	for(int i=0; i<DAMAGEPERCELL; i++)
	{
		m_damage[i].damageType = NONE;
    }
}


void TerrainClass::damageCell(WeaponClass *weapon, UPoint realPos)
{
    ObjectMap::const_iterator iterator;
    for(iterator = m_assignedObjects.begin(); iterator != m_assignedObjects.end(); iterator++) {
	ObjectPtr object = iterator->second;

	LOG(LV_DEBUG, "TerrainClass", "damageCell(): %s (%u) might damage %s (%u) at %d-%d (%d-%d)", weapon->getObjectName().c_str(), weapon->getObjectID(), object->getObjectName().c_str(), object->getObjectID(), realPos.x, realPos.y, (realPos/BLOCKSIZE).x, (realPos/BLOCKSIZE).y);
	weapon->dealDamage(object, realPos);
    }

}

#if 0
	TerrainClass* cell;
	
	if (bulletType == Unit_Sandworm) {
		ConcatIterator<uint32_t> iterator;
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
		float		damageProp;
		COORDTYPE	centrePoint;

		if (air == true) {
			// air damage
			if (hasAnAirUnit() && ((bulletType == Bullet_DRocket) || (bulletType == Bullet_Rocket) || (bulletType == Bullet_SmallRocket)))
			{
				AirUnit*	airUnit;
			
				std::list<uint32_t>::const_iterator iter;
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

						damageProp = ((float)(damageRadius + airUnit->getRadius() - distance))/((float)distance);
						if (damageProp > 0)	{
							if (damageProp > 1.0) {
								damageProp = 1.0;
							}
							airUnit->handleDamage(lround((float)(bulletDamage + damagePiercing) * damageProp) - airUnit->getArmor(), damager);
						}
					}
				}
			}
		} else {
			// non air damage
			ConcatIterator<uint32_t> iterator;
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
					
					damageProp = ((float)(damageRadius + object->getRadius() - distance))/((float)distance);
					if (damageProp > 0)	{
						if (damageProp > 1.0) {
							damageProp = 1.0;
						}

						object->handleDamage(lround((float)(bulletDamage + damagePiercing) * damageProp) - object->getArmor(), damager);
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
					&& (!hasAGroundObject() || !getGroundObject()->hasAttribute(OBJECT_STRUCTURE))
					&& ((realPos->x <= (location.x*BLOCKSIZE + BLOCKSIZE/2))//if hasn't been assigned an object or the assigned object isnt a structure
						&& (realPos->y <= (location.y*BLOCKSIZE + BLOCKSIZE/2))))
				{
					if (!cell->hasAGroundObject() || !cell->getGroundObject()->hasAttribute(OBJECT_STRUCTURE))
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
}
#endif


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
    //FIXME:Need to implement fog-o-war
    return false;
}
