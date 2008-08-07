#include "gui2/Widget.h"

Widget* Widget::m_focusedWidget = NULL;

Widget::Widget() : Rect(0, 0, 0, 0), m_enabled(true), m_visible(true), m_parent(false)
{
}

Widget::~Widget()
{

}

bool Widget::handleMotion(SPoint p)
{
	if (contains(p)) return true;
    return false;
}

bool Widget::handleButtonDown(Uint8 button, SPoint p)
{
    if (contains(p)) return true;
    return false;
}

bool Widget::handleButtonUp(Uint8 button, SPoint p)
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
