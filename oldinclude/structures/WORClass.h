#ifndef DUNE_STRUCTURES_WORCLASS_H
#define DUNE_STRUCTURES_WORCLASS_H

#include "structures/BuilderClass.h"

class PlayerClass;

class WORClass : public BuilderClass
{
public:
        WORClass(PlayerClass* newOwner);
	~WORClass();

	void checkSelectionList();
	void doSpecificStuff();

private:

};

#endif // DUNE_STRUCTURES_WORCLASS_H
