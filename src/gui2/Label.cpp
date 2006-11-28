#include "gui2/Label.h"
#include <stdio.h>
#include "Colours.h"
#include "Font.h"
#include "Application.h"
#include "Gfx.h"

Label::Label(std::string caption, int bgcolour)
{
    m_caption = caption;
    Font* font = FontManager::Instance()->getFont("INTRO:INTRO.FNT");

    Uint16 textw, texth;

    font->extents(m_caption.c_str(), textw, texth);

    /*If surface width was not %4 == 0 then you'd get a text in italics */
    m_surface  = SDL_CreateRGBSurface(SDL_SWSURFACE, textw + 4-(textw%4) , texth, 8,
                                        0, 0, 0, 0);

    assert(m_surface != NULL);

    SDL_LockSurface(m_surface);

    SDL_Palette* pal = Application::Instance()->Screen()->format->palette;

    SDL_SetColors(m_surface, pal->colors, 0, pal->ncolors);

    SDL_FillRect(m_surface, NULL, bgcolour);

    font->render(m_caption.c_str(), m_surface,
                    m_surface->w/2 - textw/2, 
                    m_surface->h/2 - texth/2, 49);
    SDL_UnlockSurface(m_surface);

//    Is it needed in case of label. It's not clickable or anything.
//    Widget::setSize(SPoint(textw, texth));
};

Label::~Label()
{

};

void Label::draw(SDL_Surface* dest, SPoint off)
{
    if (!m_visible) return;

    Rect destrect (off.x + x, off.y + y, 0, 0);

    assert(m_surface != NULL);
    SDL_BlitSurface(m_surface, NULL, dest, &destrect);

}
