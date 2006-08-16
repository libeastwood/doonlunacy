#ifndef DUNE_STRUCTURES_WALLCLASS_H
#define DUNE_STRUCTURES_WALLCLASS_H

#include "structures/StructureClass.h"

class PlayerClass;

class WallClass : public StructureClass
{
public:
        WallClass(PlayerClass* newOwner);
	~WallClass();

	void checkSelf();
	void doSpecificStuff();

	inline void setTile(int newTile) { animFrame = newTile; }
};

#endif // DUNE_STRUCTURES_WALLCLASS_H
