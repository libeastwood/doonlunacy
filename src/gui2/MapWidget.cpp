#include "gui2/MapWidget.h"
#include "Application.h"
#include "Font.h"
#include "Gfx.h"
#include "MapGenerator.h"
#include "TerrainClass.h"

MapWidget::MapWidget()
{
    m_view = SPoint(0,0);
    m_speed = SPoint(0,0);
}

MapWidget::~MapWidget()
{
}

bool MapWidget::handleMotion(SPoint p) 
{
    if (contains(p) && p.x > x+w-10 && m_view.x < m_map->w - w /16) {
        Application::Instance()->SetCursor(CURSOR_RIGHT);
        m_speed.x = 1;
        return true; 
    }

    if (contains(p) && p.x < x+10 && m_view.x >0) { 
        Application::Instance()->SetCursor(CURSOR_LEFT);
        m_speed.x = -1;
        return true; 
    }

    if (contains(p) && p.y > y+h-10 && m_view.y < m_map->h - h /16) { 
        Application::Instance()->SetCursor(CURSOR_DOWN);
        m_speed.y = 1;
        return true; 
    }

    if (contains(p) && p.y < y+10 && m_view.y >0) {
        Application::Instance()->SetCursor(CURSOR_UP);
        m_speed.y = -1;
        return true; 
    }
    
    m_speed = SPoint(0,0);
    Application::Instance()->SetCursor(CURSOR_NORMAL);

    return false;
}

bool MapWidget::handleKeyDown(SDL_keysym* key)
{
    switch (key->sym)
    {
        case SDLK_PRINT: MapGenerator::Instance()->takeMapScreenshot(); return true;
        default: return false;
    }
}

void MapWidget::draw(Image * dest, SPoint off)
{
    // We have to be sure we're not trying to draw cell with coordinates below zero or above mapsize
    
    m_view.x += m_speed.x;
    m_view.y += m_speed.y;

    if (m_view.x < 0) { m_view.x = 0; m_speed.x = 0;}
    if (m_view.x > m_map->w - w /16) { m_view.x = m_map->w - w /16; m_speed.x = 0;}
    if (m_view.y < 0) { m_view.y = 0; m_speed.y = 0;}
    if (m_view.y > m_map->h - h /16) { m_view.y = m_map->h - h /16; m_speed.y = 0;}

    for (int i = 0; i < w / 16; i++)
    {
        for (int j = 0; j < h / 16; j++)
        {
            m_map->getCell(UPoint(i+m_view.x, j+m_view.y))->draw(dest, SPoint(off.x + x + 16*i, off.y + y + 16*j));
        }
    }
    
}

