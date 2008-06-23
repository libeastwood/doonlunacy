#include <math.h>

#include "ObjectClass.h"
#include "DuneConstants.h"
#include "Definitions.h"
#include "Log.h"
#include "MapClass.h"

ObjectClass::ObjectClass(PlayerClass* newOwner) :
        Rect(0, 0, 0, 0)
{
    m_itemID = Unknown;
    m_objectID = NONE;

    m_animCounter = 0;

    m_owner = newOwner;
    m_flyingUnit = false;
    m_groundUnit = false;
    m_structure = false;
    m_unit = false;
    m_infantry = false;
    m_builder = false;

    m_realPos = PointFloat(0, 0);

    m_animCounter = 0;
    m_animFrames = 1;
    m_curAnimFrame = 0;
    m_isAnimating = false;
    m_selected = false;

    m_checkTimer = 0;
    m_drawnAngle = 2;
    m_angle = 256 / 8 * m_drawnAngle;
}

ObjectClass::~ObjectClass()
{

}

void ObjectClass::assignToMap(SPoint pos)
{
    MapClass* map = m_owner->getMap();

    if (map->cellExists(pos))
    {
        map->getCell(pos)->assignNonInfantryGroundObject(this);
        //  map->viewMap(owner->getTeam(), &location, viewRange);
    }
}

void ObjectClass::draw(Image * dest, SPoint off, SPoint view)
{
    m_pic->blitTo(dest, UPoint(x + off.x, y + off.y));
}

UPoint ObjectClass::getClosestPoint(UPoint point)
{
    return UPoint(x, y);
}

int ObjectClass::getHealthColour()
{
    double healthPercent = (double)m_health / (double)m_maxHealth;

    if (healthPercent >= 0.7)
        return COLOUR_LIGHTGREEN;
    else if (healthPercent >= HEAVILYDAMAGEDRATIO)
        return COLOUR_YELLOW;
    else
        return COLOUR_RED;
}

bool ObjectClass::isOnScreen(Rect rect)
{
    return rect.containsPartial(Rect(m_realPos.x, m_realPos.y, w, h));
}

void ObjectClass::setDestination(SPoint destination)
{
    if (m_owner->getMap()->cellExists(destination) || ((destination.x == INVALID_POS) && (destination.y == INVALID_POS)))
    {
        m_destination = destination;
    }
}

void ObjectClass::setPosition(SPoint pos)
{

    if ((pos.x == INVALID_POS) && (pos.y == INVALID_POS))
    {
        x = INVALID_POS;
        y = INVALID_POS;
    }

    else if (m_owner->getMap()->cellExists(pos))
    {
        x = pos.x;
        y = pos.y;
        m_realPos.x = pos.x * BLOCKSIZE;
        m_realPos.y = pos.y * BLOCKSIZE;

        assignToMap(pos);
    }
}

void ObjectClass::unassignFromMap(SPoint pos)
{
    if (m_owner->getMap()->cellExists(pos))
        m_owner->getMap()->getCell(pos)->unassignObject(getObjectID());
}
