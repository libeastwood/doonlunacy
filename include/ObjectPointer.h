#ifndef OBJECTPOINTER_H
#define OBJECTPOINTER_H

class ObjectClass;
class UnitClass;
class StructureClass;

#include "Definitions.h"
#include "DuneConstants.h"

class ObjectPointer
{
  public:
	ObjectPointer() { m_objectID = NONE; };
	ObjectPointer(Uint32 newItemID) { PointTo(newItemID); };
	ObjectPointer(ObjectClass* newObject) { PointTo(newObject); };
	~ObjectPointer() { ; };

	inline void PointTo(Uint32 newItemID) { m_objectID = newItemID; };
	void PointTo(ObjectClass* newObject);

	inline Uint32 getObjectID() { return m_objectID; };
	ObjectClass* getObjPointer();
	inline UnitClass* getUnitPointer() { return (UnitClass*) getObjPointer(); };
	inline StructureClass* getStructurePointer() { return (StructureClass*) getObjPointer(); };

	operator bool()
	{
		return (m_objectID != NONE);
	};
	
  private:
	Uint32 m_objectID;
};
#endif // OBJECTPOINTER_H
