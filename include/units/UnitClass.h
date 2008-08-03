#ifndef DUNE_UNITCLASS_H
#define DUNE_UNITCLASS_H

#include <list>
#include <queue>

#include "ObjectClass.h"
#include "PlayerClass.h"
#include "TerrainClass.h"
#include "PriorityQ.h"
typedef std::list <UPoint> Path;
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
    virtual void checkPos() = 0;
    
    virtual void draw(Image * dest, SPoint off, SPoint view);
    virtual void drawSelectionBox(Image* dest);
    
    virtual void destroy() {};
   	virtual void deploy(SPoint newPosition);
   	
    inline bool isAttacking() { return m_attacking; }
	inline bool isTracked() { return m_tracked; }
	inline bool isTurreted() { return m_turreted; }
	inline bool isMoving() { return m_moving; }

	void setAngle(int newAngle);

    virtual void update();
    virtual void setDestination(SPoint destination);
    virtual void setPosition(SPoint pos);
    virtual void setTarget(ObjectClass* newTarget);
        
  protected:
    void nodePushSuccesors(PriorityQ* open, TerrainClass* parent_node);
	bool AStarSearch();
	
   	virtual void move();

	virtual void navigate();

    void setDrawnPos(SPoint off, SPoint view);
    virtual void setSpeeds();
    virtual void turn();
   	void turnLeft();
	void turnRight();
    
    bool m_attacking,
         m_goingToRepairYard,
         m_justStoppedMoving,
         m_moving,
         m_nextSpotFound,
         m_pickedUp,
         m_turning,
         m_turreted,
         m_tracked;

    double  m_speed,
            m_speedCap,
            m_targetDistance,
            m_turnSpeed,
            m_xSpeed, 
            m_ySpeed;

    int	m_baseID,
        m_destAngle,
        m_deviationTimer,
        m_findTargetTimer,
        m_nextSpotAngle,
        m_noCloserPointCount,
        m_numWeapons,
        m_primaryWeaponReloadTime,
        m_primaryWeaponTimer,
        m_secondaryWeaponReloadTime,
        m_secondaryWeaponTimer,
        m_targetAngle;

    SPoint  m_nextSpot,
            m_guardPoint;        

    Path m_pathList;
};

typedef std::vector<UnitClass*> Units;

#endif // DUNE_UNITCLASS_H

