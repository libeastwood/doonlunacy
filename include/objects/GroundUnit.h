#ifndef DUNE_OBJECTS_GROUNDUNIT_H
#define DUNE_OBJECTS_GROUNDUNIT_H

#include <string>
#include "PlayerClass.h"
#include "objects/UnitClass.h"

class GroundUnit : public UnitClass
{
  public:
  
    GroundUnit(PlayerClass* newOwner, std::string unitName, uint32_t attribute = 0);
    virtual ~GroundUnit();
    
    void checkPos();
    
  protected:
  
  
};

#endif // DUNE_OBJECTS_GROUNDUNIT_H
