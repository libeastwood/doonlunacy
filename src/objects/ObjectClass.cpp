#include <cmath>

#include "GameMan.h"
#include "GameData.h"
#include "objects/ObjectClass.h"
#include "DuneConstants.h"
#include "Definitions.h"
#include "Log.h"
#include "PlayerClass.h"
#include "MapClass.h"
#include "SoundPlayer.h"
#include "PythonObjects.h"

ObjectClass::ObjectClass(PlayerClass* newOwner, std::string objectName, Uint32 attribute) :
    Rect(0, 0, 0, 0), m_objectName(objectName), m_attributes(attribute)
{
    std::string graphic, topGraphic;
    std::vector<python::object> pyWeapons;

    m_owner = newOwner;
    m_status = STATUS_NONE;

    if(m_owner == GameMan::Instance()->LocalPlayer())
	setStatus(STATUS_CONTROLLABLE);

    m_pyObject = DataCache::Instance()->getPyObject(m_objectName);


    m_animCounter = 0;
    m_frameTime = 5;
    m_frameTimer = -1;

    m_realPosition = PointFloat(0, 0);
    m_curAnimFrame = 0;
    m_fadingIn = false;
    m_adjust = 0.0;
    m_objectID = -1;

    m_checkTimer = 0;
    m_ticks = 0;

    m_attackMode = STANDGROUND;

    m_drawnTopAngle = getRandom(0,7);
    try {
	PointFloat size;
	m_angle = python::extract<int>(m_pyObject.attr("angle"));
	m_animFrames = python::extract<int>(m_pyObject.attr("animFrames"));
	m_armor = python::extract<int>(m_pyObject.attr("armor"));
	m_deathAnim = getPyObjectType(m_pyObject.attr("deathAnim"), 0);
	m_decayTime = python::extract<int>(m_pyObject.attr("decayTime"));
	m_drawnAngle = python::extract<int>(m_pyObject.attr("drawnAngle"));
	m_drawnPos = python::extract<SPoint>(m_pyObject.attr("drawnPos"));
	m_explosionSize = python::extract<int>(m_pyObject.attr("explosionSize"));
	m_guardRange = python::extract<int>(m_pyObject.attr("guardRange"));
	graphic = getPyObjectType(m_pyObject.attr("graphic"), 0);
	topGraphic = getPyObjectType(m_pyObject.attr("topGraphic"), 0);
	if(topGraphic != "NoneType")
	    m_topOffset = getPyObjectVector<SPoint>(m_pyObject.attr("topOffset"));
	m_maxHealth = python::extract<int>(m_pyObject.attr("maxHealth"));
	m_numDeathFrames = python::extract<int>(m_pyObject.attr("numDeathFrames"));
	m_numFrames = python::extract<int>(m_pyObject.attr("numFrames"));
	m_health = python::extract<int>(m_pyObject.attr("health"));
	m_objectType = getPyObjectType(m_pyObject, 1);
	m_offset = UPoint(PointFloat(python::extract<PointFloat>(m_pyObject.attr("offset"))) * BLOCKSIZE);
	m_radius = python::extract<int>(m_pyObject.attr("radius"));
	m_realPosition = python::extract<PointFloat>(m_pyObject.attr("realPos"));
	m_maxSpeed = python::extract<float>(m_pyObject.attr("speed"));
	m_turnSpeed = python::extract<float>(m_pyObject.attr("turnSpeed"));
	m_viewRange = python::extract<int>(m_pyObject.attr("viewRange"));
	pyWeapons = getPyObjectVector<python::object>(m_pyObject.attr("weapons"));
	if(getPyObject<PointFloat>(m_pyObject.attr("size"), &size))
	    setSize(size*BLOCKSIZE);
    } catch(python::error_already_set const &) {
	LOG(LV_FATAL, m_objectType, "Error loading object: %s", getObjectName().c_str());
	PyErr_Print();
	throw;
    }

    m_graphic = DataCache::Instance()->getGameData(graphic)->getImage((m_owner == NULL) ? (HOUSETYPE)HOUSE_HARKONNEN : (HOUSETYPE)m_owner->getHouse());
    if(topGraphic != "NoneType")
    	m_topGraphic = DataCache::Instance()->getGameData(topGraphic)->getImage((m_owner == NULL) ? (HOUSETYPE)HOUSE_HARKONNEN : (HOUSETYPE)m_owner->getHouse());

    m_selectionBox = DataCache::Instance()->getGameData("UI_SelectionBox")->getImage();
    m_visible.resize(MAX_PLAYERS);

    m_weapons.resize(pyWeapons.size());
    for(size_t i = 0; i < pyWeapons.size(); i++)
	m_weapons[i] = WeaponPtr(new WeaponClass(m_owner, getPyObjectType(pyWeapons[i], 0)));

    if(m_decayTime)
	m_decayTime -= getRandom<Sint16>(1+m_decayTime/4, m_decayTime);
}

ObjectClass::~ObjectClass()
{
    LOG(LV_INFO, m_objectType, "Destroyed: %S", &((ConstString)*this));
}

