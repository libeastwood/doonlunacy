#ifndef DUNE_OBJECTS_WEAPONCLASS_H
#define DUNE_OBJECTS_WEAPONCLASS_H

#include "objects/ObjectClass.h"
#include "PlayerClass.h"

class WeaponClass : public ObjectClass
{
    public:
	WeaponClass(PlayerClass* newOwner, std::string weaponName, uint32_t attribute = 0);
	inline void setShooter(ObjectPtr shooter) { m_shooter = shooter; }
	virtual void draw(Image& dest, SPoint off, SPoint view);
	virtual void update(float dt);
	virtual bool destroy();
	virtual bool setDestination(ConstSPoint realDestination, uint32_t status = 0);
	inline bool isDirectShot() { return (m_source == m_destination); }

	inline int16_t getRange() { return m_range; }
	inline bool loaded() {
	    bool reload = --m_reloadTimer == 0;
	    if(reload)
		m_reloadTimer = m_reloadTime;
	    return reload;
	}

	void dealDamage(ObjectPtr object, SPoint realPos);


    private:
	int16_t	m_damage,
		m_damagePiercing, //from 0 for none to 10 max
		m_damageRadius,
		m_deathFrame,
		m_inaccuracy,
		m_range,
		m_reloadTime,
		m_reloadTimer;

	float   m_destAngle,
		m_angleLeft,
		m_angleRight,
		m_acceleration,
		m_initialSpeed,
		m_initialXSpeed,
		m_initialYSpeed,
		m_distanceTraveled;

	uint32_t m_deathSound;

	SPoint  m_source;

	ObjectPtr m_shooter;


}; 

#endif // DUNE_OBJECTS_WEAPONCLASS_H
