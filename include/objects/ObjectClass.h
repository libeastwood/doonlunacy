#ifndef DUNE_OBJECTS_OBJECTCLASS_H
#define DUNE_OBJECTS_OBJECTCLASS_H

#define VIS_ALL -1

#include "Rect.h"
#include "PlayerClass.h"

#include <string>
#include <vector>

enum attribute {
    OBJECT_CLASS = 0,
    OBJECT_UNIT = 1 << 0,
    OBJECT_GROUNDUNIT = 1 << 1,
    OBJECT_INFANTRY = 1 << 2,
    OBJECT_AIRUNIT = 1 << 3,
    OBJECT_STRUCTURE = 1 << 4,
    OBJECT_BUILDER = 1 << 5,
    OBJECT_WEAPON = 1 << 6
};

enum status {
    STATUS_NONE = 0,
    STATUS_MOVING = 1 << 0,
    STATUS_ATTACKING = 1 << 1,
    STATUS_REPAIR = 1 << 2,
    STATUS_DESTROYED = 1 << 3
};

/*!
 *  @brief Base class for all objects (buildins, units, bullets)
 *
 *  It's derived from Rect so map coordinates can be accessed by x,y,w,h
 *  @note There are several kinds of coordinates for each object:
 *        - drawn - coordinates on the screen where the object is drawn
 *        - real  - coordinates on the map in pixels e.g. cell 16*BLOCKSIZE(16)
 *        - normal - coordinates of a cell an object is in
 */

#define SMOKEDELAY 30

class ObjectClass : public Rect
{

  public:
    //!
    //@{
    ObjectClass(PlayerClass* newOwner, std::string objectName, Uint32 attribute = 0);
    virtual ~ObjectClass();
    //@}

	//
	// Common object functions
	//
	
    virtual void assignToMap(SPoint pos);
    virtual void unassignFromMap(SPoint pos);
    bool clearObject() { return ((m_status & STATUS_DESTROYED) && (m_frameTimer == 0)); }
    
    void setDrawnPos(SPoint off, SPoint view);
    virtual void draw(Image * dest, SPoint off, SPoint view);
    void drawSelectionBox(Image* dest);

    void drawSmoke(Image *dest);
    virtual void destroy();
    virtual void update(float dt);
    virtual void animate();
    virtual void doDeath(Image *dest);

    //! @name Setters and getters
	//@{
	
    inline void setActive(bool status) { m_active = status; }
    inline void setForced(bool status) { m_forced = status; }
	void setHealth(int newHealth);
    virtual inline void setSelected(bool value) { m_selected = value; }
    inline void setRespondable(bool status) { m_respondable = status; }
	void setVisible(int team, bool status);

    virtual void setDestination(SPoint destination, Uint32 status = 0);
    virtual void setPosition(SPoint pos);
	
    inline bool isABuilder()    { return m_attributes & OBJECT_BUILDER; }
    inline bool isAFlyingUnit() { return m_attributes & OBJECT_AIRUNIT; }
    inline bool isAGroundUnit() { return m_attributes & OBJECT_GROUNDUNIT; }
    inline bool isAStructure()  { return m_attributes & OBJECT_STRUCTURE; }
    inline bool isAUnit()       { return m_attributes & OBJECT_UNIT; }
    inline bool isInfantry()    { return m_attributes & OBJECT_INFANTRY; }
    inline bool isWeapon()    { return m_attributes & OBJECT_WEAPON; }
    inline Uint32 getAttributes() { return m_attributes; }

    inline bool isActive()     { return m_active; }
    inline bool isRespondable() { return m_respondable; }
    inline bool isSelected() { return m_selected; }
    //! Checks if a given team can see this object
	inline bool isVisible(int team);
    int getHealthColour();

    inline bool wasForced() { return m_forced; }

    inline std::string getObjectName() { return m_objectName; }
    int getViewRange();

    inline Uint32 getObjectID() { return m_objectID; }
    inline void setObjectID(int newObjectID) { if (newObjectID >= 0) m_objectID = newObjectID; }
    inline int getArmor() { return m_armor; }
    inline int getRadius() { return m_radius; }
    inline UPoint getRealPos() { return m_realPos; }
    inline UPoint getPosition() { return UPoint(x,y); }
    inline float getSpeed() { return m_maxSpeed; }
    inline bool isControllable() { return m_controllable; }

    inline bool getStatus(Uint32 status) { return m_status & status; }
    inline void setStatus(Uint32 status) { m_status |= status; }
    inline void unsetStatus(Uint32 status) { m_status &= ~status; }

    bool isOnScreen(Rect rect);

    UPoint getClosestPoint(UPoint point);
    UPoint getClosestCentrePoint(UPoint objectPos);
    UPoint getCentrePoint();

    inline PlayerClass* getOwner() { return m_owner; }
    inline void setOwner(PlayerClass* newOwner) { m_owner = newOwner; }
    //@}
	//! @name  Attack related functions
	//@{
    bool canAttack(ObjectPtr object);
    void handleDamage(int damage, ObjectPtr damager);
    ObjectPtr findTarget();
    //@}
  protected:
    ATTACKTYPE m_attackMode;

    std::string m_objectName;
    
    bool m_active,
    //! Draw deathFrame if the building was destroyed, or remove unit from list and forget about it
         m_respondable,
         m_selected,
	 m_controllable,
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
    //! if true and target is friendly guard it or if it's e.g. refinery/repair yard go there
         m_targetFriendly;


    float   m_adjust,
	    m_angle,
            m_health,
	    m_maxSpeed,
	    m_turnSpeed;

    PointFloat m_realPos,
	       m_speed;

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

        m_maxHealth,

        //! Used for fog of war and area exploring.
        m_viewRange,

        m_weaponRange;

    int m_armor,
        m_radius,
	m_explosionSize,
	m_guardRange,
        m_numDeathFrames,
	m_numFrames,
        m_frameTime,
        m_frameTimer,
	m_smokeCounter,
	m_smokeFrame;
    bool m_fadingIn;


    std::string m_deathAnim;


    //! SharedPtr to image for current object
    ImagePtr m_graphic,
	     m_selectionBox,
	     m_selectionBoxGlowing;

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

    ObjectPtr m_target;

    std::vector<std::string> m_weapons;

    Uint32 m_attributes, m_status;

};

#endif //OBJECTCLASS_H
