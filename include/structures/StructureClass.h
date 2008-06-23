#ifndef DUNE_STRUCTURECLASS_H
#define DUNE_STRUCTURECLASS_H

#define ANIMATIONTIMER 25
#define ANIMMOVETIMER 50

#include "ObjectClass.h"
#include "PlayerClass.h"
/*! 
 * Base class for all structures
 */

class StructureClass : public ObjectClass
{
  public:
    //! @name Constructors & Destructor
    //@{
    StructureClass(PlayerClass* newOwner);
    virtual ~StructureClass();
    //@} 
    
    void assignToMap(SPoint pos);

    virtual void destroy() {};
    
    virtual void draw(Image * dest, SPoint off, SPoint view);
    void drawStructure(Image * dest);
    void drawSelectRect(Image * dest);
    
    virtual void update();

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

    void setDrawnPos(SPoint off, SPoint view);
};

typedef std::vector<StructureClass*> Structures;

#endif // DUNE_STRUCTURECLASS_H
