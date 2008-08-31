#include "units/TrikeClass.h"

#include "DataCache.h"
#include "Definitions.h"
#include "GCObject.h"
#include "PlayerClass.h"

TrikeClass::TrikeClass(PlayerClass* newOwner) : GroundUnit(newOwner)
{
    DataCache* cache = DataCache::Instance();
    m_baseID = m_itemID = Unit_Trike;

    sprite* tmp = cache->getSpriteInfo("trike");

    w = h = 16;
    m_speed = cache->getSpriteParameter("units.trike.speed", 0.63);

    //m_armour = 6;
    m_maxHealth = cache->getSpriteParameter("units.trike.health", 350);
    m_health = m_maxHealth;
    //radius = 6;
    m_viewRange = tmp->viewRange;
    //m_weaponRange = QUAD_WEAPONRANGE;

    //m_numWeapons = 2;
    //m_bulletType[0] = Bullet_Gun;
    //m_primaryWeaponReloadTime = 100;

    m_pic = cache->getGCObject("ObjPic_Trike")->getImage(HOUSETYPE(getOwner()->getColour()));

    //FIXME:What is the standard offset for units?
    //m_offset = UPoint((w - BLOCKSIZE)/2, (h - BLOCKSIZE)/2);

}

TrikeClass::~TrikeClass()
{

}

