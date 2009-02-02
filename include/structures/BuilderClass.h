#ifndef DUNE_BUILDERCLASS_H
#define DUNE_BUILDERCLASS_H

#include <string>

#include "structures/StructureClass.h"
class BuilderClass : public StructureClass
{
    public:
        BuilderClass(PlayerClass* newOwner, std::string);
        ~BuilderClass();

};

#endif // DUNE_BUILDERCLASS_H
// vim:ts=8:sw=4:et
