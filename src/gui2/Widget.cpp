#include "gui2/Widget.h"

Widget* Widget::m_focusedWidget = NULL;

Widget::Widget()
{
    m_x = 0;
    m_y = 0;
    m_width = 0;
    m_height = 0;
    m_enabled = true;
    m_visible = true;
};

Widget::~Widget()
{

};

bool Widget::handleMotion(Uint16 x, Uint16 y)
{
    if (hitTest(x, y)) return true;
    return false;
};

bool Widget::handleButtonDown(Uint8 button, Uint16 x, Uint16 y)
{
    if (hitTest(x, y)) return true;
    return false;
};

bool Widget::handleButtonUp(Uint8 button, Uint16 x, Uint16 y)
{
    if (hitTest(x, y)) return true;
    return false;
};

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
};