/* virtual */
void ObjectClass::assignToMap(SPoint pos)
{
    MapClass* map = m_owner->getMap();

    UPoint tileSize(ceil((float)w/BLOCKSIZE), ceil((float)h/BLOCKSIZE));
    // If structure is more than 1x1, be sure to assign it to all cells required
    for (int i = pos.x; i < x + tileSize.x; i++)
	for (int j = pos.y; j < y + tileSize.y; j++) {
	    SPoint temp(i,j);
	    if (map->cellExists(temp)) {
		map->getCell(temp)->assignObject(m_objectID);
		map->viewMap(m_owner->getTeam(), getPosition(), m_viewRange);
	    }
	}
}

bool ObjectClass::canAttack(ObjectPtr object) const
{
    if ( (object != NULL) && !object->getStatus(STATUS_DESTROYED) 
	    && ( object->hasAttribute(OBJECT_STRUCTURE) || !object->hasAttribute(OBJECT_AIRUNIT) )
	    && ( (object->getOwner()->getTeam() != m_owner->getTeam() ) 
		|| object->getObjectName() == "Sandworm") && object->isVisible(m_owner->getTeam()) ) 
	return true;
    else
	return false;
}

void ObjectClass::setDrawnPos(SPoint off, SPoint view)
{
    m_drawnPos = (off + m_realPosition - view * BLOCKSIZE) - m_offset;
}


/* virtual */
void ObjectClass::draw(Image * dest, SPoint off, SPoint view)
{

    setDrawnPos(off, view);

    if (!getStatus(STATUS_DESTROYED))
    {
	Rect source(m_numFrames > 1 ? m_drawnAngle * w : m_curAnimFrame * w, 0, w, h);
	m_graphic->blitTo(dest, source, m_drawnPos);
	if(m_topGraphic)
	{
	    source.w = source.h = m_topGraphic->getSize().y;
	    int16_t topOffset = m_drawnTopAngle;
	    if(topOffset > (signed)m_topOffset.size() - 1 || topOffset < 0)
		topOffset -= (topOffset/m_topOffset.size()) * m_topOffset.size();
	    
	    source.x = m_numFrames > 1 ? m_drawnTopAngle * source.w : m_curAnimFrame * source.w;
	    m_topGraphic->blitTo(dest, source, m_drawnPos + m_topOffset[drawnTopAngle]);
	}
    }
    else if (m_numDeathFrames > 1)
	doDeath(dest);

    animate();
}

void ObjectClass::drawSelectionBox(Image* dest)
{
#if 0
    if(!m_selectionBoxGlowing)
	m_selectionBoxGlowing = m_selectionBox->getCopy();

    bool reset = m_fadingIn ? m_selectionBoxGlowing->fadeIn() : !m_selectionBoxGlowing->fadeOut();

    m_selectionBoxGlowing->blitTo(dest, m_drawnPos);

    if(m_fadingIn != reset)
    {
	    m_selectionBoxGlowing.reset();
	    m_fadingIn = reset;
    }
#else
    m_selectionBox->blitTo(dest, m_drawnPos);
#endif

    int health = ((int)(((float)m_health / (float)m_maxHealth)*(w - 3)));

    ImagePtr healthBar(new Image(UPoint(health,1)));
    healthBar->fillRect(getHealthColour());
    healthBar->blitTo(dest, m_drawnPos+2);
}

void ObjectClass::drawSmoke(Image *dest)
{
#if 1
    Rect source(w * m_curAnimFrame, 0, w, h);

    ImagePtr smoke = DataCache::Instance()->getGameData("ObjPic_Smoke")->getImage((m_owner == NULL) ? (HOUSETYPE)HOUSE_HARKONNEN : (HOUSETYPE)m_owner->getHouse());


    source.x = m_smokeFrame;

    if(++m_smokeCounter >=SMOKEDELAY) {
	m_smokeCounter = 0;
	source.x = w*getRandom(0, 2);
	m_smokeFrame = source.x;
    };

    m_graphic->blitTo(dest, source, m_drawnPos);
#endif
}

