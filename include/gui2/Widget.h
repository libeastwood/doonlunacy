#ifndef DUNE_GUI2_WIDGET
#define DUNE_GUI2_WIDGET

#include "SDL.h"

class Widget 
{
    public:
        Widget();
        virtual ~Widget();

        virtual void draw(SDL_Surface* dest, Uint16 offx = 0, Uint16 offy = 0) {};

        virtual bool handleMotion(Uint16 x, Uint16 y);
        virtual bool handleButtonDown(Uint8 button, Uint16 x, Uint16 y);
        virtual bool handleButtonUp(Uint8 button, Uint16 x, Uint16 y);

        virtual bool handleKeyDown(SDL_keysym* key) { return false; }
        virtual bool handleKeyUp(SDL_keysym* key) { return false; }

        virtual void setSize(Uint16 w, Uint16 h) { m_width = w; m_height = h; }
        inline Uint16 getWidth() { return m_width; }
        inline Uint16 getHeight() { return m_height; }
        
        virtual void setPos(Uint16 x, Uint16 y) { m_x = x; m_y = y; }
        virtual void setX(Uint16 x) { m_x = x; }
        virtual void setY(Uint16 y) { m_y = y; }
        inline Uint16 getX() { return m_x; }
        inline Uint16 getY() { return m_y; }

        inline bool hitTest(Uint16 x, Uint16 y);
        
        virtual void setEnabled(bool enabled) { m_enabled = enabled; }
        inline bool enabled() { return m_enabled; }

        virtual void setFocus();
        inline bool focused() { return (m_focusedWidget == this); }
        static Widget* getFocused() { return m_focusedWidget; }

        virtual void setVisible(bool visible) { m_visible = visible; }
        inline bool visible() { return m_visible; }

    protected:
        virtual void gotFocus() {};
        virtual void lostFocus() {};
        
        Uint16 m_x, m_y, m_width, m_height;
        bool m_enabled;
        bool m_visible;

        static Widget* m_focusedWidget;
};

inline bool Widget::hitTest(Uint16 x, Uint16 y)
{
    return ((x > m_x) && (x < m_x + m_width) &&
            (y > m_y) && (y < m_y + m_height));
};

#endif // DUNE_GUI2_WIDGET

