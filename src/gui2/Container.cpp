#include "gui2/Container.h"

#include <assert.h>

void Container::draw(SDL_Surface* dest, SPoint off)
{
    if (!m_visible) return;

    WidgetList::iterator it;
    for ( it  = m_children.begin();
          it != m_children.end();
          ++it )
    {
        (*it)->draw(dest, SPoint(off.x + x, off.y + y));
    };
}

bool Container::handleMotion(SPoint p)
{
    if (!m_visible) return false;
	if (!contains(p)) return false;
    
	SPoint op (p.x - x, p.y - y);

    WidgetList::iterator it;
    for ( it  = m_children.begin();
          it != m_children.end();
          ++it )
    {
        if ((*it)->handleMotion(op)) return true;
    };

    return false;
}

bool Container::handleButtonDown(Uint8 button, SPoint p)
{
    if (!m_visible) return false;
    if (!contains(p)) return false;

	SPoint op (p.x - x, p.y - y);

    WidgetList::iterator it;
    for ( it  = m_children.begin();
          it != m_children.end();
          ++it )
    {
        if ((*it)->handleButtonDown(button, op)) return true;
    };

    return false;
}

bool Container::handleButtonUp(Uint8 button, SPoint p)
{
    if (!m_visible) return false;
    if (!contains(p)) return false;

	SPoint op (p.x - x, p.y - y);

    WidgetList::iterator it;
    for ( it  = m_children.begin();
          it != m_children.end();
          ++it )
    {
        if ((*it)->handleButtonUp(button, op)) return true;
    };

    return false;
}

bool Container::handleKeyDown(SDL_keysym *key)
{
    if (!m_visible) return false;
    WidgetList::iterator it;
    for ( it  = m_children.begin();
          it != m_children.end();
          ++it )
    {
        if ((*it)->handleKeyDown(key)) return true;
    };

    return false;
}

bool Container::handleKeyUp(SDL_keysym *key)
{
    if (!m_visible) return false;
    WidgetList::iterator it;
    for ( it  = m_children.begin();
          it != m_children.end();
          ++it )
    {
        if ((*it)->handleKeyUp(key)) return true;
    };

    return false;
}

void Container::addChild(Widget* child)
{
    m_children.push_back(child);
}

void Container::deleteChild(Widget* child)
{
    m_children.remove(child);
}




