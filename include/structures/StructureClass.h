#ifndef DUNE_STRUCTURECLASS_H
#define DUNE_STRUCTURECLASS_H

#define FLAGMOVETIMER 25
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
    
    void assignToMap(UPoint pos);
   	int getDrawnX();
	int getDrawnY();
    virtual void destroy() {};
    
    virtual void draw(Image * dest, SPoint off, SPoint view);
    
    virtual void update();
    
  protected:
    int m_powerRequirement;
};

#endif // DUNE_STRUCTURECLASS_H
