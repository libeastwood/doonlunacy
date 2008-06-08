#include "ObjectClass.h"
#include "DuneConstants.h"
ObjectClass::ObjectClass(PlayerClass* newOwner) : 
    Rect(0,0,0,0)
{
    m_owner = newOwner;
    m_animCounter = 0;
}

ObjectClass::~ObjectClass()
{

}

void ObjectClass::draw(Image * dest, SPoint off, SPoint view)
{
    m_pic->blitTo(dest, UPoint(x+off.x,y+off.y));
}

bool ObjectClass::isOnScreen(Rect rect) {
    return rect.containsPartial(Rect(m_realPos.x,m_realPos.y,w,h)); 
}

void ObjectClass::setLocation(SPoint pos) {

	if((pos.x == INVALID_POS) && (pos.y == INVALID_POS)) 
	{
		x = INVALID_POS;
		y = INVALID_POS;
	} else if (m_owner->getMap()->cellExists(pos))	
	{
		x = pos.x;
		y = pos.y;
		m_realPos.x = pos.x*BLOCKSIZE;
		m_realPos.y = pos.y*BLOCKSIZE;

//		assignToMap(&location);
	}
}
