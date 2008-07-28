#include "DataCache.h"
#include "structures/RadarClass.h"

RadarClass::RadarClass(PlayerClass* newOwner) : StructureClass(newOwner)
{
    m_isAnimating = true;
    w = 32;
    h = 32;
    m_maxHealth = 1000;
    m_powerRequirement = 30;
    m_health = m_maxHealth;
    m_viewRange = 9;

    m_pic = DataCache::Instance()->getObjPic(ObjPic_Radar, HOUSETYPE(getOwner()->getColour()));
    
    m_offset = UPoint(0,0);
    m_firstAnimFrame = 2;
    m_lastAnimFrame = 5;
    m_itemID = Structure_Radar;
}

RadarClass::~RadarClass()
{

}
