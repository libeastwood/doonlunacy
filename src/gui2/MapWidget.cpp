#include "Application.h"
#include "FontManager.h"
#include "GameMan.h"
#include "Gfx.h"
#include "Log.h"
#include "MapGenerator.h"
#include "TerrainClass.h"

#include "gui2/MapWidget.h"
#include "objects/StructureClass.h"
#include "objects/UnitClass.h"

#include <stack>
MapWidget::MapWidget()
{
    m_view = SPoint(0, 0);
    m_speed = SPoint(0, 0);
    m_mouseButtonDown = false;
    m_keyPressed = SDLK_UNKNOWN;
    m_modPressed = KMOD_NONE; 
    /*new WeaponClass(NULL, "Large Rocket", UPoint(50,200), UPoint(498, 372), false);
    new WeaponClass(NULL, "Large Rocket", UPoint(200,50), UPoint(498, 392), false);
    new WeaponClass(NULL, "Large Rocket", UPoint(400,400), UPoint(498, 412), false);
    new WeaponClass(NULL, "Large Rocket", UPoint(200,400), UPoint(498, 422), false);*/
}

MapWidget::~MapWidget()
{
}

bool MapWidget::handleMotion(SPoint p)
{
    MapClass* m_map = GameMan::Instance()->GetMap();
    
    if (m_mouseButtonDown)
    {
        m_selectEnd = p;
    }
    else
    {
    
        if (contains(p) && p.x > x + w - 10 && m_view.x < m_map->w - w / BLOCKSIZE)
        {
            Application::Instance()->SetCursor(CURSOR_RIGHT);
            m_speed.x = 1;
            return true;
        }

        if (contains(p) && p.x < x + 10 && m_view.x > 0)
        {
            Application::Instance()->SetCursor(CURSOR_LEFT);
            m_speed.x = -1;
            return true;
        }

        if (contains(p) && p.y > y + h - 10 && m_view.y < m_map->h - h / BLOCKSIZE)
        {
            Application::Instance()->SetCursor(CURSOR_DOWN);
            m_speed.y = 1;
            return true;
        }

        if (contains(p) && p.y < y + 10 && m_view.y > 0)
        {
            Application::Instance()->SetCursor(CURSOR_UP);
            m_speed.y = -1;
            return true;
        }

        m_speed = SPoint(0, 0);

        Application::Instance()->SetCursor(CURSOR_NORMAL);

    }

    return false;
}

bool MapWidget::handleKeyDown(SDL_keysym* key)
{
    m_keyPressed = key->sym;
    m_modPressed = key->mod;
    switch (key->sym)
    {
        case SDLK_PRINT:
            GameMan::Instance()->TakeMapScreenshot();
            return true;
        case SDLK_ESCAPE:
            Application::Instance()->RootState()->PopState();
            return true;
        default:
            return false;
    }
}

bool MapWidget::handleKeyUp(SDL_keysym* key)
{
    m_keyPressed = SDLK_UNKNOWN;
    m_modPressed = key->mod;    
    return true;
}

bool MapWidget::handleButtonDown(Uint8 button, SPoint p)
{
    //FIXME: This is a bit lame way to get coordinates for selection on map
    //       relative to the menu..
    UPoint pos(m_view + ((p - getPosition()) / BLOCKSIZE));
    GameMan* gman = GameMan::Instance();
    MapClass* m_map = gman->GetMap();
    ObjectPtr tmp;

    switch (button)
    {
        case SDL_BUTTON_LEFT:

            m_mouseButtonDown = true;
            m_selectStart = p;

            if (m_map->cellExists(pos))
            {
                tmp = m_map->getCell(pos)->getObject();
                LOG_INFO("MapWidget", "%d-%d", pos.x, pos.y);


                if (!m_selectedList.empty())
                    (*m_selectedList.begin()->second).setSelected(false);

		if(m_keyPressed != SDLK_LSHIFT)
		    m_selectedList.clear();

		if (tmp)
		{
		    if(m_keyPressed == SDLK_LSHIFT && m_selectedList.find(tmp->getObjectID()) != m_selectedList.end())
			m_selectedList.erase(tmp->getObjectID());
		    else
			if(m_selectedList.empty() || (!m_selectedList.empty()
				    && (*m_selectedList.begin()->second).getOwner() == GameMan::Instance()->LocalPlayer()
				    && tmp->getOwner() == GameMan::Instance()->LocalPlayer())) {
			    m_selectedList[tmp->getObjectID()] = tmp;
			    tmp->setSelected(true);
			    LOG_INFO("MapWidget", "Selected unit with ID: %d", tmp->getObjectID());
			}
		}
	    }

            return true;

            break; //SDL_BUTTON_LEFT

        case SDL_BUTTON_RIGHT:

            if (!m_selectedList.empty())
            {
		Uint32 status = STATUS_NONE;
		ObjectPtr targetObj;
		if(m_keyPressed == SDLK_LCTRL || ((targetObj = m_map->getCell(pos)->getObject()) && targetObj->getOwner() != GameMan::Instance()->LocalPlayer()))
		    status |= STATUS_ATTACKING;

		for(ObjectMap::const_iterator unit = m_selectedList.begin(); unit != m_selectedList.end(); unit++)
    		    if ((*unit).second->isControllable() && (*unit).second->isAUnit())
    			    ((UnitClass*)((*unit).second).get())->setDestination(pos, status);
            }

            return true;

            break;

        default:
            return false;
            break;

    }
    return false;
}

