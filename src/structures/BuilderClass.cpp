#include "structures/BuilderClass.h"

BuilderClass::BuilderClass(PlayerClass* newOwner, std::string structureName) : StructureClass(newOwner, structureName) {
    m_attributes |= OBJECT_STRUCTURE;
}

BuilderClass::~BuilderClass()
{

}

// vim:ts=8:sw=4:et
