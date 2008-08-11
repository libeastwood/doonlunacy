#include <math.h>

#include "GameState.h"
#include "ObjectClass.h"
#include "DuneConstants.h"
#include "Definitions.h"
#include "Log.h"
#include "MapClass.h"
#include "SoundPlayer.h"

#include "units/QuadClass.h"

int lookDist[11];

ObjectClass::ObjectClass(PlayerClass* newOwner) :
        Rect(0, 0, 0, 0)
{
    lookDist[0] = 10; 
    lookDist[1] = 10;
    lookDist[2] = 9;
    lookDist[3] = 9;
    lookDist[4] = 9;
    lookDist[5] = 8;
    lookDist[6] = 8;
    lookDist[7] = 7;
    lookDist[8] = 6;
    lookDist[9] = 4;
    lookDist[10] = 1;


    m_itemID = Unknown;
    m_objectID = NONE;

    m_animCounter = 0;

    m_owner = newOwner;
    m_flyingUnit = false;
    m_groundUnit = false;
    m_structure = false;
    m_unit = false;
    m_infantry = false;
    m_builder = false;

    m_realPos = PointFloat(0, 0);

    m_animCounter = 0;
    m_animFrames = 1;
    m_curAnimFrame = 0;
    m_isAnimating = false;
    m_selected = false;

    m_checkTimer = 0;
    m_drawnAngle = 2;
    m_angle = 256 / 8 * m_drawnAngle;
    
    m_attackMode = STANDGROUND;
}

ObjectClass::~ObjectClass()
{

}

/* virtual */
void ObjectClass::assignToMap(SPoint pos)
{
    MapClass* map = m_owner->getMap();

    if (map->cellExists(pos))
    {
        map->getCell(pos)->assignNonInfantryGroundObject(getObjectID());
        map->viewMap(m_owner->getTeam(), getPosition(), m_viewRange);
    }
}

/* virtual */
void ObjectClass::draw(Image * dest, SPoint off, SPoint view)
{
    m_pic->blitTo(dest, UPoint(x + off.x, y + off.y));
}

void ObjectClass::drawSmoke(UPoint pos)
{
#if 0
	int imageW;
	SDL_Rect dest, source;
	ImagePtr smoke = DataCache::Instance()->getObjPic(ObjPic_Smoke);

	imageW = smoke->w/3;

	dest.x = x - imageW/2;
	dest.y = y - smoke->h;
	dest.w = imageW;
	dest.h = smoke->h;

	source.x = smokeLast;

	if(++smokeCounter >=SMOKEDELAY) {
		smokeCounter = 0;
	    source.x = imageW*getRandomInt(0, 2);
		smokeLast = source.x;
	};

	source.y = 0;
	source.w = imageW;
	source.h = smoke->h;

	SDL_BlitSurface(smoke, &source, screen, &dest);
#endif
}

/* virtual */
UPoint ObjectClass::getClosestPoint(UPoint point)
{
    return UPoint(x, y);
}

bool ObjectClass::canAttack(ObjectClass* object)
{
	if ((object != NULL)
		&& !object->wasDestroyed()
		&& (object->isAStructure()
			|| !object->isAFlyingUnit())
		&& ((object->getOwner()->getTeam() != m_owner->getTeam())
			|| object->getItemID() == Unit_Sandworm)
		&& object->isVisible(m_owner->getTeam())) {
		return true;
	} else {
		return false;
	}
}