bool MapWidget::handleButtonUp(Uint8 button, SPoint p)
{
    GameMan* gman = GameMan::Instance();
    MapClass* m_map = gman->GetMap();

    UPoint start(m_view + ((m_selectStart - getPosition()) / BLOCKSIZE));
    UPoint end(m_view + ((m_selectEnd - getPosition()) / BLOCKSIZE));

    if(start > end)
	std::swap(start,end);

    UPoint pos;
    switch (button)
    {
	case SDL_BUTTON_LEFT:
	    if(m_selectedList.empty() || m_selectedList.begin()->second->getOwner() == GameMan::Instance()->LocalPlayer())
		for(pos.x = start.x; pos.x <= end.x; pos.x++)
		    for(pos.y = start.y; pos.y <= end.y; pos.y++) {
			if (m_map->cellExists(pos)) {
			    ObjectPtr tmp = m_map->getCell(pos)->getObject();
			    LOG_DEBUG("MapWidget", "multi: %d-%d", pos.x, pos.y);
			    if(tmp) {
				if(tmp->getOwner() == GameMan::Instance()->LocalPlayer() && tmp->isAUnit()) {
				    m_selectedList[tmp->getObjectID()] = tmp;
				    tmp->setSelected(true);
				    LOG_INFO("MapWidget", "Selected unit with ID: %d at %d-%d", tmp->getObjectID(), pos.x, pos.y);
				}
			    }
			}
		    }
            m_selectRect = Rect(0,0,0,0);
            m_selectEnd = UPoint(0,0);
            m_selectStart = UPoint(0,0);
            m_mouseButtonDown = false;
            return true;
        default:
            return false;
    }

    return false;
}

void MapWidget::draw(Image * dest, SPoint off)
{
    GameMan* gman = GameMan::Instance();
    MapClass* m_map = gman->GetMap();
    // We have to be sure we're not trying to draw cell with coordinates below zero or above mapsize
    TerrainClass* cell;

    assert (m_map != NULL);

    m_view += m_speed;

    if (m_view.x < 0)
    {
        m_view.x = 0;
        m_speed.x = 0;
    }

    if (m_view.x > m_map->w - w / BLOCKSIZE)
    {
        m_view.x = m_map->w - w / BLOCKSIZE;
        m_speed.x = 0;
    }

    if (m_view.y < 0)
    {
        m_view.y = 0;
        m_speed.y = 0;
    }

    if (m_view.y > m_map->h - h / BLOCKSIZE)
    {
        m_view.y = m_map->h - h / BLOCKSIZE;
        m_speed.y = 0;
    }

    //FIXME:This needs to be optimised. Why to redraw the whole map all the time? It takes a lot of CPU
    for (int i = 0; i < w / BLOCKSIZE; i++)
        for (int j = 0; j < h / BLOCKSIZE; j++)
        {
            cell = m_map->getCell(UPoint(i + m_view.x, j + m_view.y));
            cell->draw(dest, SPoint(off.x + x + BLOCKSIZE*i, off.y + y + BLOCKSIZE*j));
        }

    std::stack<ObjectMap> attributeKeys;
    for(ObjectTypeMap::const_iterator objTypeMap = GameMan::Instance()->getObjectsBegin(); objTypeMap != GameMan::Instance()->getObjectsEnd(); objTypeMap++) {
	uint32_t attribute = objTypeMap->first;
	if(attribute & OBJECT_WEAPON || attribute & OBJECT_AIRUNIT) {
	    attributeKeys.push((*objTypeMap).second);
	    continue;
	}
	for(ObjectMap::const_iterator objMap = (*objTypeMap).second.begin(); objMap != (*objTypeMap).second.end(); objMap++) {
	    ObjectPtr object = objMap->second;
	    if(m_map->getCell(SPoint(object->x, object->y))->isExplored(GameMan::Instance()->LocalPlayer()->getPlayerNumber()))
		object->draw(dest, SPoint(off.x + x, off.y + y), m_view);
	}
    }
    while(!attributeKeys.empty()) {
	for(ObjectMap::const_iterator objMap = attributeKeys.top().begin(); objMap != attributeKeys.top().end(); objMap++) {
	    ObjectPtr object = objMap->second;
	    if(m_map->getCell(SPoint(object->x, object->y))->isExplored(GameMan::Instance()->LocalPlayer()->getPlayerNumber()))
    		object->draw(dest, SPoint(off.x + x, off.y + y), m_view);
	}
	attributeKeys.pop();
    }

    for (ObjectMap::const_iterator iter = m_selectedList.begin(); iter != m_selectedList.end(); iter++)
        if ((*iter).second->isOnScreen(Rect(m_view.x*BLOCKSIZE, m_view.y*BLOCKSIZE, w, h)))
	    (*iter).second->drawSelectionBox(dest);


    if (m_mouseButtonDown && m_selectEnd!= UPoint(0,0))
    {
        if (m_selectStart.x < m_selectEnd.x)
        {
            m_selectRect.x = m_selectStart.x;
            m_selectRect.w = m_selectEnd.x - m_selectStart.x;
        }
        else
        {
            m_selectRect.x = m_selectEnd.x;
            m_selectRect.w = m_selectStart.x - m_selectEnd.x;
        }
        
        if (m_selectStart.y < m_selectEnd.y)
        {
            m_selectRect.y = m_selectStart.y;
            m_selectRect.h = m_selectEnd.y - m_selectStart.y;
        }
        else
        {
            m_selectRect.y = m_selectEnd.y;
            m_selectRect.h = m_selectStart.y - m_selectEnd.y;
        }
        
        dest->drawRect(m_selectRect, 255);
    }

}

