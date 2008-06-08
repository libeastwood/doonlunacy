#ifndef DUNE_STRUCTURES_WALLCLASS_H
#define DUNE_STRUCTURES_WALLCLASS_H

#include "structures/StructureClass.h"

class PlayerClass;

class WallClass : public StructureClass
{
public:
    WallClass(PlayerClass* newOwner);
	~WallClass();

	inline void setTile(int newTile) { m_animFrame = newTile; }
};

#endif // DUNE_STRUCTURES_WALLCLASS_H
