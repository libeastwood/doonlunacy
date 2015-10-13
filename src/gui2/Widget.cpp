#include "gui2/Widget.h"

Widget* Widget::m_focusedWidget = NULL;

Widget::Widget() : Rect(0, 0, 0, 0), m_enabled(true), m_visible(true), m_parent(false)
{
}

bool Widget::handleMotion(SPoint p)
{
	if (contains(p)) return true;
    return false;
}

bool Widget::handleButtonDown(uint8_t button, SPoint p)
{
    if (contains(p)) return true;
    return false;
}

bool Widget::handleButtonUp(uint8_t button, SPoint p)
{
    if (contains(p)) return true;
    return false;
}

void Widget::setFocus()
{
    if (focused()) return;
    
    Widget* cur = getFocused();

    if (cur != NULL)
    {
        cur->lostFocus();
    }; 

    m_focusedWidget = this;

    this->gotFocus();
}

bool Widget::fadeIn(const int fadeAmt)
{
	if(!m_surface) return false;
	m_enabled = !m_surface->fadeIn(fadeAmt);
	return !m_enabled;
}

bool Widget::fadeOut(const int fadeAmt)
{
	if(m_enabled) m_enabled = false;
	if(m_surface)
		return m_surface->fadeOut(fadeAmt);
	return false;
}

void Widget::recolorByHouse(HOUSETYPE house)
{
    if(m_surface) m_surface->recolorByHouse(house);
} 

