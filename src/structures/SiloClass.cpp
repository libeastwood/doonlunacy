#include "structures/SiloClass.h"

#include "DataCache.h"
#include "GCObject.h"

SiloClass::SiloClass(PlayerClass* newOwner) : StructureClass(newOwner)
{
    m_isAnimating = true;
    w = 32;
    h = 32;
	m_maxHealth = 100;
	m_powerRequirement = 5;
	m_health = m_maxHealth;
	m_viewRange = 3;

    m_pic = DataCache::Instance()->getGCObject("ObjPic_Silo")->getImage(HOUSETYPE(getOwner()->getColour()));
    
    m_offset = UPoint(0,0);
    m_firstAnimFrame = 2;
    m_lastAnimFrame = 3;
    m_itemID = Structure_Silo;
}

SiloClass::~SiloClass()
{

}
