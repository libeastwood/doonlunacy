#ifndef DUNE_STRUCTURES_WINDTRAPCLASS_H
#define DUNE_STRUCTURES_WINDTRAPCLASS_H

#include "structures/StructureClass.h"

class PlayerClass;

class WindTrapClass : public StructureClass
{
public:
        WindTrapClass(PlayerClass* newOwner);
	~WindTrapClass();

	void doSpecificStuff();

private:
	int colourTimer;
};

#endif // DUNE_STRUCTURES_WINDTRAPCLASS_H
