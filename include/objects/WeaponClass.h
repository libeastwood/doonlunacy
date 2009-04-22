#ifndef DUNE_OBJECTS_WEAPONCLASS_H
#define DUNE_OBJECTS_WEAPONCLASS_H

#include "objects/ObjectClass.h"
#include "PlayerClass.h"

class WeaponClass : public ObjectClass
{
    public:
	WeaponClass(ObjectPtr newShooter, std::string weaponName, uint32_t attribute = 0);
	~WeaponClass();
	virtual void draw(Image *dest, SPoint off, SPoint view);
	virtual void update(float dt);
	virtual void destroy();
	virtual void setDestination(SPoint destination, Uint32 status = 0);
	inline bool isDirectShot() { return (m_source == m_destination); }

	inline int getRange() { return m_range; }
	inline bool loaded() {
	    bool reload = --m_reloadTimer == 0;
	    if(reload)
		m_reloadTimer = m_reloadTime;
	    return reload;
	}

    private:
	int	m_damage,
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

	Uint32 m_deathSound;

	UPoint  m_source;

	ObjectPtr m_shooter;


}; 

#endif // DUNE_OBJECTS_WEAPONCLASS_H
