#include "units/SoldierClass.h"

#include "DataCache.h"
#include "GCObject.h"

SoldierClass::SoldierClass(PlayerClass* newOwner, std::string unitName) : GroundUnit(newOwner, unitName)
{
	m_infantry = true;

	m_deathFrame = "ObjPic_DeadSoldier";
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
	
	w = m_pic->getSize().x / 4;
	h = m_pic->getSize().y / 3;
}

SoldierClass::~SoldierClass()
{

}

