#include "gui2/Button.h"
#include <stdio.h>
#include "Colours.h"
#include "Font.h"
#include "Application.h"
#include "Gfx.h"

bool Button::handleButtonUp(Uint8 button, Uint16 x, Uint16 y)
{
    if (!m_visible) return false;
    if (!hitTest(x, y)) return false;
    onClick();
    return true;
};

//-------------------------------------------------------------------

GraphicButton::GraphicButton()
{
    m_surfNormal = NULL;
    m_surfPressed = NULL;
    m_pressed = false;
};

GraphicButton::GraphicButton(SDL_Surface* normal, SDL_Surface* pressed)
{
    setGraphics(normal, pressed);
    m_pressed = false;
};

void GraphicButton::setGraphics(SDL_Surface* normal, SDL_Surface* pressed)
{
    assert(pressed != NULL && ((normal->w == pressed->w) &&
                               (normal->h == pressed->h)));

    m_surfNormal = normal;
    m_surfPressed = pressed;

    m_width = normal->w;
    m_height = normal->h;
};

void GraphicButton::draw(SDL_Surface* dest, Uint16 offx, Uint16 offy)
{
    if (!m_visible) return;
    
    SDL_Rect destrect;
    destrect.x = offx + m_x;
    destrect.y = offy + m_y;

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
};

bool GraphicButton::handleButtonDown(Uint8 button, Uint16 x, Uint16 y)
{
    if (!m_visible) return false;   
    if (!hitTest(x, y)) 
    {
        m_pressed = false;
        return false;
    }

    m_pressed = true;
    
    return true;
};

bool GraphicButton::handleButtonUp(Uint8 button, Uint16 x, Uint16 y)
{
    if (!m_visible) return false;   
    m_pressed = false;
    return Button::handleButtonUp(button, x, y);
};

// ------------------------------------------------------------------

BoringButton::BoringButton(std::string caption)
{
    m_caption = caption;
};

BoringButton::~BoringButton()
{
    SDL_FreeSurface(m_surfNormal);
    SDL_FreeSurface(m_surfPressed);
};

void BoringButton::setSize(Uint16 w, Uint16 h)
{
    GraphicButton::setSize(w, h);
    redraw();
};

void BoringButton::setCaption(std::string newcaption)
{
    m_caption = newcaption;
    redraw();
};

void BoringButton::redraw()
{
    if (m_surfNormal != NULL) SDL_FreeSurface(m_surfNormal);
    if (m_surfPressed != NULL) SDL_FreeSurface(m_surfPressed);

    m_surfNormal  = SDL_CreateRGBSurface(SDL_SWSURFACE, m_width, m_height, 8,
                                        0, 0, 0, 0); 
    assert(m_surfNormal != NULL);

    m_surfPressed = SDL_CreateRGBSurface(SDL_SWSURFACE, m_width, m_height, 8,
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
    drawHLine(m_surfNormal, 0, 0, m_width-1, 229, false);
    drawHLine(m_surfNormal, 0, 1, m_width-3, 108, false);

    // left lines
    drawVLine(m_surfNormal, 0, 0, m_height-1, 229, false);
    drawVLine(m_surfNormal, 1, 1, m_height-2, 108, false);
   
    // bottom lines
    drawHLine(m_surfNormal, 1, m_height-2, m_width-2, 226, false);
    drawHLine(m_surfNormal, 0, m_height-1, m_width-1, 229, false);
    
    // right lines
    drawVLine(m_surfNormal, m_width-1, 0, m_height-1, 229, false);
    drawVLine(m_surfNormal, m_width-2, 1, m_height-2, 226, false);
    
    // final pixels to make it look really duneish
    putPixel(m_surfNormal, 1, m_height-2, 115);
    putPixel(m_surfNormal, m_width-2, 1, 115);
    putPixel(m_surfNormal, m_width-2, m_height-2, 227);


    /*
     * Button pressed
     */   
    // top lines 
    drawHLine(m_surfPressed, 0, 0, m_width-1, 229, false);
    drawHLine(m_surfPressed, 0, 1, m_width-3, 226, false);

    // left lines
    drawVLine(m_surfPressed, 0, 0, m_height-1, 229, false);
    drawVLine(m_surfPressed, 1, 1, m_height-2, 226, false);
   
    // bottom lines
    drawHLine(m_surfPressed, 1, m_height-2, m_width-2, 226, false);
    drawHLine(m_surfPressed, 0, m_height-1, m_width-1, 229, false);
    
    // right lines
    drawVLine(m_surfPressed, m_width-1, 0, m_height-1, 229, false);
    drawVLine(m_surfPressed, m_width-2, 1, m_height-2, 226, false);
    
    // final pixels to make it look really duneish
    putPixel(m_surfPressed, 1, m_height-2, 227);
    putPixel(m_surfPressed, m_width-2, 1, 227);
    putPixel(m_surfPressed, m_width-2, m_height-2, 227);
    
    Font* font = FontManager::Instance()->getFont("INTRO:INTRO.FNT");

    Uint16 textw, texth;
    font->extents(m_caption.c_str(), textw, texth);

    font->render(m_caption.c_str(), m_surfNormal,
                    (m_width / 2) - (textw / 2), 
                    (m_height / 2) - (texth / 2), 49);
    font->render(m_caption.c_str(), m_surfPressed,
                    (m_width / 2) - (textw / 2), 
                    (m_height / 2) - (texth / 2), 49);

	SDL_UnlockSurface(m_surfNormal);
    SDL_UnlockSurface(m_surfPressed);
};

// ------------------------------------------------------------------

TranspButton::TranspButton(Uint16 w, Uint16 h)
{
    Button::setSize(w, h);
};

TranspButton::~TranspButton()
{

};
