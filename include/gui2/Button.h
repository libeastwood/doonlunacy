#ifndef DUNE_GUI2_BUTTON
#define DUNE_GUI2_BUTTON

#include "gui2/Widget.h"
#include "boost/signal.hpp"
#include <string>

class SelectionBox;

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
        GraphicButton(ImagePtr normal, ImagePtr pressed);
            
        void setGraphics(ImagePtr normal, ImagePtr pressed);

        virtual void draw(Image * dest, SPoint off);

        virtual bool handleButtonDown(Uint8 button, SPoint p);
        virtual bool handleButtonUp(Uint8 button, SPoint p);

    protected:
    	friend class SelectionBox;

        ImagePtr m_surfNormal;
        ImagePtr m_surfPressed;

        bool m_pressed;
};


class BoringButton : public GraphicButton
{
    public:
        BoringButton(std::string caption, bool isMenuButton = true);
        ~BoringButton();

        virtual void setSize(SPoint p);
        std::string getCaption() { return m_caption;} ;
        void setCaption(std::string newcaption);
		virtual void redraw();
    protected:
        std::string m_caption;
        bool m_menuButton;
};

class TransparentButton : public Button
{
    public:
        TransparentButton(SPoint size);
        ~TransparentButton();
};


#endif // DUNE_GUI2_BUTTON

