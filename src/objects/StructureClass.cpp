#include "DuneConstants.h"
#include "Log.h"
#include "MapClass.h"
#include "objects/StructureClass.h"

StructureClass::StructureClass(PlayerClass* newOwner, std::string structureName) : ObjectClass(newOwner, structureName)
{
    DataCache *cache = DataCache::Instance();

    m_justPlacedTimer = 0;
    m_attributes |= OBJECT_STRUCTURE;
    try {
	    m_isAnimating = cache->getPyObjectAttribute<bool>(m_objectName, "animate");
	    m_firstAnimFrame = cache->getPyObjectAttribute<int>(m_objectName, "firstAnimFrame");
	    m_lastAnimFrame = cache->getPyObjectAttribute<int>(m_objectName, "lastAnimFrame");
	    m_powerRequirement = cache->getPyObjectAttribute<int>(m_objectName, "powerRequirement");
    }
    catch(python::error_already_set const &)
    {
        LOG_FATAL("StructureClass", "Error loading object: %s", m_objectName.c_str());
        PyErr_Print();
        exit(1);
    }

}

StructureClass::~StructureClass()
{

}

/*
void StructureClass::assignToMap(SPoint pos)
{
    MapClass * map = m_owner->getMap();
	SPoint temp;
	for (int i = x; i < x + w/BLOCKSIZE; i++)
		for (int j = y; j < y + h/BLOCKSIZE; j++)
		{
			temp.x = i; temp.y = j;
			if (map->cellExists(i, j))
			{
				map->getCell(i,j)->assignObject(getObjectID());
				//if ((m_itemID != Structure_Wall) && (m_itemID != Structure_ConstructionYard) && !map->getCell(i,j)->isConcrete()) //&& currentGame->concreteRequired && (gameState != START))
				//	health -= (int)(0.5*(float)maxHealth/((float)(structureSize.x*structureSize.y)));
				map->getCell(i,j)->setType(Terrain_Rock);
				//map->getCell(i,j)->setOwner(getOwner()->getPlayerNumber());
				//map->viewMap(getOwner()->getTeam(), &temp, getViewRange());

				//setVisible(VIS_ALL, true);
				//setRespondable(true);
			}
		}
}
*/

void StructureClass::setJustPlaced()
{
    if (!m_destroyed)
    {
        m_justPlacedTimer = 4;
        m_curAnimFrame = 0;
    }
}
#if 0

void StructureClass::draw(Image * dest, SPoint off, SPoint view)
{
	if (!m_destroyed)
	{
    Rect src(m_curAnimFrame * w,0,w,h);
    /*
    //TODO: Put m_fogged somw    
if(m_fogged)
	{
		//m_pic->blitTo(SDL_BlitSurface(graphic, &lastVisible, screen, &dest);
		//SDL_BlitSurface(fogSurf, &lastVisible, screen, &dest);
	}
	else
	{*/
//		m_lastVisible = source;
		m_graphic->blitTo(dest, src, m_drawnPos);

//		if (m_selected)
//		drawSelectRect(dest);
//			selectionBlitList->insertLast(this);
	}
	else
	{
    //TODO: Add this death graphic stuff
    #if 0
		int i, j;
		SDL_Rect dest, source;
		dest.w = source.w = imageW;
		dest.h = source.h = imageH;
		source.x = deathFrame*imageW;
		source.y = 0;
		
		for(i = 0; i < structureSize.x; i++)
			for(j = 0; j < structureSize.y; j++)
			{
				dest.x = getDrawnX() + i*BLOCKSIZE;
				dest.y = getDrawnY() + j*BLOCKSIZE;

				SDL_BlitSurface(deathGraphic[i][j], &source, screen, &dest);
		}
	#endif
	}
}
#endif

void StructureClass::drawSelectionBox(Image * dest)
{
    int x2 = m_drawnPos.x + ((int)(((float)m_health/(float)m_maxHealth)*(w - 1))),
	color = getHealthColour();

    for(int i = 2; i < 5; i++)
    	dest->drawHLine(m_drawnPos + UPoint(2, i), x2, color);

    // TODO: Should be glowing..
    dest->drawRect(Rect(m_drawnPos.x, m_drawnPos.y, w, h), COLOUR_WHITE);
}

void StructureClass::animate()
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
