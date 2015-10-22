#ifndef DUNE_OBJECTS_STRUCTURECLASS_H
#define DUNE_OBJECTS_STRUCTURECLASS_H

#define ANIMATIONTIMER 25
#define ANIMMOVETIMER 50

#include "objects/ObjectClass.h"
#include "PlayerClass.h"
/*! 
 * Base class for all structures
 */

class StructureClass : public ObjectClass
{
  public:
    //! @name Constructors & Destructor
    //@{
    StructureClass(const PlayerClass& newOwner, const std::string& structureName, uint32_t attributes = 0);
    //@} 
    
    virtual void animate();

	inline int getPowerRequirement() { return m_powerRequirement; }
	
    void setJustPlaced();

  protected:
    int m_powerRequirement,
    
    /*!
     *  Buildings may be in different states. E.g. 
     *  Refinery
     *  <ul>
     *  <li>3-4 normal state</li>
     *  <li>4-5 awaiting harvester</li>
     *  <li>6-7 awaiting harverser - different docking pad lights</li>
     *  <li>8-9 harvester unloading spice</li>
     *  </ul>
     *  And m_firsAnimFrame, m_lastAnimframe are used to switch between drawing appropriate frames
     */
        m_firstAnimFrame,
        m_lastAnimFrame,

     //! Specifies for how long building is going to be shown as under construction (Frame 0)
        m_justPlacedTimer;
};

#endif // DUNE_OBJECTS_STRUCTURECLASS_H
