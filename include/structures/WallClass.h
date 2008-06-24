#ifndef DUNE_STRUCTURES_WALLCLASS_H
#define DUNE_STRUCTURES_WALLCLASS_H

#include "structures/StructureClass.h"
#include "PlayerClass.h"

class WallClass : public StructureClass
{
public:
    WallClass(PlayerClass* newOwner);
	~WallClass();

	inline void setTile(int newTile) {
		switch(newTile) {
			case Structure_w1:
				m_curAnimFrame = m_firstAnimFrame = m_lastAnimFrame = 5;
				break;
			case Structure_w2:
				m_curAnimFrame = m_firstAnimFrame = m_lastAnimFrame = 6;
				break;
			case Structure_w3:
				m_curAnimFrame = m_firstAnimFrame = m_lastAnimFrame = 10;
				break;
			case Structure_w4:
				m_curAnimFrame = m_firstAnimFrame = m_lastAnimFrame = 11;
				break;
			case Structure_w5:
				m_curAnimFrame = m_firstAnimFrame = m_lastAnimFrame = 8;
				break;
			case Structure_w6:
				m_curAnimFrame = m_firstAnimFrame = m_lastAnimFrame = 4;
				break;
			case Structure_w7:
				m_curAnimFrame = m_firstAnimFrame = m_lastAnimFrame = 12;
				break;
			case Structure_w8:
				m_curAnimFrame = m_firstAnimFrame = m_lastAnimFrame = 2;
				break;
			case Structure_w9:
				m_curAnimFrame = m_firstAnimFrame = m_lastAnimFrame = 9;
				break;
			case Structure_w10:
				m_curAnimFrame = m_firstAnimFrame = m_lastAnimFrame = 3;
				break;
			case Structure_w11:
				m_curAnimFrame = m_firstAnimFrame = m_lastAnimFrame = 7;
				break;
			default:
				m_curAnimFrame = m_firstAnimFrame = m_lastAnimFrame = 2;
				break;				
		}
	}
};

#endif // DUNE_STRUCTURES_WALLCLASS_H
