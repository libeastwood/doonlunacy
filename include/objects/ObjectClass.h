#ifndef DUNE_OBJECTS_OBJECTCLASS_H
#define DUNE_OBJECTS_OBJECTCLASS_H

#include <string>
#include <vector>

#include <boost/dynamic_bitset.hpp>

#include "PlayerClass.h"
#include "PythonObjects.h"
#include "Rect.h"

class WeaponClass;
typedef boost::shared_ptr<WeaponClass> WeaponPtr;

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
    STATUS_DESTROYED = 1 << 3,
    STATUS_PICKEDUP = 1 << 4,
    STATUS_TURNING = 1 << 5,
    STATUS_TRACKED = 1 << 6,
    STATUS_TURRETED = 1 << 7,
    STATUS_JUSTSTOPPEDMOVING = 1 << 8,
    STATUS_NEXTSPOTFOUND = 1 << 9,
    STATUS_SELECTED = 1 << 10,
    STATUS_ACTIVE = 1 << 11,
    STATUS_FORCED = 1 << 12,
    STATUS_RESPONDABLE = 1 << 13,
    STATUS_CONTROLLABLE = 1 << 14,
    STATUS_BADLYDAMAGED = 1 << 15,
    STATUS_DEFAULT = 1 << 16,
    STATUS_ALL = -1
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

class ObjectClass : protected Rect
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
    inline bool clearObject() { return (getStatus(STATUS_DESTROYED) && m_frameTimer == 0 && m_decayTime == 0); }
    
    void setDrawnPos(SPoint off, SPoint view);
    virtual void draw(Image * dest, SPoint off, SPoint view);
    void drawSelectionBox(Image* dest);

    void drawSmoke(Image *dest);
    virtual bool destroy();
    virtual void update(float dt);
    virtual void animate();
    virtual void doDeath(Image *dest);

    //! @name Setters and getters
	//@{

    void handleDamage(Sint16 damage, ObjectPtr damager = ObjectPtr());
    inline Sint16 getHealth() { return m_health; }
    void setVisible(bool status, int team = -1);

    virtual bool setDestination(ConstSPoint realDestination, Uint32 status = 0);
    bool attack();
    virtual void setPosition(SPoint pos);
    virtual void setRealPosition(SPoint realPos);

    inline bool hasAttribute(Uint32 attribute) { return m_attributes & attribute; }
    inline Uint32 getAttributes() { return m_attributes; }

    inline bool getStatus(Uint32 status) { return m_status & status; }
    inline Uint32 getStatusAll() { return m_status; }
    virtual inline void setStatus(Uint32 status) { m_status |= status; }
    inline void clearStatus(Uint32 status = STATUS_ALL) { m_status &= ~status; }


    //! Checks if a given team can see this object
	inline bool isVisible(int team);
    int getHealthColour();

    inline std::string getObjectName() { return m_objectName; }
    int getViewRange();

    inline Uint32 getObjectID() { return m_objectID; }
    inline void setObjectID(Uint32 newObjectID) { m_objectID = newObjectID; }
    inline int getArmor() { return m_armor; }
    inline int getRadius() { return m_radius; }
    inline SPoint getRealPos() { return m_realPos; }
    inline float getSpeed() { return m_maxSpeed; }

    bool isOnScreen(Rect rect);

    inline SPoint getPosition() { return Rect::getPosition(); }
    inline SPoint getSize() { return Rect::getSize(); }
    SPoint getClosestPoint(SPoint point);
    SPoint getClosestCentrePoint(SPoint objectPos);
    inline SPoint getCentrePoint() { return SPoint(getRealPos()+(getSize()/2)); }

    float coverage(Rect rect) { return Rect(getRealPos(), getSize()).contains(rect); }

    inline PlayerClass* getOwner() { return m_owner; }
    inline void setOwner(PlayerClass* newOwner) { m_owner = newOwner; }
    //@}
	//! @name  Attack related functions
	//@{
    bool canAttack(ObjectPtr object);
    ObjectPtr findTarget();
    //@}
  protected:
    inline void setAttribute(Uint32 attribute) { m_attributes |= attribute; }
    ATTACKTYPE m_attackMode;

	//! Specifies which players can see a given object
    boost::dynamic_bitset<> m_visible;

    /*!
     *  If set to true, animation frame will change in certain intervals.
     *  We don't want it in case of walls, turrets, etc.
     */

    float   m_adjust,
	    m_angle,
	    m_maxSpeed,
	    m_turnSpeed;

    PointFloat m_realPos,
	       m_speed;

    /*! Currently drawn frame
     *  @note 0 Just placed
     *        1 Destroyed
     *        2 Normal flag down
     *        3 Normal flag up
     *        4,5,6,... Refinery, Starport,etc. when a unit docks, brings spice, whatever
     *
     */
    Sint16 m_checkTimer,
    //! Increments every time a sprite is drawn. Usually after 25 cycles animation frame is changed
	   m_animCounter,
	   m_curAnimFrame,
	   m_decayTime,

	   //! Total number of frames
	   m_animFrames,

	   m_drawnAngle,

	   //! Used for fog of war and area exploring.
	   m_viewRange,

	   m_weaponRange;

    Sint16 m_armor,
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


    std::string m_objectName,
		m_deathAnim;


    //! SharedPtr to image for current object
    ImagePtr m_graphic,
	     m_selectionBox,
	     m_selectionBoxGlowing;

    //! Pointer to map to which object is assigned.
    PlayerClass *m_owner;

    Uint32 m_objectID;

    SPoint m_destination,
	   m_realDestination,
           m_oldPosition;

    /*!
     *  Position on the screen where an object will be drawn. It depends on
     *  position of MapWidget, current view position and of course object's position.
     */
    SPoint m_drawnPos,
	   m_offset;

    ObjectPtr m_target;

    std::vector<WeaponPtr> m_weapons;

    python::object m_pyObject;

  private:
    Uint32 m_attributes,
	   m_status,
	   m_ticks;

    Sint16 m_maxHealth;

    float m_health;



};

#endif //OBJECTCLASS_H