ObjectClass* ObjectClass::findTarget()
{
	ObjectClass	*tempTarget,
				*closestTarget = NULL;

	int	checkRange,
		xPos = x,
		yPos = y;

	MapClass * map = GameState::Instance()->GetMap();

	double closestDistance = 1000000.0;

	//searches for a target in an area like as shown below 
	//                     *****
	//                   *********
	//                  *****T*****
	//                   *********
	//                     *****

	if (m_attackMode == STANDGROUND)
	{
		checkRange = m_weaponRange;
	}
	else
	{
		checkRange = m_guardRange;
	}
	int xCheck = xPos - checkRange;
	
	if (xCheck < 0)
		xCheck = 0;
	
	int yCheck;

	while ((xCheck < m_owner->getMap()->w) && ((xCheck - xPos) <=  checkRange))
	{
		yCheck = (yPos - lookDist[abs(xCheck - xPos)]);

		if (yCheck < 0)
			yCheck = 0;

		while ((yCheck < m_owner->getMap()->h) && ((yCheck - yPos) <=  lookDist[abs(xCheck - xPos)]))
		{
			if (map->getCell(xCheck,yCheck)->hasAnObject())
			{
				tempTarget = map->getCell(xCheck,yCheck)->getObject();

				if (((tempTarget->getItemID() != Structure_Wall) || (closestTarget == NULL)) && canAttack(tempTarget))
				{
					double targetDistance = blockDistance( getPosition(), tempTarget->getPosition());
					if (targetDistance < closestDistance)
					{
						closestTarget = tempTarget;
						closestDistance = targetDistance;
					}
				}
			}

			yCheck++;
		}

		xCheck++;
		yCheck = yPos;
	}

	return closestTarget;
}

int ObjectClass::getHealthColour()
{
    float healthPercent = (float)m_health / (float)m_maxHealth;

    if (healthPercent >= 0.7)
        return COLOUR_LIGHTGREEN;
    else if (healthPercent >= HEAVILYDAMAGEDRATIO)
        return COLOUR_YELLOW;
    else
        return COLOUR_RED;
}

int ObjectClass::getViewRange()
{
	if (m_owner->hasRadarOn() )
		return m_viewRange+2;
	else
		return m_viewRange;
}

void ObjectClass::handleDamage(int damage, ObjectClass* damager)
{
	if (!wasDestroyed()) 
	{
		if (damage >= 0) 
		{
			m_health -= damage;
			
			if (m_health < 0)
				m_health = 0;

			if (!m_badlyDamaged && (m_health/(double)m_maxHealth < HEAVILYDAMAGEDRATIO))
				m_badlyDamaged = true;
		}

		if (m_owner == GameState::Instance()->LocalPlayer()) 
		{
			//FIXME: Yeah, whatever
			//soundPlayer->changeMusic(MUSIC_ATTACK);
		}

		//FIXME: Network game
		//getOwner()->noteDamageLocation(this, &location);
	}
}

bool ObjectClass::isOnScreen(Rect rect)
{
    return rect.containsPartial(Rect(m_realPos.x, m_realPos.y, w, h));
}

void ObjectClass::setHealth(int newHealth)
{
	if ((newHealth >= 0) && (newHealth <= m_maxHealth))
	{
		m_health = newHealth;

		if (!m_badlyDamaged && (m_health/(double)m_maxHealth < HEAVILYDAMAGEDRATIO))
			m_badlyDamaged = true;
	}
}

bool ObjectClass::isVisible(int team)
{
	if ((team >= 1) && (team <= MAX_PLAYERS))
		return m_visible[team-1];
	else
		return false;
}

void ObjectClass::setVisible(int team, bool status)
{
	if (team == VIS_ALL) 
	{
		for(int i = 0; i < MAX_PLAYERS; i++)
		{
			m_visible[i] = status;
		}
	} else if ((team >= 1) && (team <= MAX_PLAYERS)) 
	{
		m_visible[--team] = status;
	}
}

void ObjectClass::setDestination(SPoint destination)
{
    if (m_owner->getMap()->cellExists(destination) || ((destination.x == INVALID_POS) && (destination.y == INVALID_POS)))
    {
        m_destination = destination;
    }
}

void ObjectClass::setPosition(SPoint pos)
{

    if ((pos.x == INVALID_POS) && (pos.y == INVALID_POS))
    {
        x = INVALID_POS;
        y = INVALID_POS;
    }

    else if (m_owner->getMap()->cellExists(pos))
    {
        x = pos.x;
        y = pos.y;
        m_realPos.x = pos.x * BLOCKSIZE;
        m_realPos.y = pos.y * BLOCKSIZE;

        assignToMap(pos);
    }
}

