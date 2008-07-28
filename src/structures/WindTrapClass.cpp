#include "DataCache.h"
#include "structures/WindTrapClass.h"

WindTrapClass::WindTrapClass(PlayerClass* newOwner) : StructureClass(newOwner)
{
    m_isAnimating = true;
    w = 32;
    h = 32;
    m_maxHealth = 1000;
    m_powerRequirement = 0;
//    m_powerSupply = 100;
    m_health = m_maxHealth;
    m_viewRange = 3;

    m_pic = DataCache::Instance()->getObjPic(ObjPic_WindTrap, HOUSETYPE(getOwner()->getColour()));
    
    m_offset = UPoint(0,0);
    m_firstAnimFrame = 2;
    m_lastAnimFrame = 3;
    m_itemID = Structure_WindTrap;
}

WindTrapClass::~WindTrapClass()
{

}
