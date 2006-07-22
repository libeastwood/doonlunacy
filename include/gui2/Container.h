#ifndef DUNE_GUI_CONTAINER_H
#define DUNE_GUI_CONTAINER_H

#include "gui2/Widget.h"
#include <list>

class Container: public Widget
{
    public:
        typedef std::list<Widget*> WidgetList;

        virtual void draw(SDL_Surface* dest, Uint16 offx=0, Uint16 offy=0);

        virtual bool handleMotion(Uint16 x, Uint16 y);
        virtual bool handleButtonDown(Uint8 button, Uint16 x, Uint16 y);
        virtual bool handleButtonUp(Uint8 button, Uint16 x, Uint16 y);

        virtual bool handleKeyDown(SDL_keysym* key);
        virtual bool handleKeyUp(SDL_keysym* key);

        virtual void addChild(Widget* child);
        virtual void deleteChild(Widget* child);
        virtual void clear() { m_children.clear(); }
    
    protected:
        WidgetList m_children;    
    
};


#endif // DUNE_GUI_CONTAINER_H
