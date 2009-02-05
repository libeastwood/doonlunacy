#include "GameMan.h"
#include "GCObject.h"
#include "objects/ObjectClass.h"
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
    std::string graphic;

    m_objectName = objectName;


    m_objectID = NONE;

    m_animCounter = 0;
    m_frameTime = 5;
    m_frameTimer = -1;

    m_owner = newOwner;
    m_badlyDamaged = false;
    m_destroyed = false;
    m_realPos = PointFloat(0, 0);
    m_curAnimFrame = 0;
    m_isAnimating = false;
    m_selected = false;
    m_adjust = 0.0;


    m_checkTimer = 0;
    
    m_attackMode = STANDGROUND;

    m_attributes = OBJECT_CLASS;

    try {
	    python::dict object = (python::dict)((python::object)cache->getPyObject(m_objectName)).attr("__dict__");
	    m_angle = cache->getPyObjectAttribute<int>(m_objectName, "angle");
	    m_animFrames = cache->getPyObjectAttribute<int>(m_objectName, "animFrames");
	    m_armor = cache->getPyObjectAttribute<int>(m_objectName, "armor");
            m_deathAnim = cache->getPyObjectAttribute<std::string>(m_objectName, "deathAnim");
	    m_drawnAngle = cache->getPyObjectAttribute<int>(m_objectName, "drawnAngle");
	    m_drawnPos = UPoint(cache->getPyObjectAttribute<int>(m_objectName, "drawnPos", 0),
			   cache->getPyObjectAttribute<int>(m_objectName, "drawnPos", 1));
	    m_explosionSize = cache->getPyObjectAttribute<int>(m_objectName, "explosionSize");
	    m_guardRange = cache->getPyObjectAttribute<int>(m_objectName, "guardRange");
	    graphic = cache->getPyObjectAttribute<std::string>(m_objectName, "graphic");
	    m_maxHealth = cache->getPyObjectAttribute<int>(m_objectName, "maxHealth");
            m_numDeathFrames = cache->getPyObjectAttribute<int>(m_objectName, "numDeathFrames");
            m_numFrames = cache->getPyObjectAttribute<int>(m_objectName, "numFrames");
	    m_health = cache->getPyObjectAttribute<int>(m_objectName, "health");
	    m_offset = UPoint(cache->getPyObjectAttribute<int>(m_objectName, "offset", 0),
			   cache->getPyObjectAttribute<int>(m_objectName, "offset", 1));
	    m_radius = cache->getPyObjectAttribute<int>(m_objectName, "radius");
	    m_realPos = PointFloat(cache->getPyObjectAttribute<float>(m_objectName, "realPos", 0),
			   cache->getPyObjectAttribute<float>(m_objectName, "realPos", 1));
            m_speed = cache->getPyObjectAttribute<float>(m_objectName, "speed");
            m_turnSpeed = cache->getPyObjectAttribute<float>(m_objectName, "turnSpeed");
	    m_viewRange = cache->getPyObjectAttribute<int>(m_objectName, "viewRange");
   	    w = cache->getPyObjectAttribute<int>(m_objectName, "size", 0) * BLOCKSIZE;
	    h = cache->getPyObjectAttribute<int>(m_objectName, "size", 1) * BLOCKSIZE;
    }
    catch(python::error_already_set const &)
    {
        LOG_FATAL("ObjectClass", "Error loading object: %s", m_objectName.c_str());
        PyErr_Print();
        exit(1);
    }

    m_graphic = cache->getGCObject(graphic)->getImage((m_owner == NULL) ? (HOUSETYPE)HOUSE_HARKONNEN : (HOUSETYPE)m_owner->getHouse());


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
	    || object->getObjectName() == "Sandworm") && object->isVisible(m_owner->getTeam()) ) 
	{
		return true;
	} else {
		return false;
	}
}

void ObjectClass::setDrawnPos(SPoint off, SPoint view)
{
    m_drawnPos.x = off.x + m_realPos.x - view.x * BLOCKSIZE - w / 2;
    m_drawnPos.y = off.y + m_realPos.y - view.y * BLOCKSIZE - h / 2;
}


/* virtual */
void ObjectClass::draw(Image * dest, SPoint off, SPoint view)
{

    setDrawnPos(off, view);
    Rect source;
	
	if (!m_destroyed)
	{
		source.x = 0;

		if (m_numFrames > 1)
			source.x = m_drawnAngle * w;

		source.y = 0;
        
        source.w = 16;
        source.h = 16;
        m_graphic->blitTo(dest, source, m_drawnPos);
	}
	else if (m_numDeathFrames > 1)
	{
	       doDeath(dest);
	       animate();
	}


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

void ObjectClass::destroy()
{
    MapClass* map = GameMan::Instance()->GetMap();
    
	if (!m_destroyed)
	{
		/*
		UPoint realPos = UPoint((short)m_realPos.x, (short)m_realPos.y);

		for(int i = 0; i < m_explosionSize; i++)
		for(int j = 0; j < m_explosionSize; j++)
		if (( m_explosionSize <= 2) || ((i != 0) && (i != (m_explosionSize-1))) || ((j != 0) && (j != (m_explosionSize-1))))
		{
        		realPos.x = m_drawnPos.x + (i - (m_explosionSize/2))*BLOCKSIZE - BLOCKSIZE/2;
        		realPos.y = m_drawnPos.y + (j - (m_explosionSize/2))*BLOCKSIZE - BLOCKSIZE/2;

			//if (deathSound != NONE)
			//	soundPlayer->playSound(deathSound);
		}
*/
		//imageW = deathGraphic[0][1]->w/numDeathFrames;
		//imageH = deathGraphic[0][1]->h;
		//xOffset = (imageW - BLOCKSIZE)/2;		    //this is where it actually draws the graphic
		//yOffset = (imageH - BLOCKSIZE)/2;		    //cause it draws at top left, not middle
		//	SDL_FreeSurface(graphic);
    		//if (deathSound != NONE)
		//	soundPlayer->playSound(deathSound);

		m_graphic = DataCache::Instance()->getGCObject(m_deathAnim)->getImage((m_owner == NULL) ? (HOUSETYPE)HOUSE_HARKONNEN : (HOUSETYPE)m_owner->getHouse());

		m_destroyed = true;
		m_frameTimer = m_frameTime;
	}
}

void ObjectClass::animate()
{
    if (m_frameTimer > 0)
    {

	if(m_frameTimer == 1)
		{
			if(++m_curAnimFrame < m_numDeathFrames)
				m_frameTimer = m_frameTime;
		}
	m_frameTimer--;
    }	
}

void ObjectClass::doDeath(Image *dest)
{
		Rect source(w * m_curAnimFrame, 0, w, h);

		for(int i = 0; i < m_explosionSize; i++)
    		for(int j = 0; j < m_explosionSize; j++)
        	if ((m_explosionSize <= 2) || ((i != 0) && (i != (m_explosionSize-1))) || ((j != 0) && (j != (m_explosionSize-1))))
        	{
			UPoint destPoint;
        		destPoint.x = m_drawnPos.x + (i - (m_explosionSize/2))*BLOCKSIZE - BLOCKSIZE/2;
        		destPoint.y = m_drawnPos.y + (j - (m_explosionSize/2))*BLOCKSIZE - BLOCKSIZE/2;
			m_graphic->blitTo(dest, source, destPoint);
		}
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

				if (((tempTarget->getObjectName() != "Wall") || (closestTarget == NULL)) && canAttack(tempTarget))
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
