#ifndef DUNE_GUI_CONTAINER_H
#define DUNE_GUI_CONTAINER_H

#include "gui2/Widget.h"
#include <list>

class Container: public Widget
{
    public:
	Container();
	~Container();
        typedef std::list<Widget*> WidgetList;

        virtual void draw(Image * dest, SPoint offset);

		virtual bool handleMotion(SPoint p);
        virtual bool handleButtonDown(Uint8 button, SPoint p);
        virtual bool handleButtonUp(Uint8 button, SPoint p);

        virtual bool handleKeyDown(SDL_keysym* key);
        virtual bool handleKeyUp(SDL_keysym* key);

        virtual void addChild(Widget* child);
        virtual void deleteChild(Widget* child);
        virtual void clear() { m_children.clear(); }
        virtual int getChildrenSize();
    
    protected:
        WidgetList m_children;    
    
};


#endif // DUNE_GUI_CONTAINER_H
