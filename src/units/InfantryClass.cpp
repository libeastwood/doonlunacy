#include "units/InfantryClass.h"

#include "DataCache.h"
#include "GCObject.h"

InfantryClass::InfantryClass(PlayerClass* newOwner) : GroundUnit(newOwner)
{
	m_baseID = m_itemID = Unit_Infantry;
	m_infantry = true;

	m_deathFrame = "ObjPic_DeadInfantry";
	m_numDeathFrames = 2;

	m_armour = 1;
    m_maxHealth = 10;
	m_health = m_maxHealth;
	m_radius = 2;
	m_speed = 0.25;
	m_viewRange = 4;
	m_weaponRange = 6;

	m_numWeapons = 1;	
	//m_bulletType[0] = Bullet_Inf_Gun;
	m_primaryWeaponReloadTime = 55;
	
    m_pic = DataCache::Instance()->getGCObject("ObjPic_Infantry")->getImage(HOUSETYPE(getOwner()->getColour()));
	w = h= m_pic->getSurface()->h/3;	
}

InfantryClass::~InfantryClass()
{

}

