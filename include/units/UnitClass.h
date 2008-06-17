#ifndef DUNE_UNITCLASS_H
#define DUNE_UNITCLASS_H

#include "ObjectClass.h"
#include "PlayerClass.h"

//! Base class for all units
class UnitClass : public ObjectClass
{
  public:
    //! @name Constructor and destructor
    //@{
    UnitClass(PlayerClass* newOwner);
    virtual ~UnitClass();
    //@}

    bool canPass(UPoint pos);

    virtual void draw(Image * dest, SPoint off, SPoint view);
    virtual void destroy() {};
   	virtual void deploy(UPoint newPosition);
   	
    inline bool isAttacking() { return m_attacking; }
	inline bool isTracked() { return m_tracked; }
	inline bool isTurreted() { return m_turreted; }
	inline bool isMoving() { return m_moving; }

	void setAngle(int newAngle);

    virtual void update();
    virtual void setPosition(SPoint pos);
    
  protected:
    bool m_attacking,
         m_goingToRepairYard,
         m_justStoppedMoving,
         m_moving,
         m_nextSpotFound,
         m_pickedUp,
         m_turning,
         m_turreted,
         m_tracked;

    int	m_baseID,
        m_destAngle,
        deviationTimer,
        findTargetTimer,
        m_nextSpotAngle,
        noCloserPointCount,
        m_numWeapons,
        primaryWeaponReloadTime,
        primaryWeaponTimer,
        secondaryWeaponReloadTime,
        secondaryWeaponTimer,
        targetAngle;
    
    
    
};

typedef std::vector<UnitClass*> Units;

#endif // DUNE_UNITCLASS_H

