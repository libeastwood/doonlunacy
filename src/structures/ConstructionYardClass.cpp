#include "structures/ConstructionYardClass.h"

#include "DataCache.h"
#include "GCObject.h"

ConstructionYardClass::ConstructionYardClass(PlayerClass* newOwner) : StructureClass(newOwner)
{
    m_itemID = Structure_ConstructionYard;

    m_isAnimating = true;
    w = 32;
    h = 32;
	m_maxHealth = 100;
	m_powerRequirement = 5;
	m_health = m_maxHealth;
	m_viewRange = 3;

    m_pic = DataCache::Instance()->getGCObject("ObjPic_ConstructionYard")->getImage(HOUSETYPE(getOwner()->getColour()));
    
    m_offset = UPoint(0,0);
    m_firstAnimFrame = 2;
    m_lastAnimFrame = 3;
}

ConstructionYardClass::~ConstructionYardClass()
{

}
