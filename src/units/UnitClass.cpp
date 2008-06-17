#include "Definitions.h"
#include "Gfx.h"
#include "Log.h"
#include "MapClass.h"
#include "ObjectClass.h"

#include "units/UnitClass.h"

UnitClass::UnitClass(PlayerClass* newOwner) : ObjectClass(newOwner)
{
    m_unit = true;
    m_destroyed = false;
    m_drawnAngle = 0;
}

UnitClass::~UnitClass()
{

}


bool UnitClass::canPass(UPoint pos)
{
    MapClass * map = m_owner->getMap();

	return (map->cellExists(pos) && !map->getCell(pos)->hasAGroundObject() && !map->getCell(pos)->isMountain());
}

void UnitClass::deploy(UPoint newPosition)
{
	if (m_owner->getMap()->cellExists(newPosition))
	{
		setPosition(newPosition);

/*		if ((guardPoint.x == NONE) || (guardPoint.y == NONE))

			guardPoint = location;

		setDestination(&guardPoint);

		pickedUp = false;

		setRespondable(true);

		setActive(true);

		setVisible(VIS_ALL, true);
*/
    
	}

}

void UnitClass::draw(Image * dest, SPoint off, SPoint view)
{
    UPoint dst(off.x+m_realPos.x-view.x*16,off.y+m_realPos.y-view.y*16);
    Rect src(0,0, w*16,h*16);
	
	if (getItemID() == Unit_Frigate)
		m_itemID = Unit_Frigate;

	if (!m_destroyed)
	{

		src.x = m_drawnAngle*w*16;
		src.y = 0;

        m_pic->blitTo(dest, src, dst);
	}
	else
	{
		src.x = m_deathFrame*w;
		src.y = 0;

        m_pic->blitTo(dest, src, dst);
	}
}

void UnitClass::setAngle(int newAngle)
{
	if (!m_moving && (newAngle >= 0) && (newAngle < NUM_ANGLES))

	{

		m_angle = m_drawnAngle = newAngle;

		m_nextSpotAngle = m_drawnAngle; 

		m_nextSpotFound = false;

	}
}

void UnitClass::setPosition(SPoint pos)
{
	if (m_owner->getMap()->cellExists(pos) || ((pos.x == NONE) && (pos.y == NONE)))

	{

		ObjectClass::setPosition(pos);

		m_realPos.x += BLOCKSIZE/2;

		m_realPos.y += BLOCKSIZE/2;

		m_moving = false;


		m_nextSpotFound = false;

		m_nextSpotAngle = m_drawnAngle;
		
/*

		m_pickedUp = false;

		setTarget(NULL);

		clearPath();

		noCloserPointCount = 0;
*/
	}
}

void UnitClass::update()
{

}
