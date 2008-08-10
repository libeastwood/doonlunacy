#include <stdio.h>

#include "Application.h"
#include "Colours.h"
#include "Font.h"
#include "Gfx.h"
#include "Log.h"
#include "Strings.h"
#include "gui2/Label.h"

Label::Label(std::string caption, int textColour, int bgColour, int maxLineLength)
{
    m_textColour = textColour;
    m_bgColour = bgColour;
    m_maxLineLength = maxLineLength;
    setLabel(caption);
}

void Label::redraw()
{
    Font* font = FontManager::Instance()->getFont("INTRO:INTRO.FNT");
    Uint16 textw, texth;

    if (m_maxLineLength > 0)
    {    
        Uint16 width = 0;

        std::vector<String> lines;
        StringInputCache str(m_caption);
        String out = "";

        while (!str.isEos())
        {
            if ((out.length() + str.peekWord().length()) < m_maxLineLength)
            {
                out+= str.getWord();
                out+= " ";
                str.advance();
            }
            else
            {
                lines.push_back(out);
                out = "";
            }
        }

        int numLines = lines.size();
        
        for (int i=0; i < numLines; i++)
        {
            font->extents(lines[i].c_str(), textw, texth);
            width = (Uint16)std::max(width, textw);
        }


        LOG_INFO("Label", "Text has %d lines.", numLines);
        LOG_INFO("Label", "Width is %d", width);

        //If surface width was not %4 == 0 then you'd get a text in italics 
        m_surface.reset(new Image(UPoint(width + 4-(width%4), texth * numLines) ) );
        if (m_bgColour < 0)
        {
            //Make it transparent
            m_surface->setColorKey();
        } else 
        {
            m_surface->fillRect(m_bgColour);
        }

        
        for (int i=0; i < numLines; i++)
        {
            font->render(lines[i].c_str(), m_surface,
                0, 0 + texth * i, m_textColour);
        }
    } else
    {
        font->extents(m_caption.c_str(), textw, texth);
        m_surface.reset(new Image(UPoint(textw + 4-(textw%4), texth) ) );
        m_surface->fillRect(m_bgColour);
        font->render(m_caption.c_str(), m_surface,
            m_surface->getSurface()->w/2 - textw/2, 
            m_surface->getSurface()->h/2 - texth/2, m_textColour);
    }

}

void Label::setLabel(std::string caption)
{
    m_caption = caption;
    redraw();
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
    delete m_anim;
}
void AnimationLabel::draw(Image * screen, SPoint off)
{
    if (!m_visible) return;

	Image * surface = m_anim->getFrame().get();

	screen->blitFrom(surface, UPoint(off.x + x, off.y + y));
}
