#ifndef DUNE_STRUCTURES_REPAIRYARDCLASS
#define DUNE_STRUCTURES_REPAIRYARDCLASS

#include "structures/StructureClass.h"
#include "gui/Button.h"

class PlayerClass;
class UnitClass;

class RepairYardClass : public StructureClass
{
public:
	RepairYardClass(PlayerClass* newOwner);
	~RepairYardClass();

	void deployRepairUnit();
	void doSpecificStuff();

	inline void book() { bookings++; }
	inline void unBook() { bookings--; }
	inline void assignUnit(UnitClass* newUnit) { repairUnit = newUnit; repairing = true; }
	inline bool isFree() { return !repairing; }
	inline int getNumBookings() { return bookings; }	//number of harvesters goings there
	inline UnitClass* getRepairUnit() { return repairUnit; }

private:
	bool	repairing;

	int	bookings;
	int picID;

	UnitClass* repairUnit;
	Button repairProgress;
};

#endif // DUNE_STRUCTURES_REPAIRYARDCLASS
