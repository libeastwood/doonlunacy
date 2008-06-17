#ifndef DUNE_GROUNDUNIT_H
#define DUNE_GROUNDUNIT_H

#include "PlayerClass.h"
#include "units/UnitClass.h"

class GroundUnit : public UnitClass
{
  public:
  
    GroundUnit(PlayerClass* newOwner);
    virtual ~GroundUnit();
    
  protected:
  
  
};

#endif // DUNE_GROUNDUNIT_H

