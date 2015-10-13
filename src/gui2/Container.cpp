#include "gui2/Container.h"

#include <assert.h>

Container::Container() : Widget()
{
    m_parent = true;
}

Container::~Container()
{
    while(!m_children.empty())
    {
        Widget *child = m_children.back();
        deleteChild(child);
        delete child;
    }
}

void Container::draw(Image& dest, SPoint off)
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

bool Container::handleButtonDown(uint8_t button, SPoint p)
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

bool Container::handleButtonUp(uint8_t button, SPoint p)
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

int Container::getChildrenSize()
{
    return m_children.size();
}

bool Container::fadeInChildren(const int fadeAmt)
{
    if (!m_visible) return false;
	bool fade = false;
    WidgetList::iterator it;
    for ( it  = m_children.begin();
          it != m_children.end();
          ++it )
    {
        if ((*it)->parent() && ((Container*)(*it))->fadeInChildren(fadeAmt)) fade = true;
		if((*it)->fadeIn(fadeAmt)) fade = true;
    };

    return fade;
}

bool Container::fadeOutChildren(const int fadeAmt)
{
    if (!m_visible) return false;
	bool fade = false;
    WidgetList::iterator it;
    for ( it  = m_children.begin();
          it != m_children.end();
          ++it )
    {
        if ((*it)->parent() && ((Container*)(*it))->fadeOutChildren(fadeAmt)) fade = true;
		if((*it)->fadeOut(fadeAmt)) fade = true;
    };

    return fade;
}

void Container::recolorChildrenByHouse(HOUSETYPE house)
{
    WidgetList::iterator it;
    for ( it  = m_children.begin();
          it != m_children.end();
          ++it )
    {
        (*it)->recolorByHouse(house);
    };
}
