#ifndef DUNE_OBJECTS_UNITCLASS_H
#define DUNE_OBJECTS_UNITCLASS_H

#include <list>
#include <queue>
#include <string>

#include "objects/ObjectClass.h"
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
    UnitClass(PlayerClass* newOwner, std::string unitName, uint32_t attribute = 0);
    virtual ~UnitClass();
    //@}

    bool canPass(UPoint pos);
    //! Function used e.g. by air units to calculate their posision, etc.
    virtual void checkPos() = 0;
    
    virtual void draw(Image * dest, SPoint off, SPoint view);
    
    virtual void destroy();
   	virtual void deploy(SPoint newPosition);
   	
	inline bool isTracked() { return m_tracked; }
	inline bool isTurreted() { return m_turreted; }

	void setAngle(int newAngle);

    virtual void update(float dt);
    virtual void setDestination(SPoint destination, Uint32 status = 0);
    virtual void setPosition(SPoint pos);
    virtual void setSelected(bool value);

    void playConfirmSound();
    void playSelectSound();
        
  protected:
    void nodePushSuccesors(PriorityQ* open, TerrainClass* parent_node);
	bool AStarSearch();
	
	
	//! That's where the actual move of unit takes place
   	virtual void move();
    //! Find next spot to which to move unit and set all neede variables
	virtual void navigate();

    virtual void setSpeeds();
    virtual void turn();
   	void turnLeft();
	void turnRight();

	void targeting();
	void setGuardPoint(UPoint newGuardPoint);
	void setGuardPoint(int newX, int newY);
    bool m_justStoppedMoving,
         m_nextSpotFound,
         m_pickedUp,
         m_turning,
         m_turreted,
         m_tracked;

    float   m_speedCap,
            m_targetDistance,
            m_xSpeed, 
            m_ySpeed;

    int	m_baseID,
        m_destAngle,
        m_deviationTimer,
        m_findTargetTimer,
        m_nextSpotAngle,
        m_noCloserPointCount,
        m_targetAngle;
    
    SPoint  m_nextSpot,
            m_guardPoint;        

    Path m_pathList;
    std::vector<SoundPtr> m_confirmSound,
			  m_selectSound;
};

#endif // DUNE_OBJECTS_UNITCLASS_H
