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


WeaponClass::WeaponClass(ObjectPtr newShooter, std::string weaponName, bool air, uint32_t attribute) :
    ObjectClass(newShooter->getOwner(), weaponName, attribute | OBJECT_WEAPON)
{
    m_groundBlocked = false;
    m_airAttack = air;

    m_shooter = newShooter;

    m_deathSound = NONE;

    try {
	m_damage = python::extract<int>(m_pyObject.attr("damage"));
	m_damagePiercing = python::extract<int>(m_pyObject.attr("damagePiercing"));
	m_damageRadius =  python::extract<int>(m_pyObject.attr("damageRadius"));
	m_groundBlocked =  python::extract<int>(m_pyObject.attr("groundBlocked"));
	m_inaccuracy = python::extract<int>(m_pyObject.attr("inaccuracy"));
	m_range = python::extract<int>(m_pyObject.attr("range"));
    }
    catch(python::error_already_set const &)
    {
	LOG_FATAL("WeapontClass", "Error loading object: %s", getObjectName().c_str());
	PyErr_Print();
	exit(EXIT_FAILURE);
    }

}

void WeaponClass::setDestination(SPoint destination, Uint32 status) {
    ObjectClass::setDestination(destination/BLOCKSIZE);
    m_destination = destination;
    m_destination += getRandomInt(-m_inaccuracy, m_inaccuracy);

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


    m_realPos = PointFloat(m_shooter->getRealPos());
    m_source = m_shooter->getRealPos();
    Rect::setPosition(m_shooter->getPosition());
    setSize(UPoint(m_graphic->getSize().y, m_graphic->getSize().y));

    m_destAngle = dest_angle(m_source, m_destination);
    m_drawnAngle = (int)((float)m_numFrames*m_destAngle/256.0);
    m_angle = m_destAngle;
    LOG_INFO("WeaponClass", "Angle %f, drawn angle %d", m_angle, m_drawnAngle);

    m_speed = PointFloat(m_maxSpeed * cos(m_destAngle * conv2char), m_maxSpeed * -sin(m_destAngle * conv2char));

}

WeaponClass::~WeaponClass()
{

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
	UPoint oldLocation = UPoint(x,y);
	LOG_DEBUG("WeaponClass", "Old location was %d-%d", x,y);

	m_realPos += m_speed * m_adjust;  //keep the bullet moving by its current speeds
	Rect::setPosition(m_realPos/BLOCKSIZE);

	if ((x < -5) || (x >= map->w + 5) || (y < -5) || (y >= map->h + 5))
	    m_frameTimer = 0;   //its off the screen, kill it
	else
	{
	    if (distance_from(m_source.x, m_source.y, m_realPos.x, m_realPos.x) >= distance_from(m_source, m_destination))
	    {
		m_realPos = m_destination;
		destroy();
	    }
	    else if (getObjectName() == "Sonic")
	    {
		if (getPosition() != oldLocation.x)
		{
		    UPoint realPos = UPoint((short)m_realPos.x, (short)m_realPos.y);
		    map->damage(m_shooter, m_owner, realPos, getObjectName(), m_damage, m_damagePiercing, m_damageRadius, false);
		}
	    }
	    else if (map->cellExists(UPoint(x,y)) && map->getCell(UPoint(x,y))->hasAGroundObject() && map->getCell(UPoint(x,y))->getGroundObject()->isAStructure())
	    {
		if (m_groundBlocked)
		    destroy();
	    }
	}
    }
    //	animate();
}

void WeaponClass::destroy()
{
    if (!getStatus(STATUS_DESTROYED))
    {
	/* Here we deal damage to explosion area according to it's size */
    	MapClass* map = GameMan::Instance()->GetMap();
	UPoint destPoint((UPoint(m_realPos) - ((m_explosionSize/2) * BLOCKSIZE)) - BLOCKSIZE/2);

	for(int x = 0; x < m_explosionSize; x++, destPoint.x += BLOCKSIZE)
	    for(int y = 0; y < m_explosionSize; y++)
		if ((m_explosionSize <= 2) || ((x != 0) && (x != (m_explosionSize-1))) || ((y != 0) && (y != (m_explosionSize-1))))
		    map->damage(m_shooter, m_owner, destPoint + UPoint(0, y*BLOCKSIZE), getObjectName(), m_damage, m_damagePiercing, m_damageRadius, m_airAttack);
    }
    ObjectClass::destroy();

}
