#ifndef DUNE_OBJECTS_OBJECTCLASS_H
#define DUNE_OBJECTS_OBJECTCLASS_H

#include <string>
#include <vector>

#include <boost/dynamic_bitset.hpp>

#include "Colours.h"
#include "PlayerClass.h"
#include "PythonObjects.h"
#include "Rect.h"

class WeaponClass;
typedef std::shared_ptr<WeaponClass> WeaponPtr;

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
    ObjectClass(const PlayerClass& newOwner, const std::string& objectName, uint32_t attribute = 0);
    virtual ~ObjectClass() {}
    //@}

    friend std::ostream& operator<<(std::ostream& os, const ObjectClass& c){
	return os << c.m_objectType << "(" << (c.getOwner()).getPlayerNumber() << "," << c.getObjectName()
	    << "," << c.getAttributes() << ")" << "ID: " << c.getObjectID() << " position: "
	    << c.getPosition() << c.getRealPosition();
    }

    virtual operator std::string() const {
	std::stringstream ss(std::stringstream::in | std::stringstream::out);
	ss << *this;
	return ss.str();
    }


    //
    // Common object functions
    //

    virtual void assignToMap(SPoint pos);
    virtual void unassignFromMap(SPoint pos);
    inline bool clearObject() const { return (getStatus(STATUS_DESTROYED) && m_frameTimer == 0 && m_decayTime == 0); }
    
    void setDrawnPos(SPoint off, SPoint view);
    virtual void draw(Image& dest, SPoint off, SPoint view);
    void drawSelectionBox(Image& dest);

    void drawSmoke(Image& dest);
    virtual bool destroy();
    virtual void update(float dt);
    virtual void animate();
    virtual void doDeath(Image& dest);

    //! @name Setters and getters
	//@{

    void handleDamage(int16_t damage, ObjectPtr damager = ObjectPtr());
    inline int16_t getHealth() const { return m_health; }
    void setVisible(bool status, int team = -1);

    virtual bool setDestination(ConstSPoint realDestination, uint32_t status = 0);
    bool attack();
    virtual void setPosition(SPoint pos);
    virtual void setRealPosition(SPoint realPos);

    inline bool hasAttribute(uint32_t attribute) const { return m_attributes & attribute; }
    inline uint32_t getAttributes() const { return m_attributes; }

    inline bool getStatus(uint32_t status) const { return m_status & status; }
    inline uint32_t getStatusAll() const { return m_status; }
    virtual inline void setStatus(uint32_t status) { m_status |= status; }
    inline void clearStatus(uint32_t status = STATUS_ALL) { m_status &= ~status; }


    //! Checks if a given team can see this object
    inline bool isVisible (int team) const;
    int getHealthColour() const;

    inline const std::string& getObjectName() const { return m_objectName; }
    int getViewRange() const;

    inline uint32_t getObjectID() const { return m_objectID; }
    inline void setObjectID(uint32_t newObjectID) { m_objectID = newObjectID; }
    inline int getArmor() const { return m_armor; }
    inline int getRadius() const { return m_radius; }
    inline SPoint getRealPosition() const { return m_realPosition; }
    inline float getSpeed() const { return m_maxSpeed; }

    inline bool isOnScreen(Rect rect) const { return rect.containsPartial(Rect(m_realPosition.x, m_realPosition.y, w, h)); }

    inline SPoint getPosition() const { return Rect::getPosition(); }
    inline SPoint getSize() const { return Rect::getSize(); }
    SPoint getClosestPoint(SPoint point) const;
    SPoint getClosestCentrePoint(SPoint objectPos) const;
    inline SPoint getCentrePoint() const { return SPoint(getRealPosition()+(getSize()/2)); }

    float coverage(Rect rect) const { return Rect(getRealPosition(), getSize()).contains(rect); }

    inline const PlayerClass& getOwner() const { return *m_owner; }
    inline void setOwner(const PlayerClass newOwner) { m_owner = &newOwner; }
    //@}
	//! @name  Attack related functions
	//@{
    bool canAttack(ObjectPtr object) const;
    ObjectPtr findTarget();
    //@}
  protected:
    inline void setAttribute(uint32_t attribute) { m_attributes |= attribute; }
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

    PointFloat m_realPosition,
	       m_speed;

    /*! Currently drawn frame
     *  @note 0 Just placed
     *        1 Destroyed
     *        2 Normal flag down
     *        3 Normal flag up
     *        4,5,6,... Refinery, Starport,etc. when a unit docks, brings spice, whatever
     *
     */
    int16_t m_checkTimer,
    //! Increments every time a sprite is drawn. Usually after 25 cycles animation frame is changed
	   m_animCounter,
	   m_curAnimFrame,
	   m_decayTime,

	   //! Total number of frames
	   m_animFrames,

	   m_drawnAngle,
	   m_drawnTopAngle,

	   //! Used for fog of war and area exploring.
	   m_viewRange,

	   m_weaponRange;

    int16_t m_armor,
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
		m_objectType,
		m_deathAnim;


    //! SharedPtr to image for current object
    ImagePtr m_graphic,
	     m_topGraphic,
	     m_selectionBox,
	     m_selectionBoxGlowing;

    //! Pointer to map to which object is assigned.
    const PlayerClass *m_owner;

    uint32_t m_objectID;

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
    std::vector<SPoint> m_topOffset;

    python::object m_pyObject;

  private:
    uint32_t m_attributes,
	   m_status,
	   m_ticks;

    int16_t m_maxHealth;

    float m_health;



};

#endif //OBJECTCLASS_H
