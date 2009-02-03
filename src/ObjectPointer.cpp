#include "GameMan.h"
#include "ObjectPointer.h"

#include "objects/ObjectClass.h"

ObjectClass* ObjectPointer::getObjPointer()
{
	if(m_objectID == NONE) 
	{
		return NULL;
	}

	ObjectClass* objPointer = GameMan::Instance()->GetObjectTree()->getObject(m_objectID);

	if(objPointer == NULL) 
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
