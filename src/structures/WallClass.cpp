#include "DataCache.h"
#include "structures/WallClass.h"

WallClass::WallClass(PlayerClass* newOwner) : StructureClass(newOwner)
{
    m_itemID = Structure_Wall;
    w = h = 16;
    m_animFrames = 1;
//	m_drawnAngle = Structure_w8;
	m_maxHealth = 500;
	m_powerRequirement = 0;
	m_health = m_maxHealth;
	m_viewRange = 2;
	m_pic = DataCache::Instance()->getObjPic(ObjPic_Wall);
    m_offset = UPoint(0,0);                //cause it draws at top left, not middle

//    setTile(25);

}

WallClass::~WallClass()
{
}
