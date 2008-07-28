#include "DataCache.h"
#include "structures/GunTurretClass.h"

GunTurretClass::GunTurretClass(PlayerClass* newOwner) : StructureClass(newOwner)
{
    m_isAnimating = true;
    w = 16;
    h = 16;
    m_maxHealth = 700;
    m_powerRequirement = 10;
    m_health = m_maxHealth;
    m_viewRange = 5;
    // m_weaponDamage = 100;
    m_weaponRange = 5;

    m_pic = DataCache::Instance()->getObjPic(ObjPic_GunTurret, HOUSETYPE(getOwner()->getColour()));
    
    m_offset = UPoint(0,0);
    m_firstAnimFrame = 6;
    m_lastAnimFrame = 6;
//  m_currentAnimFrame = m_firstAnimFrame = m_lastAnimFrame = ((10-drawnAngle) % 8) + 2;
    m_itemID = Structure_GunTurret;
}

GunTurretClass::~GunTurretClass()
{

}
