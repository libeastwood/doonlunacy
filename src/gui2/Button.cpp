#include "gui2/Button.h"
#include <stdio.h>
#include "Colours.h"
#include "Font.h"
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
    m_surfNormal = NULL;
    m_surfPressed = NULL;
    m_pressed = false;
}

GraphicButton::GraphicButton(SDL_Surface* normal, SDL_Surface* pressed)
{
    setGraphics(normal, pressed);
    m_pressed = false;
}

void GraphicButton::setGraphics(SDL_Surface* normal, SDL_Surface* pressed)
{
    assert(pressed != NULL && ((normal->w == pressed->w) &&
                               (normal->h == pressed->h)));

    m_surfNormal = normal;
    m_surfPressed = pressed;

    w = normal->w;
    h = normal->h;
}

void GraphicButton::draw(SDL_Surface* dest, SPoint off)
{
    if (!m_visible) return;

    Rect destrect (off.x + x, off.y + y, 0, 0);

    if (m_pressed)
    {
        assert(m_surfPressed != NULL);
        SDL_BlitSurface(m_surfPressed, NULL, dest, &destrect);
    }
    else
    {
        assert(m_surfNormal != NULL);
        SDL_BlitSurface(m_surfNormal, NULL, dest, &destrect);
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

BoringButton::BoringButton(std::string caption)
{
    m_caption = caption;
}

BoringButton::~BoringButton()
{
    SDL_FreeSurface(m_surfNormal);
    SDL_FreeSurface(m_surfPressed);
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
    if (m_surfNormal != NULL) SDL_FreeSurface(m_surfNormal);
    if (m_surfPressed != NULL) SDL_FreeSurface(m_surfPressed);

    m_surfNormal  = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 8,
                                        0, 0, 0, 0); 
    assert(m_surfNormal != NULL);

    m_surfPressed = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 8,
                                        0, 0, 0, 0); 
    assert(m_surfPressed != NULL);

    SDL_LockSurface(m_surfNormal);
    SDL_LockSurface(m_surfPressed);

    SDL_Palette* pal = Application::Instance()->Screen()->format->palette;

    SDL_SetColors(m_surfNormal, pal->colors, 0, pal->ncolors);
    SDL_SetColors(m_surfPressed, pal->colors, 0, pal->ncolors);

    SDL_FillRect(m_surfNormal, NULL, 115);
    SDL_FillRect(m_surfPressed, NULL, 116);
    
    /*
     * Button normal
     */   
    // top lines 
    drawHLine(m_surfNormal, 0, 0, w-1, 229, false);
    drawHLine(m_surfNormal, 0, 1, w-3, 108, false);

    // left lines
    drawVLine(m_surfNormal, 0, 0, h-1, 229, false);
    drawVLine(m_surfNormal, 1, 1, h-2, 108, false);
   
    // bottom lines
    drawHLine(m_surfNormal, 1, h-2, w-2, 226, false);
    drawHLine(m_surfNormal, 0, h-1, w-1, 229, false);
    
    // right lines
    drawVLine(m_surfNormal, w-1, 0, h-1, 229, false);
    drawVLine(m_surfNormal, w-2, 1, h-2, 226, false);
    
    // final pixels to make it look really duneish
    putPixel(m_surfNormal, 1, h-2, 115);
    putPixel(m_surfNormal, w-2, 1, 115);
    putPixel(m_surfNormal, w-2, h-2, 227);


    /*
     * Button pressed
     */   
    // top lines 
    drawHLine(m_surfPressed, 0, 0, w-1, 229, false);
    drawHLine(m_surfPressed, 0, 1, w-3, 226, false);

    // left lines
    drawVLine(m_surfPressed, 0, 0, h-1, 229, false);
    drawVLine(m_surfPressed, 1, 1, h-2, 226, false);
   
    // bottom lines
    drawHLine(m_surfPressed, 1, h-2, w-2, 226, false);
    drawHLine(m_surfPressed, 0, h-1, w-1, 229, false);
    
    // right lines
    drawVLine(m_surfPressed, w-1, 0, h-1, 229, false);
    drawVLine(m_surfPressed, w-2, 1, h-2, 226, false);
    
    // final pixels to make it look really duneish
    putPixel(m_surfPressed, 1, h-2, 227);
    putPixel(m_surfPressed, w-2, 1, 227);
    putPixel(m_surfPressed, w-2, h-2, 227);
    
    Font* font = FontManager::Instance()->getFont("INTRO:INTRO.FNT");

    Uint16 textw, texth;
    font->extents(m_caption.c_str(), textw, texth);

    font->render(m_caption.c_str(), m_surfNormal,
                    (w / 2) - (textw / 2), 
                    ((h / 2) - (texth / 2)), 49);
    font->render(m_caption.c_str(), m_surfPressed,
                    (w / 2) - (textw / 2), 
                    ((h / 2) - (texth / 2)), 49);

	SDL_UnlockSurface(m_surfNormal);
    SDL_UnlockSurface(m_surfPressed);
}

// ------------------------------------------------------------------

TranspButton::TranspButton(Uint16 w, Uint16 h)
{
	setSize(SPoint(w, h));
}

TranspButton::~TranspButton()
{

}
