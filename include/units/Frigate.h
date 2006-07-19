#ifndef DUNE_UNITSFRIGATE_H
#define DUNE_UNITSFRIGATE_H

#include "units/Carryall.h"

class Frigate : public Carryall
{
public:
	Frigate(PlayerClass* newOwner);
	~Frigate();
};

#endif // DUNE_UNITSFRIGATE_H
