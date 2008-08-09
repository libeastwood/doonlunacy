#include "GameState.h"
#include "ObjectClass.h"
#include "ObjectPointer.h"

ObjectClass* ObjectPointer::getObjPointer()
{
	if(ObjectID == NONE) 
	{
		return NULL;
	}

	ObjectClass* objPointer = GameState::Instance()->GetObjectTree().getObject(ObjectID);

	if(ObjPointer == NULL) 
	{
		m_objectID = NONE;
	}
	
	return objPointer;
}

void ObjectPointer::PointTo(ObjectClass* newObject)
{
	if(newObject != NULL) 
	{
		m_objectID = newObject->getObjectID();
	} else {
		m_objectID = NONE;
	}
}