bool ObjectClass::destroy()
{
    if (!(getStatus(STATUS_DESTROYED)))
    {
	/*
	   UPoint realPos = UPoint((short)m_realPosition.x, (short)m_realPos.y);

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

	m_graphic = DataCache::Instance()->getGameData(m_deathAnim)->getImage((m_owner == NULL) ? (HOUSETYPE)HOUSE_HARKONNEN : (HOUSETYPE)m_owner->getHouse());

	setStatus(STATUS_DESTROYED);
	m_frameTimer = m_frameTime;
	return true;
    }
    return false;
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
    if(m_frameTimer <= 0) {
	if(m_decayTime > 0) {
	    Uint32 tick = SDL_GetTicks();
	    if(tick > (m_ticks + 1000)) {
		swap(m_ticks, tick);		
		m_decayTime--;
		m_graphic->fadeOut();
	    }
	}
	else
	    return;
    }
    Rect source(w * m_curAnimFrame, 0, w, h);
    UPoint destPoint((m_drawnPos - ((m_explosionSize/2) * BLOCKSIZE)));

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
SPoint ObjectClass::getClosestPoint(SPoint point) const
{
    SPoint min(getRealPosition()-(getSize()/2)),
	   max(getRealPosition()+(getSize()/2)),
	   tmp, closest;
    float closestDistance = (1<<15)-1,
	     distance;
    for(Sint16 x = min.x; x < max.x; x++)
	for(Sint16 y = min.y; y < max.y; y++)
	    if((distance=(tmp=SPoint(x,y)).distance(point)) < closestDistance)  {
		swap(closestDistance, distance);
		swap(closest, tmp);
	    }

    return closest;
}

SPoint ObjectClass::getClosestCentrePoint(SPoint objectPos) const
{
    return getCentrePoint();
}

int ObjectClass::getHealthColour() const
{
    float healthPercent = (float)m_health / (float)m_maxHealth;

    if (healthPercent >= 0.7)
	return COLOUR_LIGHTGREEN;
    else if (healthPercent >= HEAVILYDAMAGEDRATIO)
	return COLOUR_YELLOW;
    else
	return COLOUR_RED;
}

int ObjectClass::getViewRange() const
{
    if (m_owner->hasRadarOn() )
	return m_viewRange+2;
    else
	return m_viewRange;
}

void ObjectClass::handleDamage(Sint16 damage, ObjectPtr damager)
{
    if (!getStatus(STATUS_DESTROYED))
    {
	if (damage >= 0) 
	{
	    m_health -= damage;
	    if (m_health/(float)m_maxHealth < HEAVILYDAMAGEDRATIO)
	    {
		setStatus(STATUS_BADLYDAMAGED);
		if(m_health <= 0)
		{
		    m_health = 0;
		    destroy();
		}
	    }
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

bool ObjectClass::isVisible(int team) const
{
    if ((team >= 1) && (team <= MAX_PLAYERS))
	return m_visible[team-1];
    else
	return false;
}

bool ObjectClass::setDestination(ConstSPoint realDestination, Uint32 status)
{
    SPoint destination(realDestination/BLOCKSIZE);
    if (m_owner->getMap()->cellExists(destination) || ((destination.x == INVALID_POS) && (destination.y == INVALID_POS))) {
    	m_target.reset();
        m_destination = destination;
	m_realDestination = realDestination;
    	clearStatus(STATUS_MOVING | STATUS_MOVING | STATUS_DEFAULT | STATUS_ATTACKING);
	setStatus(status);
	return true;
    }

    return false;
}

bool ObjectClass::attack() {
    bool inRange = false;
    if(getStatus(STATUS_ATTACKING)) {
	if(m_weapons.empty())
    	    clearStatus(STATUS_ATTACKING);
	else {

	    for(std::vector<WeaponPtr>::const_iterator weapon = m_weapons.begin(); weapon != m_weapons.end(); weapon++) {
		if(getRealPosition().distance(m_realDestination) <= (*weapon)->getRange()*BLOCKSIZE) {
		    inRange = true;
		    if((*weapon)->loaded()) {
			WeaponPtr missile(new WeaponClass(*(*weapon).get()));
			GameMan::Instance()->addObject(missile);
			missile->setShooter(GameMan::Instance()->getObject(getObjectID()));
			missile->setDestination(m_realDestination);
		    }
		}
	    }
	    if(inRange && getStatus(STATUS_MOVING)) {
		clearStatus(STATUS_MOVING);
		setStatus(STATUS_JUSTSTOPPEDMOVING);
		setStatus(STATUS_NEXTSPOTFOUND);
	    }
	}
    }
    return inRange;
}

void ObjectClass::setPosition(SPoint pos)
{
    if (m_owner->getMap()->cellExists(pos) && pos != SPoint(INVALID_POS, INVALID_POS))
    {
	Rect::setPosition(pos);

	m_realPosition = pos * BLOCKSIZE;
	if(getSize() < SPoint(BLOCKSIZE, BLOCKSIZE))
	    m_realPosition = getCentrePoint();
	assignToMap(pos);
    }
}

void ObjectClass::setRealPosition(SPoint realPos)
{
    SPoint pos(realPos/BLOCKSIZE);
    if (m_owner->getMap()->cellExists(pos) && pos != SPoint(INVALID_POS, INVALID_POS))
    {
	Rect::setPosition(pos);

	m_realPosition = realPos;
	assignToMap(pos);
    }
}

void ObjectClass::setVisible(bool status, int team)
{
    if(team < 0)
	for(int i = 0; i < MAX_PLAYERS; i++)
	    m_visible[i] = status;
    else if (team <= MAX_PLAYERS)
	m_visible[team] = status;
    else
	throw "invalid player!";
}

void ObjectClass::unassignFromMap(SPoint pos)
{
    if (m_owner->getMap()->cellExists(pos))
	m_owner->getMap()->getCell(pos)->unassignObject(m_objectID);
}

void ObjectClass::update(float dt)
{
    m_adjust = dt * (Settings::Instance()->GetGameSpeed() * 10);
    if(getStatus(STATUS_DESTROYED)) {
	unassignFromMap(getPosition());
    }
}
