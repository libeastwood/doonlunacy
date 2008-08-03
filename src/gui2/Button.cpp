#include "gui2/Button.h"
#include <stdio.h>
#include "Colours.h"
#include "FontManager.h"
#include "Application.h"
#include "Gfx.h"

bool Button::handleButtonUp(Uint8 button, SPoint p)
{
    if (!m_visible) return false;
    if (!contains(p)) return false;
    onClick();
    return true;
}

//-------------------------------------------------------------------

GraphicButton::GraphicButton()
{
    m_pressed = false;
}

GraphicButton::GraphicButton(ImagePtr normal, ImagePtr pressed)
{
    setGraphics(normal, pressed);
    m_pressed = false;
}

void GraphicButton::setGraphics(ImagePtr normal, ImagePtr pressed)
{
    assert(pressed != NULL && ((normal->getSurface()->w == pressed->getSurface()->w) &&
                               (normal->getSurface()->h == pressed->getSurface()->h)));

    m_surfNormal = normal;
    m_surfPressed = pressed;

    setSize(normal->getSize());
    //w = normal->getSurfacew;
    //h = normal->h;
}

void GraphicButton::draw(Image * dest, SPoint off)
{
    if (!m_visible) return;

    if (m_pressed)
    {
        m_surfPressed->blitTo(dest, UPoint(off.x + x, off.y + y));
    }
    else
    {
        m_surfNormal->blitTo(dest, UPoint(off.x + x, off.y + y));
    };
}

bool GraphicButton::handleButtonDown(Uint8 button, SPoint p)
{
    if (!m_visible) return false;   
    if (!contains(p)) 
    {
        m_pressed = false;
        return false;
    }

    m_pressed = true;
    
    return true;
}

bool GraphicButton::handleButtonUp(Uint8 button, SPoint p)
{
    if (!m_visible) return false;   
    m_pressed = false;
    return Button::handleButtonUp(button, p);
}

// ------------------------------------------------------------------

BoringButton::BoringButton(std::string caption, bool isMenuButton)
{
    m_caption = caption;
    m_menuButton = isMenuButton;
}

BoringButton::~BoringButton()
{

}


void BoringButton::setSize(SPoint p)
{
    Widget::setSize(p);
    redraw();
}


void BoringButton::setCaption(std::string newcaption)
{
    m_caption = newcaption;
    redraw();
}

void BoringButton::redraw()
{
    m_surfNormal.reset(new Image(UPoint(w,h)));
    m_surfPressed.reset(new Image(UPoint(w,h)));                               

    if (m_menuButton)
    {

        m_surfNormal->fillRect(115);
        m_surfPressed->fillRect(116);
        
        /*
         * Button normal
         */   
        // top lines 
        m_surfNormal->drawHLine(UPoint(0, 0), w-1, 229, false);
        m_surfNormal->drawHLine(UPoint(0, 1), w-3, 108, false);

        // left lines
        m_surfNormal->drawVLine(UPoint(0, 0), h-1, 229, false);
        m_surfNormal->drawVLine(UPoint(1, 1), h-2, 108, false);
       
        // bottom lines
        m_surfNormal->drawHLine(UPoint(1, h-2), w-2, 226, false);
        m_surfNormal->drawHLine(UPoint(0, h-1), w-1, 229, false);
        
        // right lines
        m_surfNormal->drawVLine(UPoint(w-1, 0), h-1, 229, false);
        m_surfNormal->drawVLine(UPoint(w-2, 1), h-2, 226, false);
        
        // final pixels to make it look really duneish
        m_surfNormal->putPixel(UPoint(1, h-2), 115);
        m_surfNormal->putPixel(UPoint(w-2, 1), 115);
        m_surfNormal->putPixel(UPoint(w-2, h-2), 227);


        /*
         * Button pressed
         */   
        // top lines 
        m_surfPressed->drawHLine(UPoint(0, 0), w-1, 229, false);
        m_surfPressed->drawHLine(UPoint(0, 1), w-3, 226, false);

        // left lines
        m_surfPressed->drawVLine(UPoint(0, 0), h-1, 229, false);
        m_surfPressed->drawVLine(UPoint(1, 1), h-2, 226, false);
       
        // bottom lines
        m_surfPressed->drawHLine(UPoint(1, h-2), w-2, 226, false);
        m_surfPressed->drawHLine(UPoint(0, h-1), w-1, 229, false);
        
        // right lines
        m_surfPressed->drawVLine(UPoint(w-1, 0), h-1, 229, false);
        m_surfPressed->drawVLine(UPoint(w-2, 1), h-2, 226, false);
        
        // final pixels to make it look really duneish
        m_surfPressed->putPixel(UPoint(1, h-2), 227);
        m_surfPressed->putPixel(UPoint(w-2, 1), 227);
        m_surfPressed->putPixel(UPoint(w-2, h-2), 227);
        
    } else
    {
        Rect rect(0,0, w, h);
        m_surfNormal->fillRect(133);
        m_surfPressed->fillRect(134);
        
        m_surfNormal->drawRect(rect, 0);
        m_surfPressed->drawRect(rect, 0);
        
        m_surfNormal->drawHLine(UPoint(1, 1), w-2, 132, false);
        m_surfNormal->drawVLine(UPoint(1, 2), h-2, 132, false);
        
    }
    
    Font* font = FontManager::Instance()->getFont("INTRO:INTRO.FNT");

    Uint16 fontColour;
    
    m_menuButton ? fontColour = 49 : fontColour = 105;

    Uint16 textw, texth;
    font->extents(m_caption.c_str(), textw, texth);

    font->render(m_caption.c_str(), m_surfNormal->getSurface(),
                    (w / 2) - (textw / 2), 
                    ((h / 2) - (texth / 2)), fontColour);
    font->render(m_caption.c_str(), m_surfPressed->getSurface(),
                    (w / 2) - (textw / 2), 
                    ((h / 2) - (texth / 2)), fontColour);


}

// ------------------------------------------------------------------

TranspButton::TranspButton(SPoint size)
{
	setSize(size);
}

TranspButton::~TranspButton()
{

}
