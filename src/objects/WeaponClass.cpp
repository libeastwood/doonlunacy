#include "objects/WeaponClass.h"
#include "DataCache.h"
#include "Definitions.h"
#include "GameMan.h"
#include "GCObject.h"
#include "houses.h"
#include "Log.h"
#include "MapClass.h"
#include "mmath.h"


WeaponClass::WeaponClass(ObjectClass* newShooter, std::string weaponName, UPoint realPosition, UPoint realDestination, bool air) :
    ObjectClass(newShooter ? newShooter->getOwner() : NULL, weaponName)
{
    DataCache* cache = DataCache::Instance();

    m_attributes |= OBJECT_WEAPON;

    m_groundBlocked = false;
    m_airAttack = air;

    int inaccuracy;

    m_shooter = newShooter;

    m_deathSound = NONE;

    try {
	m_damage = cache->getPyObjectAttribute<int>(m_objectName, "damage");
	m_damagePiercing = cache->getPyObjectAttribute<int>(m_objectName, "damagePiercing");
	m_damageRadius =  cache->getPyObjectAttribute<int>(m_objectName, "damageRadius");
	m_groundBlocked =  cache->getPyObjectAttribute<int>(m_objectName, "groundBlocked");
	inaccuracy = cache->getPyObjectAttribute<int>(m_objectName, "inaccuracy");
	m_range = cache->getPyObjectAttribute<int>(m_objectName, "range");
    }
    catch(python::error_already_set const &)
    {
	LOG_FATAL("WeapontClass", "Error loading object: %s", m_objectName.c_str());
	PyErr_Print();
	exit(1);
    }

    m_destination.x = realDestination.x + getRandomInt(-inaccuracy, inaccuracy);
    m_destination.y = realDestination.y + getRandomInt(-inaccuracy, inaccuracy);

    if (m_objectName == "Sonic")
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
    }

    m_realPos = PointFloat(realPosition.x, realPosition.y);
    m_source = realPosition;
    x = realPosition.x/BLOCKSIZE;
    y = realPosition.y/BLOCKSIZE;
    h = w = m_graphic->getSize().y;

    m_destAngle = dest_angle(realPosition, m_destination);
    m_drawnAngle = (int)((float)m_numFrames*m_destAngle/256.0);
    m_angle = m_destAngle;
    LOG_INFO("WeaponClass", "Angle %f, drawn angle %d", m_angle, m_drawnAngle);

    m_xSpeed = m_speed * cos(m_destAngle * conv2char);
    m_ySpeed = m_speed * -sin(m_destAngle * conv2char);
}

WeaponClass::~WeaponClass()
{

}

void WeaponClass::draw(Image * dest, SPoint off, SPoint view)
{
    if (!m_destroyed && m_objectName == "Sonic")
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
    ObjectClass::draw(dest, off, view);
}

void WeaponClass::update(float dt)
{
    MapClass* map = GameMan::Instance()->GetMap();
    m_adjust = dt * (Settings::Instance()->GetGameSpeed() * 10);

    if (m_shooter && m_shooter->wasDestroyed())
	m_shooter = NULL;

    if (!m_destroyed)
    {
	UPoint oldLocation = UPoint(x,y);
	LOG_INFO("WeaponClass", "Old location was %d-%d", x,y);

	m_realPos.x += m_xSpeed * m_adjust;  //keep the bullet moving by its current speeds
	m_realPos.y += m_ySpeed * m_adjust;
	x = (short)(m_realPos.x/BLOCKSIZE);
	y = (short)(m_realPos.y/BLOCKSIZE);

	if ((x < -5) || (x >= map->w + 5) || (y < -5) || (y >= map->h + 5))
	    m_frameTimer = 0;   //its off the screen, kill it
	else
	{
	    if (distance_from(m_source.x, m_source.y, m_realPos.x, m_realPos.x) >= distance_from(m_source, m_destination))
	    {
		m_realPos = m_destination;
		destroy();
	    }
	    else if (m_objectName == "Sonic")
	    {
		if (getPosition() != oldLocation.x)
		{
		    UPoint realPos = UPoint((short)m_realPos.x, (short)m_realPos.y);
		    map->damage(m_shooter, m_owner, realPos, m_objectName, m_damage, m_damagePiercing, m_damageRadius, false);
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
    ObjectClass::destroy();
    MapClass* map = GameMan::Instance()->GetMap();

    if (!m_destroyed)
    {
	UPoint realPos = UPoint((short)m_realPos.x, (short)m_realPos.y);
	for(int i = 0; i < m_explosionSize; i++)
	    for(int j = 0; j < m_explosionSize; j++)
		if (( m_explosionSize <= 2) || ((i != 0) && (i != (m_explosionSize-1))) || ((j != 0) && (j != (m_explosionSize-1))))
		{
		    realPos.x = m_drawnPos.x + (i - (m_explosionSize/2))*BLOCKSIZE - BLOCKSIZE/2;
		    realPos.y = m_drawnPos.y + (j - (m_explosionSize/2))*BLOCKSIZE - BLOCKSIZE/2;

		    map->damage(m_shooter, m_owner, realPos, m_objectName, m_damage, m_damagePiercing, m_damageRadius, m_airAttack);
		}
    }
}
