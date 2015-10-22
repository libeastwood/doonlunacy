#ifndef DUNE_BUILDERCLASS_H
#define DUNE_BUILDERCLASS_H

#include <string>

#include "objects/StructureClass.h"
class BuilderClass : public StructureClass
{
    public:
        BuilderClass(const PlayerClass& newOwner, const std::string&, uint32_t attribute = 0);

};

#endif // DUNE_BUILDERCLASS_H
// vim:ts=8:sw=4:et
