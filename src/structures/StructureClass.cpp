#include "DuneConstants.h"
#include "Log.h"
#include "MapClass.h"
#include "structures/StructureClass.h"

StructureClass::StructureClass(PlayerClass* newOwner) : ObjectClass(newOwner)
{
    m_justPlacedTimer = 0;
	m_itemID = Unknown;
	w = h = 0;
}

StructureClass::~StructureClass()
{

}

void StructureClass::assignToMap(SPoint pos)
{
    MapClass * map = m_owner->getMap();
	SPoint temp;
	for (int i = x; i < x + w; i++)
		for (int j = y; j < y + h; j++)
		{
			temp.x = i; temp.y = j;
			if (map->cellExists(i, j))
			{
				map->getCell(i,j)->assignNonInfantryGroundObject(this);
				//if ((m_itemID != Structure_Wall) && (m_itemID != Structure_ConstructionYard) && !map->getCell(i,j)->isConcrete()) //&& currentGame->concreteRequired && (gameState != START))
				//	health -= (int)(0.5*(double)maxHealth/((double)(structureSize.x*structureSize.y)));
				map->getCell(i,j)->setType(Terrain_Rock);
				//map->getCell(i,j)->setOwner(getOwner()->getPlayerNumber());
				//map->viewMap(getOwner()->getTeam(), &temp, getViewRange());

				//setVisible(VIS_ALL, true);
				//setRespondable(true);
			}
		}
}

void StructureClass::setJustPlaced()
{
    if (!m_destroyed)
    {
        m_justPlacedTimer = 4;
        m_curAnimFrame = 0;
    }
}

void StructureClass::draw(Image * dest, SPoint off, SPoint view)
{
    Rect src(m_curAnimFrame * w*BLOCKSIZE,0,w*BLOCKSIZE,h*BLOCKSIZE);
    
    UPoint dst(off.x+m_realPos.x-view.x*BLOCKSIZE,off.y+m_realPos.y-view.y*BLOCKSIZE);
    
    m_pic->blitTo(dest, src, dst);
}



void StructureClass::update()
{
    if (!m_destroyed)
    {
		// update animations
		m_animCounter++;
		if(m_animCounter > ANIMATIONTIMER) 
		{
			m_animCounter = 0;
			m_curAnimFrame++;
			if((m_curAnimFrame < m_firstAnimFrame) || (m_curAnimFrame > m_lastAnimFrame)) 
			{
				m_curAnimFrame = m_firstAnimFrame;
			}
		    
			m_justPlacedTimer--;
			if((m_justPlacedTimer > 0) && (m_justPlacedTimer % 2 == 0)) 
			{
				m_curAnimFrame = 0;
			}
        }
    }	
}
