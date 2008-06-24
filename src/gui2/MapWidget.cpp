#include "Application.h"
#include "Font.h"
#include "Log.h"
#include "Gfx.h"
#include "MapGenerator.h"
#include "TerrainClass.h"

#include "gui2/MapWidget.h"
#include "structures/StructureClass.h"
#include "units/UnitClass.h"
MapWidget::MapWidget()
{
    m_view = SPoint(0, 0);
    m_speed = SPoint(0, 0);
}

MapWidget::~MapWidget()
{
}

bool MapWidget::handleMotion(SPoint p)
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

    return false;
}

void MapWidget::setGameState(GameState* gs)
{
    m_map = gs->m_map;
    m_structures = gs->m_structures;
    m_units = gs->m_units;
}

bool MapWidget::handleKeyDown(SDL_keysym* key)
{
    switch (key->sym)
    {

        case SDLK_PRINT:
            MapGenerator::Instance()->takeMapScreenshot();
            return true;

        default:
            return false;
    }
}

bool MapWidget::handleButtonDown(Uint8 button, SPoint p)
{
    UPoint pos(m_view.x + p.x / BLOCKSIZE, m_view.y + p.y / BLOCKSIZE);

    ObjectClass * tmp = NULL;

    switch (button)
    {

        case SDL_BUTTON_LEFT:

            if (m_map->cellExists(pos))
            {
                tmp = m_map->getCell(pos)->getObject();
                LOG_INFO("MapWidget", "%d-%d", pos.x, pos.y);

                if (tmp != NULL)
                {
                    //TODO:Yeah. Add code to unselect units.
                    m_selectedList.clear();
                    m_selectedList.push_back(tmp);
                    tmp->setSelected(true);
                }

                else
                {
                    if (!m_selectedList.empty())
                    {
                        m_selectedList.front()->setSelected(false);
                    }

                    m_selectedList.clear();
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

    return false;
}

void MapWidget::draw(Image * dest, SPoint off)
{
    // We have to be sure we're not trying to draw cell with coordinates below zero or above mapsize


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
    {
        for (int j = 0; j < h / BLOCKSIZE; j++)
        {
            m_map->getCell(UPoint(i + m_view.x, j + m_view.y))->draw(dest, SPoint(off.x + x + BLOCKSIZE*i, off.y + y + BLOCKSIZE*j));
        }
    }

    assert (m_structures != NULL);

    StructureClass* tmp;

    for (unsigned int i = 0; i < m_structures->size(); i++)
    {
        tmp = (StructureClass*)m_structures->at(i);
        tmp->update();

        if (tmp->isOnScreen(Rect(m_view.x*BLOCKSIZE, m_view.y*BLOCKSIZE, w, h)))
        {
            tmp->draw(dest, SPoint(off.x + x, off.y + y), SPoint(m_view.x, m_view.y));
        }
    }

    UnitClass* tmp2;

    for (unsigned int j = 0; j < m_units->size(); j++)
    {
        tmp2 = (UnitClass*)m_units->at(j);
        tmp2->update();

        if (tmp2->isOnScreen(Rect(m_view.x*BLOCKSIZE, m_view.y*BLOCKSIZE, w, h)))
        {
            tmp2->draw(dest, SPoint(off.x + x, off.y + y), SPoint(m_view.x, m_view.y));

        }
    }

    ObjectClass* tmp3;

    for (unsigned int i = 0; i < m_selectedList.size(); i++)
    {
        tmp3 = m_selectedList.at(i);

        if (tmp3->isOnScreen(Rect(m_view.x*BLOCKSIZE, m_view.y*BLOCKSIZE, w, h)))
        {
            if (tmp3->isAStructure())
            {
                ((StructureClass*)tmp3)->drawSelectRect(dest);
            }

            else
            {
                ((UnitClass*)tmp3)->drawSelectionBox(dest);
            }
        }
    }

}

