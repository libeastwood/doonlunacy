#include "DataCache.h"
#include "structures/SiloClass.h"

SiloClass::SiloClass(PlayerClass* newOwner) : StructureClass(newOwner)
{
    m_isAnimating = true;
    DataCache * cache = DataCache::Instance();
    w = 2;
    h = 2;
	m_maxHealth = 100;
	m_powerRequirement = 5;
	m_health = m_maxHealth;
	m_viewRange = 3;

	m_pic = DataCache::Instance()->getObjPic(ObjPic_Silo, HOUSETYPE(getOwner()->getColour()));
    
    m_offset = UPoint(0,0);
    m_firstAnimFrame = 2;
    m_lastAnimFrame = 3;
    m_itemID = Structure_Silo;
}

SiloClass::~SiloClass()
{

}
