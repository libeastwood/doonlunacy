#ifndef DUNE_STRUCTURES_REFINERYCLASS_H
#define DUNE_STRUCTURES_REFINERYCLASS_H

#include "structures/StructureClass.h"

class PlayerClass;
class HarvesterClass;

class RefineryClass : public StructureClass
{
public:
	RefineryClass(PlayerClass* newOwner);
	~RefineryClass();

	void assignHarvester(HarvesterClass* newHarvester);
	void deployHarvester();
	void doSpecificStuff();
	void drawGraphic();
	void startAnimate();

	inline void book() { bookings++; }
	inline void unBook() { bookings--; }
	inline bool isFree() { return !extractingSpice; }
	inline int getNumBookings() { return bookings; }	//number of units goings there
	inline HarvesterClass* getHarvester() { return harvester; }

private:
	bool	extractingSpice;

	int	bookings, firstRun;

	HarvesterClass*	harvester;
};

#endif // DUNE_STRUCTURES_REFINERYCLASS_H
