#include "DataCache.h"
#include "Definitions.h"
#include "GCObject.h"
#include "PlayerClass.h"

#include "units/QuadClass.h"

QuadClass::QuadClass(PlayerClass* newOwner) : GroundUnit(newOwner)
{
    m_baseID = m_itemID = Unit_Quad;

    sprite* tmp = DataCache::Instance()->getSpriteInfo("quad");

    w = h = 16;
    m_speed = DataCache::Instance()->getSpriteParameter("units.quad.speed", 0.63);

    //m_armour = 6;
    m_maxHealth = tmp->health;
    m_health = m_maxHealth;
    //radius = 6;
    m_viewRange = tmp->viewRange;
    //m_weaponRange = QUAD_WEAPONRANGE;

    //m_numWeapons = 2;
    //m_bulletType[0] = Bullet_Gun;
    //m_primaryWeaponReloadTime = 100;

    m_pic = DataCache::Instance()->getGCObject("ObjPic_Quad")->getImage(HOUSETYPE(getOwner()->getColour()));

    //FIXME:What is the standard offset for units?
    //m_offset = UPoint((w - BLOCKSIZE)/2, (h - BLOCKSIZE)/2);

}

QuadClass::~QuadClass()
{

}

