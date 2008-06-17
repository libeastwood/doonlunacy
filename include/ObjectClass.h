#ifndef OBJECTCLASS_H
#define OBJECTCLASS_H

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

class ObjectClass : public Rect
{
  public:
    //!
    //@{
    ObjectClass(PlayerClass* newOwner);
    virtual ~ObjectClass();
    //@}
    
    virtual void assignToMap(SPoint pos);
    
    virtual void draw(Image * dest, SPoint off, SPoint view);
    
    virtual void destroy() = 0;
  
    virtual void update() = 0;
  
    bool isOnScreen(Rect rect);
    
    inline int getItemID() { return m_itemID; } 
    inline int getRealX() { return m_realPos.x; }
    inline int getRealY() { return m_realPos.y; }    
    
  	inline bool isActive() { return m_active; }
	inline bool isAFlyingUnit() { return m_flyingUnit; }
	inline bool isAGroundUnit() { return m_groundUnit; }
	inline bool isAStructure() { return m_structure; }
	inline bool isABuilder() { return m_builder; }
	inline bool isInfantry() { return m_infantry; }
	inline bool isAUnit() { return m_unit; }
	inline bool isRespondable() { return m_respondable; }
	inline bool isSelected() { return m_selected; }
	
	virtual void setPosition(SPoint pos);
	inline PlayerClass* getOwner() { return m_owner; }
	inline void setOwner(PlayerClass* newOwner) { m_owner = newOwner; }
    inline Uint32 getObjectID() { return m_objectID; }
    inline void setObjectID(int newObjectID) { if (newObjectID >= 0) m_objectID = newObjectID; }
    
  protected:

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
         m_unit;

    /*! 
     *  If set to true, animation frame will change in certain intervals.
     *  We don't want it in case of walls, turrets, etc.
     */
    bool m_isAnimating;

  
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
    int m_curAnimFrame,
    
    //! Total number of frames
        m_animFrames,
    
        m_drawnAngle,
    
    //! Type of current object e.g. Structure_Silo
        m_itemID,
    
        m_maxHealth,
    
    //! Used for fog of war and area exploring.
        m_viewRange;
    
    int m_deathFrame,

        m_numDeathFrames;


    //! SharedPtr to image for current object
    ImagePtr m_pic;
    
    //! Pointer to map to which object is assigned.
    PlayerClass * m_owner;

    Uint32 m_objectID;

    UPoint m_destination;

    UPoint m_offset;
};

#endif //OBJECTCLASS_H
