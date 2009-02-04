#ifndef DUNE_OBJECTS_WEAPONCLASS_H
#define DUNE_OBJECTS_WEAPONCLASS_H

#include "objects/ObjectClass.h"
#include "PlayerClass.h"

class WeaponClass : public ObjectClass
{
  public:
    WeaponClass(ObjectClass* newShooter, std::string weaponName, UPoint position, UPoint destination, bool air);
    ~WeaponClass();
	virtual void draw(Image * img, SPoint off, SPoint view);
	virtual void updatePosition(float dt);
	virtual void destroy();
	virtual void update();

	
	inline bool isDirectShot() { return (m_source == m_destination); }
    void setDrawnPos(SPoint off, SPoint view);
  private:
    //! Is bullet headig for an air unit?
    bool m_airAttack,
         m_destroyed,
         m_groundBlocked;
         
    int	m_damage,
        m_damagePiercing, //from 0 for none to 10 max
        m_damageRadius,
        m_numDeathFrames,
        m_numFrames,
        m_curAnimFrame,
        m_frameTime,
        m_frameTimer;

    float   m_destAngle,
            m_angleLeft,
            m_angleRight,
            m_maxSpeed,
            m_acceleration,
            m_initialSpeed,
            m_xSpeed, m_initialXSpeed,
            m_ySpeed, m_initialYSpeed,
            m_distanceTraveled;
            
    float m_adjust;

	Uint32 m_deathSound;
	
    UPoint  m_source;

    ObjectClass*    m_shooter;
    ImagePtr        m_deathGraphic[5][5];

    
}; 

#endif // DUNE_OBJECTS_WEAPONCLASS_H
