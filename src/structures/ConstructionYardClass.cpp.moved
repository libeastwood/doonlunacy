#include "DataCache.h"
#include "structures/ConstructionYardClass.h"

ConstructionYardClass::ConstructionYardClass(PlayerClass* newOwner) : StructureClass(newOwner)
{
    m_isAnimating = true;
    w = 32;
    h = 32;
    m_maxHealth = 800;
    m_powerRequirement = 20;
    m_health = m_maxHealth;
    m_viewRange = 4;

    m_pic = DataCache::Instance()->getObjPic(ObjPic_ConstructionYard, HOUSETYPE(getOwner()->getColour()));
    
    m_offset = UPoint(0,0);
    m_firstAnimFrame = 2;
    m_lastAnimFrame = 3;
    m_itemID = Structure_Barracks;
}

ConstructionYardClass::~ConstructionYardClass()
{

}
