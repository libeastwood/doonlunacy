#include "DuneConstants.h"
#include "Log.h"
#include "structures/StructureClass.h"

StructureClass::StructureClass(PlayerClass* newOwner) : ObjectClass(newOwner)
{
    m_animFrame = 1;
}

StructureClass::~StructureClass()
{

}

void StructureClass::assignToMap(UPoint pos)
{

}

int StructureClass::getDrawnX()
{
	return ((int)(m_realPos.x - m_offset.x));
}

int StructureClass::getDrawnY()
{
	return ((int)(m_realPos.y - m_offset.y));
}

void StructureClass::draw(Image * dest, SPoint off, SPoint view)
{
    if (!m_destroyed&&m_isAnimating)
    {
    	//Animation for buildings
    	if (++m_animCounter > ANIMMOVETIMER)
    	{	
    		m_animFrame++;

    		if(m_animFrame == m_animFrames)
    			m_animFrame = 2;
    		
    		m_animCounter = 0;
    	}

    }

 
    Rect src(m_animFrame * w*BLOCKSIZE,0,w*BLOCKSIZE,h*BLOCKSIZE);
    m_pic->blitTo(dest, src, UPoint(off.x+m_realPos.x-view.x*16,off.y+m_realPos.y-view.y*16));
        
    //LOG_ERROR("ObjectClass", "x-y:%d-%d, RealX-RealY:%d-%d, drawnX-Y:%d-%d", x,y, (int)m_realPos.x, (int)m_realPos.y, getDrawnX(), getDrawnY());
}

void StructureClass::update()
{

}
