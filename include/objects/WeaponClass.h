#ifndef DUNE_OBJECTS_WEAPONCLASS_H
#define DUNE_OBJECTS_WEAPONCLASS_H

#include "Gfx.h"
#include "ObjectClass.h"
#include "PlayerClass.h"

class WeaponClass : public Rect
{
  public:
    WeaponClass(ObjectClass* newShooter, UPoint position, UPoint destination, int bulletType, bool air);
    ~WeaponClass();
	void draw(Image * img, SPoint off, SPoint view);
	void updatePosition(float dt);
	void destroy();
	
	inline bool isDirectShot() { return (m_source == m_destination); }
    inline int getItemID() { return m_bulletType; }
    inline float getSpeed() { return m_speed; }
    void setDrawnPos(SPoint off, SPoint view);
  private:
    //! Is bullet headig for an air unit?
    bool m_airAttack,
         m_destroyed,
         m_groundBlocked;
         
    int	m_damage,
        m_damagePiercing, //from 0 for none to 10 max
        m_damageRadius,
        m_drawnAngle,
        m_bulletType,//itemID,
        m_numDeathFrames,
        m_numFrames,
        m_xOffset,
        m_yOffset,
        m_frameTime,
        m_frameTimer;

    std::string m_deathFrame;

    float m_speed,
            m_angle,
            m_destAngle,
            m_angleLeft,
            m_angleRight,
            m_maxSpeed,
            m_acceleration,
            m_initialSpeed,
            m_xSpeed, m_initialXSpeed,
            m_ySpeed, m_initialYSpeed,
            m_turnSpeed,
            m_distanceTraveled;
            
    float m_adjust;

    PointFloat m_realPos;
    SPoint m_drawnPos;
    ImagePtr m_graphic;

	Uint32 m_deathSound;
	
    UPoint  m_destination,
            m_source;

    ObjectClass*    m_shooter;
    PlayerClass*    m_owner;
    ImagePtr        m_deathGraphic[5][5];

    
}; 

#endif // DUNE_OBJECTS_WEAPONCLASS_H
