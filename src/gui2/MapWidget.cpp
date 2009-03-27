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

MapWidget::MapWidget()
{
    m_view = SPoint(0, 0);
    m_speed = SPoint(0, 0);
    m_mouseButtonDown = false;
    new WeaponClass(NULL, "Large Rocket", UPoint(50,50), UPoint(498, 352), false);
    new WeaponClass(NULL, "Large Rocket", UPoint(50,200), UPoint(498, 372), false);
    new WeaponClass(NULL, "Large Rocket", UPoint(200,50), UPoint(498, 392), false);
    new WeaponClass(NULL, "Large Rocket", UPoint(400,400), UPoint(498, 412), false);
    new WeaponClass(NULL, "Large Rocket", UPoint(200,400), UPoint(498, 422), false);
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

bool MapWidget::handleButtonDown(Uint8 button, SPoint p)
{
    //FIXME: This is a bit lame way to get coordinates for selection on map
    //       relative to the menu..
    p.x -= getPosition().x;
    p.y -= getPosition().y;
    UPoint pos(m_view.x + p.x / BLOCKSIZE, m_view.y + p.y / BLOCKSIZE);
    GameMan* gman = GameMan::Instance();
    MapClass* m_map = gman->GetMap();

    ObjectClass * tmp = NULL;

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
                {
                    m_selectedList.front()->setSelected(false);
                }

                m_selectedList.clear();

                if (tmp != NULL)
                {
                    //TODO:Yeah. Add code to unselect units.
                    m_selectedList.clear();
                    m_selectedList.push_back(tmp);
                    tmp->setSelected(true);
                    LOG_INFO("MapWidget", "Selected unit with ID: %d", tmp->getObjectID());
                }



            }

            return true;

            break; //SDL_BUTTON_LEFT

        case SDL_BUTTON_RIGHT:

            if (!m_selectedList.empty())
            {
                tmp = m_selectedList.front();

                if (tmp->isAUnit())
                {
                    ((UnitClass*)tmp)->setDestination(pos);
                }

            }

            return true;

            break;

        default:
            return false;
            break;

    }

}

bool MapWidget::handleButtonUp(Uint8 button, SPoint p)
{
    switch (button)
    {

        case SDL_BUTTON_LEFT:
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

    m_view.x += m_speed.x;
    m_view.y += m_speed.y;

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
	    ObjectClass *object;
            if (cell->isExplored(GameMan::Instance()->LocalPlayer()->getPlayerNumber()))
		if((object = cell->getObject()))
		    object->draw(dest, SPoint(off.x + x, off.y + y), SPoint(m_view.x, m_view.y));
        }
    
    ObjectClass* tmp3;

    for (unsigned int i = 0; i < m_selectedList.size(); i++)
    {
        tmp3 = m_selectedList.at(i);
        if (tmp3->isOnScreen(Rect(m_view.x*BLOCKSIZE, m_view.y*BLOCKSIZE, w, h)))
        {
            if (tmp3->isAStructure())
                ((StructureClass*)tmp3)->drawSelectRect(dest);
            else
                ((UnitClass*)tmp3)->drawSelectionBox(dest);
        }
    }

    std::vector<ObjectClass*> *objects = GameMan::Instance()->GetObjects();
    for(std::vector<ObjectClass*>::const_iterator iter = objects->begin(); iter != objects->end(); iter++)
	if((*iter)->isWeapon())
	    (*iter)->draw(dest, SPoint(off.x + x, off.y + y), SPoint(m_view.x, m_view.y));

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

