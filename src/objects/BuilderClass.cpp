#include "objects/BuilderClass.h"

BuilderClass::BuilderClass(PlayerClass* newOwner, std::string structureName, uint32_t attribute) : StructureClass(newOwner, structureName, attribute | OBJECT_BUILDER) {
}

// vim:ts=8:sw=4:et
