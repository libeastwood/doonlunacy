#ifndef DUNE_STRUCTURES_RADARCLASS_H
#define DUNE_STRUCTURES_RADARCLASS_H

#include "structures/StructureClass.h"

class PlayerClass;

class RadarClass : public StructureClass
{
public:
	RadarClass(PlayerClass* newOwner);
	~RadarClass();

	void doSpecificStuff();

private:

};

#endif // DUNE_STRUCTURES_RADARCLASS_H