void ObjectClass::unassignFromMap(SPoint pos)
{
    if (m_owner->getMap()->cellExists(pos))
        m_owner->getMap()->getCell(pos)->unassignObject(getObjectID());
}

ObjectClass* ObjectClass::createObject(int ItemID,PlayerClass* Owner, Uint32 ObjectID)
{
	ObjectClass* newObject = NULL;
    //FIXME: Temporarily create quads only
	ItemID = 48; //QUAD
	switch (ItemID)
	{
	#if 0
		case Structure_Barracks:			newObject = new BarracksClass(Owner); break;
		case Structure_ConstructionYard:	newObject = new ConstructionYardClass(Owner); break;
		case Structure_GunTurret:			newObject = new GunTurretClass(Owner); break;
		case Structure_HeavyFactory:		newObject = new HeavyFactoryClass(Owner); break;
		case Structure_HighTechFactory:		newObject = new HighTechFactoryClass(Owner); break;
		case Structure_IX:					newObject = new IXClass(Owner); break;
		case Structure_LightFactory:		newObject = new LightFactoryClass(Owner); break;
		case Structure_Palace:				newObject = new PalaceClass(Owner); break;
		case Structure_Radar:				newObject = new RadarClass(Owner); break;
		case Structure_Refinery:			newObject = new RefineryClass(Owner); break;
		case Structure_RepairYard:			newObject = new RepairYardClass(Owner); break;
		case Structure_RocketTurret:		newObject = new RocketTurretClass(Owner); break;
		case Structure_Silo:				newObject = new SiloClass(Owner); break;
		case Structure_StarPort:			newObject = new StarPortClass(Owner); break;
		case Structure_Wall:				newObject = new WallClass(Owner); break;
		case Structure_WindTrap:			newObject = new WindTrapClass(Owner); break;
		case Structure_WOR:					newObject = new WORClass(Owner); break;
		
		case Unit_Carryall:					newObject = new Carryall(Owner); break;
		case Unit_Devastator:				newObject = new DevastatorClass(Owner); break;
		case Unit_Deviator:					newObject = new DeviatorClass(Owner); break;
		case Unit_Frigate:					newObject = new Frigate(Owner); break;
		case Unit_Harvester:				newObject = new HarvesterClass(Owner); break;
		case Unit_Infantry:					newObject = new InfantryClass(Owner); break;
		case Unit_Launcher:					newObject = new LauncherClass(Owner); break;
		case Unit_MCV:						newObject = new MCVClass(Owner); break;
		case Unit_Ornithopter:				newObject = new Ornithopter(Owner); break;
		case Unit_Quad:						newObject = new QuadClass(Owner); break;
		case Unit_Saboteur:					newObject = new Saboteur(Owner); break;
		case Unit_Sandworm:					newObject = new Sandworm(Owner); break;
		case Unit_SiegeTank:				newObject = new SiegeTankClass(Owner); break;
		case Unit_SonicTank:				newObject = new SonicTankClass(Owner); break;
		case Unit_Tank:						newObject = new TankClass(Owner); break;
		case Unit_Trike:					newObject = new TrikeClass(Owner); break;
		case Unit_Raider:					newObject = new RaiderClass(Owner); break;
		case Unit_Trooper:					newObject = new TrooperClass(Owner); break;
		case Unit_Sardaukar:				newObject = new SardaukarClass(Owner); break;
		case Unit_Fremen:					newObject = new FremenClass(Owner); break;
	#endif
		case Unit_Quad:						newObject = new QuadClass(Owner); break;
		default:							newObject = NULL;
											LOG_ERROR("ObjectClass", "%d is no valid ItemID!",ItemID);
											break;
	}
	
	if(newObject == NULL)
		return NULL;
	
	if(ObjectID == (Uint32)NONE) {
		ObjectID = GameState::Instance()->GetObjectTree()->AddObject(newObject);
		newObject->setObjectID(ObjectID);
	} else {
		newObject->setObjectID(ObjectID);
	}
	
	return newObject;
}
