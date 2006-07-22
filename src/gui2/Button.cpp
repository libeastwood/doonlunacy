#include "gui2/Button.h"
#include <stdio.h>

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
        SDL_BlitSurface(m_surfPressed, NULL, dest, &destrect);
    }
    else
    {
        SDL_BlitSurface(m_surfNormal, NULL, dest, &destrect);
    };
};

bool GraphicButton::handleButtonDown(Uint8 button, Uint16 x, Uint16 y)
{
    if (!m_visible) return false;   
    printf("%p down\n", this);
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
    printf("%p up\n", this);
    return Button::handleButtonUp(button, x, y);
};





