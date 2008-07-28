#include "DataCache.h"
#include "structures/RefineryClass.h"

RefineryClass::RefineryClass(PlayerClass* newOwner) : StructureClass(newOwner)
{
    m_isAnimating = true;
    w = 48;
    h = 32;
    m_maxHealth = 1500;
    m_powerRequirement = 30;
    m_health = m_maxHealth;
    m_viewRange = 6;

    m_pic = DataCache::Instance()->getObjPic(ObjPic_Refinery, HOUSETYPE(getOwner()->getColour()));
    
    m_offset = UPoint(0,0);
    m_firstAnimFrame = 2;
    m_lastAnimFrame = 3;
    m_itemID = Structure_Refinery;
}

RefineryClass::~RefineryClass()
{

}
