#include "DataCache.h"
#include "structures/WORClass.h"

WORClass::WORClass(PlayerClass* newOwner) : StructureClass(newOwner)
{
    m_isAnimating = true;
    w = 32;
    h = 32;
    m_maxHealth = 1500;
    m_powerRequirement = 20;
    m_health = m_maxHealth;
    m_viewRange = 5;

    m_pic = DataCache::Instance()->getObjPic(ObjPic_WOR, HOUSETYPE(getOwner()->getColour()));
    
    m_offset = UPoint(0,0);
    m_firstAnimFrame = 2;
    m_lastAnimFrame = 3;
    m_itemID = Structure_WOR;
}

WORClass::~WORClass()
{

}
