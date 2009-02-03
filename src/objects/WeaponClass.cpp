#include <math.h>

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
    ObjectClass(m_shooter ? NULL : m_shooter->getOwner(), weaponName)
{
    DataCache* cache = DataCache::Instance();
    sprite *tmp;


    m_groundBlocked = false;
    m_airAttack = air;

    int inaccuracy;

    m_destroyed = false;
    m_shooter = newShooter;
    
    m_deathSound = NONE;

    tmp = cache->getSpriteInfo(m_objectName);
    try {
	    python::dict object = (python::dict)((python::object)tmp->pyObject).attr("__dict__");
            m_damagePiercing = python::extract<int>(object["damagePiercing"]);
            m_damageRadius = python::extract<int>(object["damageRadius"]);
            m_groundBlocked = python::extract<bool>(object["groundBlocked"]);
            inaccuracy = python::extract<int>(object["inaccuracy"]);
            m_damage = python::extract<int>(object["damage"]);
            m_speed = python::extract<float>(object["speed"]);
            m_numDeathFrames = python::extract<int>(object["damagePiercing"]);
            m_deathFrame = python::extract<std::string>(object["deathFrame"]);
            m_numFrames = python::extract<int>(object["numFrames"]);
    }
    catch(python::error_already_set const &)
    {
        LOG_FATAL("WeapontClass", "Error loading object: %s", m_objectName.c_str());
        PyErr_Print();
        exit(1);
    }

    m_destination.x = realDestination.x + getRandomInt(-inaccuracy, inaccuracy);
    m_destination.y = realDestination.y + getRandomInt(-inaccuracy, inaccuracy);
    
    if (weaponName == "Bullet_Sonic")
    {
        int diffX = m_destination.x - realPosition.x,
            diffY = m_destination.y - realPosition.y;

        float ratio;
        float square_root;

        int SonicTankWeaponRange;
        SonicTankWeaponRange = cache->getSpriteParameter("units.sonic_tank.weapon_range", 1);
        
        if ((diffX == 0) && (diffY == 0))
            diffY = SonicTankWeaponRange * BLOCKSIZE;

        square_root = sqrt((float)(diffX*diffX + diffY*diffY));
        ratio = (SonicTankWeaponRange * BLOCKSIZE)/square_root;
        m_destination.x = realPosition.x + (int)(((float)diffX)*ratio);
        m_destination.y = realPosition.y + (int)(((float)diffY)*ratio);
    }

    m_realPos = UPoint((float)realPosition.x, (float)realPosition.y);
    m_source = realPosition;
    x = realPosition.x/BLOCKSIZE;
    y = realPosition.y/BLOCKSIZE;
    h  = w = m_graphic->getSize().y;
	
    m_destAngle = dest_angle(realPosition, m_destination);
    m_drawnAngle = (int)((float)m_numFrames*m_destAngle/256.0);
    m_angle = m_destAngle;
    LOG_INFO("WeaponClass", "Angle %f, drawn angle %d", m_angle, m_drawnAngle);
    m_frameTime = 5;
    m_frameTimer = -1;

    m_xOffset = 8;                    //this is where it actually draws the graphic
    m_yOffset = 8;                    //cause it will draw from top left, not middle

    m_xSpeed = m_speed * cos(m_destAngle * conv2char);
    m_ySpeed = m_speed * -sin(m_destAngle * conv2char);
}

WeaponClass::~WeaponClass()
{

}

void WeaponClass::setDrawnPos(SPoint off, SPoint view)
{
    m_drawnPos.x = off.x + m_realPos.x - view.x * BLOCKSIZE - w / 2;
    m_drawnPos.y = off.y + m_realPos.y - view.y * BLOCKSIZE - h / 2;
}

