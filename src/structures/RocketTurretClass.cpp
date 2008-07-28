#include "DataCache.h"
#include "structures/RocketTurretClass.h"

RocketTurretClass::RocketTurretClass(PlayerClass* newOwner) : StructureClass(newOwner)
{
    m_isAnimating = true;
    w = 16;
    h = 16;
    m_maxHealth = 700;
    m_powerRequirement = 15;
    m_health = m_maxHealth;
    m_viewRange = 7;
//    m_weaponDamage = 150; rocket turret should do more damage than gun turret, right?
    m_weaponRange = 14;

    m_pic = DataCache::Instance()->getObjPic(ObjPic_RocketTurret, HOUSETYPE(getOwner()->getColour()));
    
    m_offset = UPoint(0,0);
    m_firstAnimFrame = 6;
    m_lastAnimFrame = 6;
//  m_currentAnimFrame = m_firstAnimFrame = m_lastAnimFrame = ((10-drawnAngle) % 8) + 2;
    m_itemID = Structure_RocketTurret;
}

RocketTurretClass::~RocketTurretClass()
{

}
