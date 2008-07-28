#include "DataCache.h"
#include "structures/HighTechFactoryClass.h"

HighTechFactoryClass::HighTechFactoryClass(PlayerClass* newOwner) : StructureClass(newOwner)
{
    m_isAnimating = true;
    w = 48;
    h = 32;
    m_maxHealth = 1000;
    m_powerRequirement = 35;
    m_health = m_maxHealth;
    m_viewRange = 5;

    m_pic = DataCache::Instance()->getObjPic(ObjPic_HighTechFactory, HOUSETYPE(getOwner()->getColour()));
    
    m_offset = UPoint(0,0);
    m_firstAnimFrame = 2;
    m_lastAnimFrame = 3;
    m_itemID = Structure_HighTechFactory;
}

HighTechFactoryClass::~HighTechFactoryClass()
{

}