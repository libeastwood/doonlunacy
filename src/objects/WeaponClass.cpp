#include <boost/random.hpp>

#include "objects/WeaponClass.h"
#include "DataCache.h"
#include "Definitions.h"
#include "GameMan.h"
#include "GameData.h"
#include "houses.h"
#include "Log.h"
#include "MapClass.h"
#include "mmath.h"
#include "PythonObjects.h"


WeaponClass::WeaponClass(PlayerClass* newOwner, std::string weaponName, uint32_t attribute) :
    ObjectClass(newOwner, weaponName, attribute | OBJECT_WEAPON)
{
    m_deathSound = NONE;
    clearStatus();

    try {
	m_damage = python::extract<int>(m_pyObject.attr("damage"));
	m_damagePiercing = python::extract<int>(m_pyObject.attr("damagePiercing"));
	m_damageRadius =  python::extract<int>(m_pyObject.attr("damageRadius"));
	m_inaccuracy = python::extract<int>(m_pyObject.attr("inaccuracy"));
	m_range = python::extract<int>(m_pyObject.attr("range"));
	m_reloadTime = python::extract<int>(m_pyObject.attr("reloadTime"));
	if(!python::extract<bool>(m_pyObject.attr("groundBlocked")))
	    setAttribute(OBJECT_AIRUNIT);
    }
    catch(python::error_already_set const &)
    {
	LOG(LV_FATAL, m_objectType, "Error loading object: %s", getObjectName().c_str());
	PyErr_Print();
	exit(EXIT_FAILURE);
    }

    m_reloadTime += getRandom(-m_reloadTime/4, m_reloadTime/2);
    m_reloadTimer = getRandom(1, m_reloadTime/2);
}

bool WeaponClass::setDestination(ConstSPoint realDestination, uint32_t status) {
    if(ObjectClass::setDestination(realDestination + getRandom<int16_t>(-m_inaccuracy, m_inaccuracy), status)) {

	/*
	   if (getObjectName() == "Sonic")
	   {
	   int diffX = m_destination.x - realPosition.x,
	   diffY = m_destination.y - realPosition.y;

	   float ratio;
	   float square_root;

	   if ((diffX == 0) && (diffY == 0))
	   diffY = m_range * BLOCKSIZE;

	   square_root = sqrt((float)(diffX*diffX + diffY*diffY));
	   ratio = (m_range * BLOCKSIZE)/square_root;
	   m_destination.x = realPosition.x + (int)(((float)diffX)*ratio);
	   m_destination.y = realPosition.y + (int)(((float)diffY)*ratio);
	   }*/


	setRealPosition(m_shooter->getRealPosition());
	m_source = m_shooter->getCentrePoint();
	setSize(UPoint(m_graphic->getSize().y, m_graphic->getSize().y));

	m_destAngle = m_source.angle(m_realDestination);
	m_drawnAngle = (int)((float)m_numFrames*m_destAngle/360.0);
	m_angle = m_destAngle;
	LOG(LV_DEBUG, m_objectType, "Angle %f, drawn angle %d", m_angle, m_drawnAngle);

	m_speed = PointFloat(cos(m_destAngle * (M_PI / 180)), -sin(m_destAngle * (M_PI / 180))) * m_maxSpeed;
	m_distanceTraveled = 10000;
	return true;
    }
    return false;
}

void WeaponClass::dealDamage(ObjectPtr object, SPoint realPos) {
	Rect rect(realPos-m_damageRadius, (getSize()/2)+(m_damageRadius*2));
	float damageProp = object->coverage(rect);
	int16_t damage = ((m_damage + m_damagePiercing) * damageProp) - object->getArmor();
	object->handleDamage(damage, m_shooter);
}