void WeaponClass::draw(Image * dest, SPoint off, SPoint view)
{
    setDrawnPos(off, view);
    Rect source;
	
	if (!m_destroyed)
	{
		source.x = 0;

		if (m_numFrames > 1)
			source.x = m_drawnAngle * w;

		source.y = 0;
        
        
        
		if (m_bulletType == Bullet_Sonic)
		{
			ImagePtr tmp = DataCache::Instance()->getGCObject("ObjPic_Bullet_Sonic")->getImage((m_owner == NULL) ? (HOUSETYPE)HOUSE_HARKONNEN : (HOUSETYPE)m_owner->getHouse());

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
        //Rect testSource(0,0, 16, 16);
        //m_graphic->blitTo(dest, testSource, UPoint(0,0));
        source.w = 16;
        source.h = 16;
        m_graphic->blitTo(dest, source, m_drawnPos);
	}
	else if (m_numDeathFrames > 1)
	{
        UPoint destPoint;
		source.y = 0;

		if (m_bulletType == Bullet_LargeRocket)
		{
            UPoint destPoint;
			source.w = w;
			source.h = h;
			//FIXME: source.x = source.w * m_deathFrame;
			source.x = source.w * 1;

			for(int i = 0; i < 5; i++)
			{
    			for(int j = 0; j < 5; j++)
    			{
        			if (((i != 0) && (i != 4)) || ((j != 0) && (j != 4)))
        			{
        				destPoint.x = m_drawnPos.x + (i - 2)*BLOCKSIZE - BLOCKSIZE/2;
        				destPoint.y = m_drawnPos.y + (j - 2)*BLOCKSIZE - BLOCKSIZE/2;
        				m_deathGraphic[i][j]->blitTo(dest, source, destPoint);
        			}
			    }
			}
		}
		else
		{
			source.w = w;
			source.h = h;
//FIXME:			source.x = source.w * m_deathFrame;
			source.x = source.w * 1;
			

//			if (m_bulletType == Bullet_DRocket)
//				SDL_SetPalette(graphic, SDL_LOGPAL, &palette->colors[houseColour[owner->getHouse()]], COLOUR_HARKONNEN, 7);
			m_graphic->blitTo(dest, source, m_drawnPos);
		}
	}
	else
		m_frameTimer = 0;

}

void WeaponClass::updatePosition(float dt)
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
			else if (getItemID() == Bullet_Sonic)
			{
				if (getPosition() != oldLocation.x)
				{
					UPoint realPos = UPoint((short)m_realPos.x, (short)m_realPos.y);
					map->damage(m_shooter, m_owner, realPos, getItemID(), m_damage, m_damagePiercing, m_damageRadius, false);
				}
			}
			else if (map->cellExists(UPoint(x,y)) && map->getCell(UPoint(x,y))->hasAGroundObject() && map->getCell(UPoint(x,y))->getGroundObject()->isAStructure())
			{
				if (m_groundBlocked)
					destroy();
			}
		}
	}
//FIXME:
	/*
	if (m_frameTimer > 0)   //death frame has started
	{
		if (m_frameTimer == 1)
		{
			m_deathFrame++;
			if (m_deathFrame < m_numDeathFrames)
				m_frameTimer = m_frameTime;
		}
		m_frameTimer--;
	}
	*/
}

void WeaponClass::destroy()
{
    DataCache* cache = DataCache::Instance();
    MapClass* map = GameMan::Instance()->GetMap();
    
	if (!m_destroyed)
	{
		UPoint realPos = UPoint((short)m_realPos.x, (short)m_realPos.y);

		if (m_bulletType == Bullet_LargeRocket)
		{
			int i, j;
			for(i = 0; i < 5; i++)
			for(j = 0; j < 5; j++)
			if (((i != 0) && (i != 4)) || ((j != 0) && (j != 4)))
			{
//FIXME:				m_deathGraphic[i][j] = cache->getObjPic((ObjPic_enum)getRandomOf(2,ObjPic_ExplosionLarge1,ObjPic_ExplosionLarge2),(m_owner == NULL) ? (HOUSETYPE)HOUSE_HARKONNEN : (HOUSETYPE)m_owner->getHouse());;

				m_deathGraphic[i][j] = cache->getGCObject("ObjPic_Hit_ExplosionLarge1")->getImage((m_owner == NULL) ? (HOUSETYPE)HOUSE_HARKONNEN : (HOUSETYPE)m_owner->getHouse());
				realPos.x = (short)m_realPos.x + (i - 2)*BLOCKSIZE;
				realPos.y = (short)m_realPos.y + (j - 2)*BLOCKSIZE;

				map->damage(m_shooter, m_owner, realPos, m_bulletType, m_damage, m_damagePiercing, m_damageRadius, m_airAttack);
				//if (deathSound != NONE)
				//	soundPlayer->playSound(deathSound);
			}

			//imageW = deathGraphic[0][1]->w/numDeathFrames;
			//imageH = deathGraphic[0][1]->h;
			//xOffset = (imageW - BLOCKSIZE)/2;		    //this is where it actually draws the graphic
			//yOffset = (imageH - BLOCKSIZE)/2;		    //cause it draws at top left, not middle
			m_xOffset = 8;
			m_yOffset = 8;
		}
		else
		{
			//if (m_bulletType == Bullet_Sonic)
			//	SDL_FreeSurface(graphic);
			
			m_graphic = cache->getGCObject(m_deathFrame)->getImage((m_owner == NULL) ? (HOUSETYPE)HOUSE_HARKONNEN : (HOUSETYPE)m_owner->getHouse());
			//xOffset = (graphic->w/numDeathFrames)/2;		    //this is where it actually draws the graphic
			//yOffset = (graphic->h)/2;		    //cause it draws at top left, not middle
			m_xOffset = 8;
			m_yOffset = 8;
			
			map->damage(m_shooter, m_owner, realPos, m_bulletType, m_damage, m_damagePiercing, m_damageRadius, m_airAttack);
    		//if (deathSound != NONE)
			//	soundPlayer->playSound(deathSound);
		}

//FIXME:		m_deathFrame = 0;
		m_deathFrame = "";
		m_destroyed = true;
		m_frameTimer = m_frameTime;
	}
}
