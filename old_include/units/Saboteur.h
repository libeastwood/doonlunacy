#ifndef DUNE_UNITS_SABOTEUR_H
#define DUNE_UNITS_SABOTEUR_H

#include "units/InfantryClass.h"
#include "Math.h"

class ObjectClass;

class Saboteur : public InfantryClass
{
public:
	Saboteur(PlayerClass* newOwner);
	~Saboteur();

	void checkPos();
	void deploy(COORDTYPE* newLocation);
	bool canAttack(ObjectClass* object);

protected:
};

#endif // DUNE_UNITS_SABOTEUR_H