void WeaponClass::draw(Image * dest, SPoint off, SPoint view)
{
    /*
    if (!getStatus(STATUS_DESTROYED) && getObjectName() == "Sonic")
    {
	ImagePtr tmp = m_graphic->getCopy();
	SDL_Surface *mask = tmp->getSurface();
	SDL_Surface* screen = dest->getSurface();
	SDL_Surface* graphic = m_graphic->getSurface();
	if (mask->format->BitsPerPixel == 8)
	{
	    if ((!SDL_MUSTLOCK(screen) || (SDL_LockSurface(screen) == 0))
		    && (!SDL_MUSTLOCK(mask) || (SDL_LockSurface(mask) == 0))
		    && (!SDL_MUSTLOCK(graphic) || (SDL_LockSurface(graphic) == 0)))
	    {
		unsigned char	*maskPixels = (unsigned char*)mask->pixels,
				*screenPixels = (unsigned char*)screen->pixels,
				*surfacePixels = (unsigned char*)graphic->pixels;
		int	i,j, x,y, maxX = 10;//gameBarPos.x;
		for (i = 0; i < w; i++)
		    for (j = 0; j < h; j++)
		    {
			if (maskPixels[i + j*mask->pitch] == 0)	//not masked, direct copy
			{
			    x = i;
			    y = j;
			}
			else
			{
			    x = i + getRandomInt(-3, 3);
			    y = j + getRandomInt(-3, 3);
			}

			if (m_drawnPos.x < 0)
			    m_drawnPos.x = 0;
			else if (m_drawnPos.x >= maxX)
			    m_drawnPos.x = maxX - 1;
			if (x < 0)
			    x = 0;
			else if (m_drawnPos.x + x >= maxX)
			    x = maxX - m_drawnPos.x - 1;

			if (m_drawnPos.y < 0)
			    m_drawnPos.y = 0;
			else if (m_drawnPos.y >= screen->h)
			    m_drawnPos.y = screen->h - 1;
			if (y < 0)
			    y = 0;
			else if (m_drawnPos.y + y >= screen->h)
			    x = screen->h - m_drawnPos.y - 1;

			surfacePixels[i + j*graphic->pitch] = screenPixels[m_drawnPos.x + x + (m_drawnPos.y + y)*screen->pitch];
		    }

		if (SDL_MUSTLOCK(graphic))
		    SDL_UnlockSurface(graphic);
		if (SDL_MUSTLOCK(mask))
		    SDL_UnlockSurface(mask);
		if (SDL_MUSTLOCK(screen))
		    SDL_UnlockSurface(screen);
	    }
	}
    }//end of if sonic
    */
    ObjectClass::draw(dest, off, view);

}

void WeaponClass::update(float dt)
{
    ObjectClass::update(dt);
    MapClass* map = GameMan::Instance()->GetMap();

    if (m_shooter && m_shooter->getStatus(STATUS_DESTROYED))
	m_shooter.reset();

    if (!getStatus(STATUS_DESTROYED))
    {
	UPoint oldLocation(getPosition());
	LOG(LV_DEBUG, m_objectType, "Old location was %S", &((ConstString)getPosition()));

	setRealPosition(m_realPosition + m_speed * m_adjust);  //keep the bullet moving by its current speeds

	if ((x < -5) || (x >= map->w + 5) || (y < -5) || (y >= map->h + 5))
	    m_frameTimer = 0;   //its off the screen, kill it
	else
	{
	    float distance = m_realDestination.distance(getRealPosition());
	    if(distance < m_distanceTraveled)
		m_distanceTraveled = distance;
	    else 
	    {
		setRealPosition(m_realDestination);
		destroy();
	    }
	    /*
	    if (getObjectName() == "Sonic")
	    {
		if (getPosition() != oldLocation.x)
		{
		    UPoint realPos = UPoint((short)m_realPosition.x, (short)m_realPos.y);
		    map->damage(m_shooter, m_owner, realPos, getObjectName(), m_damage, m_damagePiercing, m_damageRadius, false);
		}
	    }
	    else */
	    if (map->cellExists(getPosition()) && map->getCell(getPosition())->hasAGroundObject() && map->getCell(getPosition())->getGroundObject()->hasAttribute(OBJECT_STRUCTURE))
	    {
		if (!hasAttribute(OBJECT_AIRUNIT))
		    destroy();

	    }
	}
    }
    //	animate();
}

bool WeaponClass::destroy()
{
    if (ObjectClass::destroy()) {
	/* Here we deal damage to explosion area relative to it's size */
    	MapClass* map = GameMan::Instance()->GetMap();
	UPoint destPoint((UPoint(getRealPosition()) - ((m_explosionSize/2) * BLOCKSIZE)));

	for(int x = 0; x < m_explosionSize; x++, destPoint.x += BLOCKSIZE)
	    for(int y = 0; y < m_explosionSize; y++)
		if ((m_explosionSize <= 2) || ((x != 0) && (x != (m_explosionSize-1))) || ((y != 0) && (y != (m_explosionSize-1))))
		    map->damage(this, destPoint + UPoint(0, y*BLOCKSIZE));
	return true;
    }
    return false;
}
