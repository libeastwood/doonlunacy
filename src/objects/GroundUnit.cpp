#include "MapClass.h"

#include "objects/GroundUnit.h"

GroundUnit::GroundUnit(PlayerClass* newOwner, std::string unitName, uint32_t attributes) : UnitClass(newOwner, unitName, attributes | OBJECT_GROUNDUNIT)
{
}

GroundUnit::~GroundUnit()
{

}

void GroundUnit::checkPos()
{
    MapClass* map = m_owner->getMap();

    if (getStatus(STATUS_JUSTSTOPPEDMOVING))
    {
        m_realPos = UPoint(x, y) * BLOCKSIZE + BLOCKSIZE / 2;
        m_findTargetTimer = 0; //allow a scan for new targets now

        //if (currentGame->playerType == SERVER)
        // netDoConsistancy();

        if (map->getCell(SPoint(x, y))->isBloom())
        {
#if 0
            COORDTYPE realPos;
            realPos.x = (short)realX;
            realPos.y = (short)realY;
            map->getCell(&location)->damageCell(this, getOwner(), &realPos, NONE, 500, NONE, NONE, false);
#endif
        }
    }

#if 0
    if (goingToRepairYard)
    {
        COORDTYPE closestPoint = target.getObjPointer()->getClosestPoint(&location);

        if ((blockDistance(&location, &closestPoint) <= 1.5)
                && ((RepairYardClass*)target.getObjPointer())->isFree())
        {
            if (health*100 / maxHealth < 100)
                setGettingRepaired();
            else
            {
                setTarget(NULL);
                setDestination(&guardPoint);
            }
        }
    }

#endif
}

