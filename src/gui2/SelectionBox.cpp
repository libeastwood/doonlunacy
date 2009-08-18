#include "gui2/SelectionBox.h"
#include "gui2/Button.h"
#include <iostream>

SelectionBox::SelectionBox(bool multi) : Container(), m_multi(multi)
{
}

bool SelectionBox::handleButtonDown(uint8_t button, SPoint p)
{
    if (!m_visible) return false;
    if (!contains(p)) return false;

    SPoint op (p.x - x, p.y - y);

    WidgetList::iterator it;
    ButtonList::iterator it2;
    for ( it  = m_children.begin(), it2 = m_buttons.begin();
          it != m_children.end();
          ++it )
    {
	if(*it == *it2)
	{
	    //FIXME: Ugly, but what the hell..
	    if((*it)->contains(op)) {
		if(m_multi) {
		    if((*it2)->m_pressed) {
			if ((*it)->handleButtonUp(button, op)) return true;
		    } else {
			if ((*it)->handleButtonDown(button, op)) return true;
		    }
		}
		else {
		    if(!(*it2)->m_pressed)
			if ((*it)->handleButtonDown(button, op)) {
			    //FIXME: maybe a new onHit() or something in handleButtonDown()?
			    (*it)->handleButtonUp(button, op);
			    (*it)->handleButtonDown(button, op);
			    for(ButtonList::iterator it3 = m_buttons.begin();
				    it3 != m_buttons.end();
				    it3++) {
				if(*it3 != *it2)
				    (*it3)->handleButtonUp(button, op);
			    }
			    return true;
			}
		}
	    }
	    it2++;
	}
	else
    	    if ((*it)->handleButtonDown(button, op)) return true;
    };

    return false;
}

void SelectionBox::addChild(GraphicButton *child)
{
    m_buttons.push_back(child);
    if(!m_multi && m_buttons.size() == 1)
    	child->m_pressed = true;
    m_children.push_back(child);
}

