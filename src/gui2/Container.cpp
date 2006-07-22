#include "gui2/Container.h"

#include <assert.h>

void Container::draw(SDL_Surface* dest, Uint16 offx, Uint16 offy)
{
    if (!m_visible) return;

    WidgetList::iterator it;
    for ( it  = m_children.begin();
          it != m_children.end();
          ++it )
    {
        (*it)->draw(dest, offx + m_x, offy + m_y);
    };
};

bool Container::handleMotion(Uint16 x, Uint16 y)
{
    if (!m_visible) return false;
    if (!hitTest(x, y)) return false;
    
    WidgetList::iterator it;
    for ( it  = m_children.begin();
          it != m_children.end();
          ++it )
    {
        if ((*it)->handleMotion(x - m_x, y - m_y)) return true;
    };

    return false;
};

bool Container::handleButtonDown(Uint8 button, Uint16 x, Uint16 y)
{
    if (!m_visible) return false;
    if (!hitTest(x, y)) return false;

    WidgetList::iterator it;
    for ( it  = m_children.begin();
          it != m_children.end();
          ++it )
    {
        if ((*it)->handleButtonDown(button, x - m_x, y- m_y)) return true;
    };

    return false;
};

bool Container::handleButtonUp(Uint8 button, Uint16 x, Uint16 y)
{
    if (!m_visible) return false;
    if (!hitTest(x, y)) return false;

    WidgetList::iterator it;
    for ( it  = m_children.begin();
          it != m_children.end();
          ++it )
    {
        if ((*it)->handleButtonUp(button, x - m_x, y - m_y)) return true;
    };

    return false;
};

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
};

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
};

void Container::addChild(Widget* child)
{
    m_children.push_back(child);
};

void Container::deleteChild(Widget* child)
{
    m_children.remove(child);
};




