#ifndef OBJECTCLASS_H
#define OBJECTCLASS_H

#define VIS_ALL -1

#include <vector>

#include "Gfx.h"
#include "PlayerClass.h"
/*!
 *  @brief Base class for all objects (buildins, units, bullets)
 *
 *  It's derived from Rect so map coordinates can be accessed by x,y,w,h
 *  @note There are several kinds of coordinates for each object:
 *        - drawn - coordinates on the screen where the object is drawn
 *        - real  - coordinates on the map in pixels e.g. cell 16*BLOCKSIZE(16)
 *        - normal - coordinates of a cell an object is in
 */

extern int lookDist[11];

class ObjectClass : public Rect
{

  public:
    //!
    //@{
    ObjectClass(PlayerClass* newOwner);
    virtual ~ObjectClass();
    //@}

	//
	// Common object functions
	//
	
    virtual void assignToMap(SPoint pos);
    virtual void unassignFromMap(SPoint pos);

	static ObjectClass* createObject(int ItemID,PlayerClass* Owner, Uint32 ObjectID = NONE);

    virtual void draw(Image * dest, SPoint off, SPoint view);
	void drawSmoke(UPoint pos);
    virtual void destroy() = 0;
    virtual void update() {};

    //! @name Setters and getters
	//@{
	
    inline void setActive(bool status) { m_active = status; }
    inline void setForced(bool status) { m_forced = status; }
	void setHealth(int newHealth);
    inline void setSelected(bool value) { m_selected = value; }
    inline void setRespondable(bool status) { m_respondable = status; }
	void setVisible(int team, bool status);

    virtual void setDestination(SPoint destination);
    virtual void setPosition(SPoint pos);
	
    inline bool isABuilder()    { return m_builder; }
    inline bool isAFlyingUnit() { return m_flyingUnit; }
    inline bool isAGroundUnit() { return m_groundUnit; }
    inline bool isAStructure()  { return m_structure; }
    inline bool isAUnit()       { return m_unit; }
    inline bool isInfantry()    { return m_infantry; }

    inline bool isActive()     { return m_active; }
    inline bool isRespondable() { return m_respondable; }
    inline bool isSelected() { return m_selected; }
	inline bool isVisible(int team);
    int getHealthColour();

	inline bool wasDestroyed() { return m_destroyed; }
	inline bool wasForced() { return m_forced; }

    inline int getItemID() { return m_itemID; }
	int getViewRange();

    inline Uint32 getObjectID() { return m_objectID; }
    inline void setObjectID(int newObjectID) { if (newObjectID >= 0) m_objectID = newObjectID; }
    
    inline UPoint getRealPos() { return m_realPos; }
    inline UPoint getPosition() { return UPoint(x,y); }


    bool isOnScreen(Rect rect);

    virtual UPoint getClosestPoint(UPoint objectLocation);

    inline PlayerClass* getOwner() { return m_owner; }
    inline void setOwner(PlayerClass* newOwner) { m_owner = newOwner; }
    //@}
	//
	// Attack related functions
	//
	
	bool canAttack(ObjectClass* object);
	void handleDamage(int damage, ObjectClass* damager);
	ObjectClass* findTarget();

  protected:
    ATTACKTYPE m_attackMode;

    bool m_active,
    //! Draw deathFrame if the building was destroyed, or remove unit from list and forget about it
         m_builder,
         m_destroyed,
         m_flyingUnit,
         m_groundUnit,
         m_infantry,
         m_respondable,
         m_selected,
         m_structure,
         m_unit,
	//! Specifies which players can see a given object
         m_visible[MAX_PLAYERS+1];

    /*!
     *  If set to true, animation frame will change in certain intervals.
     *  We don't want it in case of walls, turrets, etc.
     */
    bool m_badlyDamaged,
    //!can i do damage to stuff?
         m_canAttackStuff,
         m_forced,
         m_isAnimating,
         m_targetFriendly;


    double  m_angle,
            m_health;

    PointFloat m_realPos;

    //! Increments every time a sprite is drawn. Usually after 25 cycles animation frame is changed
    int m_animCounter;

    /*! Currently drawn frame
     *  @note 0 Just placed
     *        1 Destroyed
     *        2 Normal flag down
     *        3 Normal flag up
     *        4,5,6,... Refinery, Starport,etc. when a unit docks, brings spice, whatever
     *
     */
    int m_checkTimer,

        m_curAnimFrame,

        //! Total number of frames
        m_animFrames,

        m_drawnAngle,

        //! Type of current object e.g. Structure_Silo
        m_itemID,

        m_maxHealth,

        //! Used for fog of war and area exploring.
        m_viewRange,

        m_weaponRange;

    int m_deathFrame,

        m_numDeathFrames,
		m_guardRange;


    //! SharedPtr to image for current object
    ImagePtr m_pic;

    //! Pointer to map to which object is assigned.
    PlayerClass * m_owner;

    Uint32 m_objectID;

    UPoint m_destination,
           m_oldPosition;

    /*!
     *  Position on the screen where an object will be drawn. It depends on
     *  position of MapWidget, current view position and of course object's position.
     */
    UPoint m_drawnPos;

    UPoint m_offset;

    ObjectClass * m_target;
};

#endif //OBJECTCLASS_H
