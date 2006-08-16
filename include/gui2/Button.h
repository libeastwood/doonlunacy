#ifndef DUNE_GUI2_BUTTON
#define DUNE_GUI2_BUTTON

#include "gui2/Widget.h"
#include "boost/signal.hpp"
#include <string>

class Button : public Widget
{
    public:
        boost::signal<void ()> onClick;
        
        virtual bool handleButtonUp(Uint8 button, SPoint p);
};


class GraphicButton : public Button
{
    public:
        GraphicButton();
        GraphicButton(SDL_Surface* normal, SDL_Surface* pressed=NULL);
            
        void setGraphics(SDL_Surface* normal, 
                         SDL_Surface* pressed = NULL);

        virtual void draw(SDL_Surface* dest, SPoint off);

        virtual bool handleButtonDown(Uint8 button, SPoint p);
        virtual bool handleButtonUp(Uint8 button, SPoint p);

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

        //virtual void setSize(Uint16 w, Uint16 h);
        std::string getCaption() { return m_caption;} ;
        void setCaption(std::string newcaption);
		virtual void redraw();
    protected:
        std::string m_caption;
};

class TranspButton : public Button
{
    public:
        TranspButton(Uint16 w, Uint16 h);
        ~TranspButton();
};


#endif // DUNE_GUI2_BUTTON

