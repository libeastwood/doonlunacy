#include "DataCache.h"
#include "structures/IXClass.h"

IXClass::IXClass(PlayerClass* newOwner) : StructureClass(newOwner)
{
    m_isAnimating = true;
    w = 32;
    h = 32;
    m_maxHealth = 1000;
    m_powerRequirement = 40;
    m_health = m_maxHealth;
    m_viewRange = 3;

    m_pic = DataCache::Instance()->getObjPic(ObjPic_Barracks, HOUSETYPE(getOwner()->getColour()));
    
    m_offset = UPoint(0,0);
    m_firstAnimFrame = 2;
    m_lastAnimFrame = 3;
    m_itemID = Structure_Barracks;
}

IXClass::~IXClass()
{

}
