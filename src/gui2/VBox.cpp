#include "gui2/VBox.h"

#include <assert.h>

void VBox::reshape()
{
    Uint16 d = h / m_children.size(); 

    Uint16 y = 0;

    WidgetList::iterator it;
    
    for (it =  m_children.begin();
         it != m_children.end();
         ++it)
    {
        (*it)->setPosition(SPoint(0, y));
        y += d;
    }; 
}

void VBox::fit(int margin)
{
    assert(m_children.size() > 0);
    
    Widget* child = *(m_children.begin());
    
    w = child->w;
    h = child->h * m_children.size() + (m_children.size() * margin);
}






