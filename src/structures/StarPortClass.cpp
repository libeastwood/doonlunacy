#include "DataCache.h"
#include "structures/StarPortClass.h"

StarPortClass::StarPortClass(PlayerClass* newOwner) : StructureClass(newOwner)
{
    m_isAnimating = true;
    w = 48;
    h = 48;
    m_maxHealth = 2000;
    m_powerRequirement = 80;
    m_health = m_maxHealth;
    m_viewRange = 6;

    m_pic = DataCache::Instance()->getObjPic(ObjPic_StarPort, HOUSETYPE(getOwner()->getColour()));
    
    m_offset = UPoint(0,0);
    m_firstAnimFrame = 2;
    m_lastAnimFrame = 3;
    m_itemID = Structure_StarPort;
}

StarPortClass::~StarPortClass()
{

}
