#include "DataCache.h"
#include "structures/PalaceClass.h"

PalaceClass::PalaceClass(PlayerClass* newOwner) : StructureClass(newOwner)
{
    m_isAnimating = true;
    w = 48;
    h = 48;
    m_maxHealth = 2000;
    m_powerRequirement = 10;
    m_health = m_maxHealth;
    m_viewRange = 8;

    m_pic = DataCache::Instance()->getObjPic(ObjPic_Palace, HOUSETYPE(getOwner()->getColour()));
    
    m_offset = UPoint(0,0);
    m_firstAnimFrame = 2;
    m_lastAnimFrame = 3;
    m_itemID = Structure_Palace;
}

PalaceClass::~PalaceClass()
{

}
