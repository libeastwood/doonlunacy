#include "gui2/VBox.h"

#include <assert.h>

void VBox::reshape()
{
    Uint16 d = m_height / m_children.size(); 

    Uint16 y = 0;

    WidgetList::iterator it;
    
    for (it =  m_children.begin();
         it != m_children.end();
         ++it)
    {
        (*it)->setPos(0, y);
        y += d;
    }; 
}

void VBox::fit(int margin)
{
    assert(m_children.size() > 0);
    
    Widget* child = *(m_children.begin());
    
    m_width = child->getWidth();
    m_height = child->getHeight() * m_children.size();
    m_height += m_children.size() * margin;
}






