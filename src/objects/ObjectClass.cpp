#include "GameMan.h"
#include "GCObject.h"
#include "objects/ObjectClass.h"
#include "DuneConstants.h"
#include "Definitions.h"
#include "Log.h"
#include "PlayerClass.h"
#include "MapClass.h"
#include "SoundPlayer.h"

ObjectClass::ObjectClass(PlayerClass* newOwner, std::string objectName, Uint32 attribute) :
    Rect(0, 0, 0, 0)
{
    m_attributes = OBJECT_CLASS | attribute;
    m_status = STATUS_NONE;


    DataCache *cache = DataCache::Instance();
    std::string graphic;

    m_objectName = objectName;


    m_animCounter = 0;
    m_frameTime = 5;
    m_frameTimer = -1;

    m_owner = newOwner;
    m_badlyDamaged = false;
    m_realPos = PointFloat(0, 0);
    m_curAnimFrame = 0;
    m_isAnimating = false;
    m_selected = false;
    m_fadingIn = false;
    m_adjust = 0.0;
    m_controllable = (m_owner == GameMan::Instance()->LocalPlayer());


    m_checkTimer = 0;

    m_attackMode = STANDGROUND;


    try {
	m_angle = cache->getPyObjectAttribute<int>(m_objectName, "angle");
	m_animFrames = cache->getPyObjectAttribute<int>(m_objectName, "animFrames");
	m_armor = cache->getPyObjectAttribute<int>(m_objectName, "armor");
	m_deathAnim = cache->getPyObjectAttribute<std::string>(m_objectName, "deathAnim");
	m_drawnAngle = cache->getPyObjectAttribute<int>(m_objectName, "drawnAngle");
	m_drawnPos = cache->getPyObjectAttribute<UPoint>(m_objectName, "drawnPos");
	m_explosionSize = cache->getPyObjectAttribute<int>(m_objectName, "explosionSize");
	m_guardRange = cache->getPyObjectAttribute<int>(m_objectName, "guardRange");
	graphic = cache->getPyObjectAttribute<std::string>(m_objectName, "graphic");
	m_maxHealth = cache->getPyObjectAttribute<int>(m_objectName, "maxHealth");
	m_numDeathFrames = cache->getPyObjectAttribute<int>(m_objectName, "numDeathFrames");
	m_numFrames = cache->getPyObjectAttribute<int>(m_objectName, "numFrames");
	m_health = cache->getPyObjectAttribute<int>(m_objectName, "health");
	m_offset = UPoint(PointFloat(cache->getPyObjectAttribute<PointFloat>(m_objectName, "offset")) * BLOCKSIZE),
	m_radius = cache->getPyObjectAttribute<int>(m_objectName, "radius");
	m_realPos = cache->getPyObjectAttribute<PointFloat>(m_objectName, "realPos");
	m_maxSpeed = cache->getPyObjectAttribute<float>(m_objectName, "speed");
	m_turnSpeed = cache->getPyObjectAttribute<float>(m_objectName, "turnSpeed");
	m_viewRange = cache->getPyObjectAttribute<int>(m_objectName, "viewRange");
	m_weapons = cache->getPyObjectVector<std::string>(m_objectName, "weapons");
	w = (cache->getPyObjectAttribute<float>(m_objectName, "size", 0) * BLOCKSIZE);
	h = (cache->getPyObjectAttribute<float>(m_objectName, "size", 1) * BLOCKSIZE);
    }
    catch(python::error_already_set const &)
    {
	LOG_FATAL("ObjectClass", "Error loading object: %s", m_objectName.c_str());
	PyErr_Print();
	exit(1);
    }

    m_graphic = cache->getGCObject(graphic)->getImage((m_owner == NULL) ? (HOUSETYPE)HOUSE_HARKONNEN : (HOUSETYPE)m_owner->getHouse());
    m_selectionBox = DataCache::Instance()->getGCObject("UI_SelectionBox")->getImage();

}

ObjectClass::~ObjectClass()
{
    LOG_INFO("ObjectClass", "Object destroyed"); 
}

/* virtual */
void ObjectClass::assignToMap(SPoint pos)
{
    MapClass* map = m_owner->getMap();

    // If structure is more than 1x1, be sure to assign it to all cells required
    for (int i = pos.x; i < x + w/BLOCKSIZE; i++)
	for (int j = pos.y; j < y + h/BLOCKSIZE; j++) {
	    SPoint temp(i,j);
	    if (map->cellExists(temp)) {
		map->getCell(temp)->assignObject(m_objectID);
		map->viewMap(m_owner->getTeam(), getPosition(), m_viewRange);
	    }
	}
}

bool ObjectClass::canAttack(ObjectPtr object)
{
    if ( (object != NULL) && !object->getStatus(STATUS_DESTROYED) 
	    && ( object->isAStructure() || !object->isAFlyingUnit() )
	    && ( (object->getOwner()->getTeam() != m_owner->getTeam() ) 
		|| object->getObjectName() == "Sandworm") && object->isVisible(m_owner->getTeam()) ) 
	return true;
    else
	return false;
}

