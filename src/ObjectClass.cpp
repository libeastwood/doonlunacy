#include "GameMan.h"
#include "GCObject.h"
#include "ObjectClass.h"
#include "DuneConstants.h"
#include "Definitions.h"
#include "Log.h"
#include "PlayerClass.h"
#include "MapClass.h"
#include "SoundPlayer.h"

ObjectClass::ObjectClass(PlayerClass* newOwner, std::string objectName) :
        Rect(0, 0, 0, 0)
{
    DataCache *cache = DataCache::Instance();
    sprite *tmp;
    std::string pic;

    m_objectName = objectName;


    m_itemID = Unknown;
    m_objectID = NONE;

    m_animCounter = 0;

    m_owner = newOwner;
    m_badlyDamaged = false;
    m_destroyed = false;
    m_maxHealth = 100;
    m_health = m_maxHealth;
    m_realPos = PointFloat(0, 0);
    m_armour = 0;
    m_radius = 0;
    m_animCounter = 0;
    m_animFrames = 1;
    m_curAnimFrame = 0;
    m_isAnimating = false;
    m_selected = false;


    m_checkTimer = 0;
    m_drawnAngle = 2;
    m_angle = 256 / 8 * m_drawnAngle;
    
    m_attackMode = STANDGROUND;

    m_attributes = OBJECT_CLASS;

    tmp = cache->getSpriteInfo(m_objectName);
    try {
	    python::dict object = (python::dict)((python::object)tmp->pyObject).attr("__dict__");
	    m_health = python::extract<int>(object["health"]);
	    m_maxHealth = python::extract<int>(object["maxHealth"]);
	    m_offset = UPoint(python::extract<int>(object["offset"][0]),
			   python::extract<int>(object["offset"][1]));
	    m_radius = python::extract<int>(object["radius"]);
	    m_viewRange = python::extract<int>(object["viewRange"]);
	    pic = python::extract<std::string>(object["picture"]);
   	    w = python::extract<int>(object["size"][0]) * 16;
	    h = python::extract<int>(object["size"][1]) * 16;
    }
    catch(python::error_already_set const &)
    {
        LOG_FATAL("ObjectClass", "Error loading object: %s", m_objectName.c_str());
        PyErr_Print();
        exit(1);
    }

    m_pic = cache->getGCObject(pic)->getImage(HOUSETYPE(getOwner()->getColour()));


}

ObjectClass::~ObjectClass()
{
    LOG_INFO("ObjectClass", "Object destroyed"); 
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

bool ObjectClass::canAttack(ObjectClass* object)
{
	if ( (object != NULL) && !object->wasDestroyed() 
	    && ( object->isAStructure() || !object->isAFlyingUnit() )
	    && ( (object->getOwner()->getTeam() != m_owner->getTeam() ) 
	    || object->getItemID() == Unit_Sandworm) && object->isVisible(m_owner->getTeam()) ) 
	{
		return true;
	} else {
		return false;
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

ObjectClass* ObjectClass::findTarget()
{
	ObjectClass	*tempTarget,
				*closestTarget = NULL;

	int	checkRange,
		xPos = x,
		yPos = y;

	MapClass * map = m_owner->getMap();

	float closestDistance = 1000000.0;

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
					float targetDistance = blockDistance( getPosition(), tempTarget->getPosition());
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

/* virtual */
UPoint ObjectClass::getClosestPoint(UPoint point)
{
    return UPoint(x, y);
}

UPoint ObjectClass::getClosestCentrePoint(UPoint objectPos)
{
	return getCentrePoint();
}



UPoint ObjectClass::getCentrePoint()
{
    UPoint result;
    
    result.x = int(round(m_realPos.x));
    result.y = int(round(m_realPos.y));

    return result;
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

			if (!m_badlyDamaged && (m_health/(float)m_maxHealth < HEAVILYDAMAGEDRATIO))
				m_badlyDamaged = true;
		}

		if (m_owner == GameMan::Instance()->LocalPlayer()) 
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

bool ObjectClass::isVisible(int team)
{
	if ((team >= 1) && (team <= MAX_PLAYERS))
		return m_visible[team-1];
	else
		return false;
}

void ObjectClass::setDestination(SPoint destination)
{
    if (m_owner->getMap()->cellExists(destination) || ((destination.x == INVALID_POS) && (destination.y == INVALID_POS)))
    {
        m_destination = destination;
    }
}

void ObjectClass::setHealth(int newHealth)
{
	if ((newHealth >= 0) && (newHealth <= m_maxHealth))
	{
		m_health = newHealth;

		if (!m_badlyDamaged && (m_health/(float)m_maxHealth < HEAVILYDAMAGEDRATIO))
			m_badlyDamaged = true;
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

void ObjectClass::unassignFromMap(SPoint pos)
{
    if (m_owner->getMap()->cellExists(pos))
        m_owner->getMap()->getCell(pos)->unassignObject(getObjectID());
}
