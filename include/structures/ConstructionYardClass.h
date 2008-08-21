#ifndef CONSTRUCTIONYARDCLASS_H_INCLUDED
#define CONSTRUCTIONYARDCLASS_H_INCLUDED

#include "StructureClass.h"

class ConstructionYardClass : public StructureClass
{
  public:
    ConstructionYardClass(PlayerClass* newOwner);
    ~ConstructionYardClass();
};


#endif // CONSTRUCTIONYARDCLASS_H_INCLUDED
