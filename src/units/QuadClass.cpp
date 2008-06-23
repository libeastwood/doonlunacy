#include "DataCache.h"
#include "Definitions.h"
#include "PlayerClass.h"
#include "units/QuadClass.h"

QuadClass::QuadClass(PlayerClass* newOwner) : GroundUnit(newOwner)
{
    m_baseID = m_itemID = Unit_Quad;

    w = h = 16;
    m_speed = 0.64;

    //m_armour = 6;
    m_maxHealth = 350;
    m_health = m_maxHealth;
    //radius = 6;
    //m_viewRange = QUAD_VIEWRANGE;
    //m_weaponRange = QUAD_WEAPONRANGE;

    //m_numWeapons = 2;
    //m_bulletType[0] = Bullet_Gun;
    //m_primaryWeaponReloadTime = 100;

    m_pic = DataCache::Instance()->getObjPic(ObjPic_Quad, HOUSETYPE(getOwner()->getColour()));

    //FIXME:What is the standard offset for units?
    //m_offset = UPoint((w - BLOCKSIZE)/2, (h - BLOCKSIZE)/2);

}

QuadClass::~QuadClass()
{

}