void ObjectClass::setDrawnPos(SPoint off, SPoint view)
{
    m_drawnPos = (off + m_realPos - view * BLOCKSIZE) - m_offset;
}


/* virtual */
void ObjectClass::draw(Image * dest, SPoint off, SPoint view)
{

    setDrawnPos(off, view);

    if (!getStatus(STATUS_DESTROYED))
    {
	Rect source(m_numFrames > 1 ? m_drawnAngle * w : m_curAnimFrame * w, 0, w, h);
	m_graphic->blitTo(dest, source, m_drawnPos);
    }
    else if (m_numDeathFrames > 1)
	doDeath(dest);

    animate();
}

void ObjectClass::drawSelectionBox(Image* dest)
{
    if(!m_selectionBoxGlowing)
	m_selectionBoxGlowing = m_selectionBox->getCopy();

    bool reset = m_fadingIn ? m_selectionBoxGlowing->fadeIn() : !m_selectionBoxGlowing->fadeOut();

    m_selectionBoxGlowing->blitTo(dest, m_drawnPos);

    if(m_fadingIn != reset)
    {
	    m_selectionBoxGlowing.reset();
	    m_fadingIn = reset;
    }
 
    dest->drawHLine(UPoint(m_drawnPos.x + 2, m_drawnPos.y + 2), m_drawnPos.x + 1 + ((int)(((float)m_health / (float)m_maxHealth)*(w - 3))), getHealthColour());
} //want it to start in one from edges  finish one from right edge

void ObjectClass::drawSmoke(Image *dest)
{
#if 1
    Rect source(w * m_curAnimFrame, 0, w, h);

    ImagePtr smoke = DataCache::Instance()->getGCObject("ObjPic_Smoke")->getImage((m_owner == NULL) ? (HOUSETYPE)HOUSE_HARKONNEN : (HOUSETYPE)m_owner->getHouse());


    source.x = m_smokeFrame;

    if(++m_smokeCounter >=SMOKEDELAY) {
	m_smokeCounter = 0;
	source.x = w*getRandomInt(0, 2);
	m_smokeFrame = source.x;
    };

    m_graphic->blitTo(dest, source, m_drawnPos);
#endif
}

void ObjectClass::destroy()
{
    if (!(getStatus(STATUS_DESTROYED)))
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

	setStatus(STATUS_DESTROYED);
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
    UPoint destPoint((m_drawnPos - ((m_explosionSize/2) * BLOCKSIZE)) - BLOCKSIZE/2);

    for(int x = 0; x < m_explosionSize; x++, destPoint.x += BLOCKSIZE)
	for(int y = 0; y < m_explosionSize; y++)
	    if ((m_explosionSize <= 2) || ((x != 0) && (x != (m_explosionSize-1))) || ((y != 0) && (y != (m_explosionSize-1))))
		m_graphic->blitTo(dest, source, destPoint + UPoint(0, y*BLOCKSIZE));
}

ObjectPtr ObjectClass::findTarget()
{
    ObjectPtr	tempTarget,
		closestTarget;

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
	checkRange = m_weaponRange;
    else
	checkRange = m_guardRange;

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
	    if (map->getCell(SPoint(xCheck,yCheck))->hasAnObject())
	    {
		tempTarget = map->getCell(SPoint(xCheck,yCheck))->getObject();

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

void ObjectClass::handleDamage(int damage, ObjectPtr damager)
{
    if (!getStatus(STATUS_DESTROYED))
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

void ObjectClass::setDestination(SPoint destination, Uint32 status)
{
    setStatus(status);
    m_target.reset();
    if (m_owner->getMap()->cellExists(destination) || ((destination.x == INVALID_POS) && (destination.y == INVALID_POS)))
    {

	if(getStatus(STATUS_ATTACKING)) {
    	    m_target = GameMan::Instance()->GetMap()->getCell(destination)->getObject();
	    UPoint targetPos(m_target ? m_target->getRealPos() : UPoint(destination * BLOCKSIZE));
	    if(!m_weapons.empty())
	    {
    		ObjectPtr missile(new WeaponClass(GameMan::Instance()->getObject(getObjectID()), m_weapons.front(), m_realPos, targetPos, false));
    		GameMan::Instance()->addObject(missile);
		unsetStatus(STATUS_MOVING);
	    }
	}

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
	x = y = INVALID_POS;
    else if (m_owner->getMap()->cellExists(pos))
    {
	x = pos.x;
	y = pos.y;
	m_realPos = pos * BLOCKSIZE;

	assignToMap(pos);
    }
}

void ObjectClass::setVisible(int team, bool status)
{
    if (team == VIS_ALL) 
	for(int i = 0; i < MAX_PLAYERS; i++)
	    m_visible[i] = status;
    else if ((team >= 1) && (team <= MAX_PLAYERS)) 
	m_visible[--team] = status;
}

void ObjectClass::unassignFromMap(SPoint pos)
{
    if (m_owner->getMap()->cellExists(pos))
	m_owner->getMap()->getCell(pos)->unassignObject(m_objectID);
}

void ObjectClass::update(float dt)
{
    m_adjust = dt * (Settings::Instance()->GetGameSpeed() * 10);
}
