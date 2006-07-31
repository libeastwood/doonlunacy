#ifndef DUNE_GUI2_BUTTON
#define DUNE_GUI2_BUTTON

#include "gui2/Widget.h"
#include "boost/signal.hpp"
#include <string>

class Button : public Widget
{
    public:
        boost::signal<void ()> onClick;
        
        virtual bool handleButtonUp(Uint8 button, Uint16 x, Uint16 y);
};


class GraphicButton : public Button
{
    public:
        GraphicButton();
        GraphicButton(SDL_Surface* normal, SDL_Surface* pressed=NULL);
            
        void setGraphics(SDL_Surface* normal, 
                         SDL_Surface* pressed = NULL);

        virtual void draw(SDL_Surface* dest, Uint16 offx=0, Uint16 offy=0);

        virtual bool handleButtonDown(Uint8 button, Uint16 x, Uint16 y);
        virtual bool handleButtonUp(Uint8 button, Uint16 x, Uint16 y);

    protected:
        SDL_Surface* m_surfNormal;
        SDL_Surface* m_surfPressed;

        bool m_pressed;
};


class BoringButton : public GraphicButton
{
    public:
        BoringButton(std::string caption);
        ~BoringButton();

        virtual void setSize(Uint16 w, Uint16 h);

    protected:
        std::string m_caption;

        virtual void redraw();
};

#endif // DUNE_GUI2_BUTTON

