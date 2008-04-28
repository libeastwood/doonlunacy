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
    m_surface.reset(new Image(UPoint(textw + 4-(textw%4) , texth)));

    m_surface->fillRect(bgcolour);

    font->render(m_caption.c_str(), m_surface,
                    m_surface->getSurface()->w/2 - textw/2, 
                    m_surface->getSurface()->h/2 - texth/2, 49);

//    Is it needed in case of label. It's not clickable or anything.
//    Widget::setSize(SPoint(textw, texth));
}

Label::~Label()
{

}

void Label::draw(Image * dest, SPoint off)
{
    if (!m_visible) return;

    m_surface->blitTo(dest, UPoint(off.x + x, off.y + y));

}

AnimationLabel::AnimationLabel(Animation* pAnim)
{
	m_anim = pAnim;
}

AnimationLabel::~AnimationLabel()
{

}
void AnimationLabel::draw(Image * screen, SPoint off)
{
    if (!m_visible) return;

	Image * surface = m_anim->getFrame();
	
	screen->blitFrom(surface->getResized(2).get(), UPoint(off.x + x, off.y + y));
}
