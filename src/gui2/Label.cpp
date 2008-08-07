#include "FontManager.h"
#include "Gfx.h"
#include "Log.h"

#include "gui2/Label.h"

#include <Animation.h>

Label::Label(std::string caption, int textColor, int bgColor, int maxLineLength)
{
    m_textColor = textColor;
    m_bgColor = bgColor;
    m_maxLineLength = maxLineLength;
    m_caption = caption;
    m_redraw = true;
}

Label::~Label()
{
    m_surface.reset();
}

void Label::drawBackground(Uint16 textw, Uint16 texth, Uint16 numLines)
{
    m_surface.reset(new Image(UPoint(textw + 4-(textw%4), texth * numLines) ) );
    m_surface->recolor(0, m_bgColor);
    setSize(m_surface->getSize());
}

void Label::redraw()
{
    Font* font = FontManager::Instance()->getFont("INTRO:INTRO.FNT");
    Uint16 textw, texth;

    int numLines = 1;
    if (m_maxLineLength > 0)
    {    
        Uint16 width = 0;
        std::vector<std::string> textLines  = splitString(m_caption, m_maxLineLength);
        numLines = textLines.size();
        
        for (int i=0; i < numLines; i++)
        {
            font->extents(textLines[i].c_str(), textw, texth);
            width = (Uint16)std::max(width, textw);
        }


        LOG_INFO("Label", "Text has %d lines.", numLines);
        LOG_INFO("Label", "Width is %d", width);

        // If surface width was not %4 == 0 then you'd get a text in italics 
        drawBackground(width, texth, numLines);
        
        for (int i=0; i < numLines; i++)
        {
            font->render(textLines[i].c_str(), m_surface->getSurface(),
                0, 0 + texth * i, m_textColor);
        }
    } else
    {
        font->extents(m_caption.c_str(), textw, texth);
        drawBackground(textw, texth);
	font->render(m_caption.c_str(), m_surface->getSurface(),
        m_surface->getSurface()->w/2 - textw/2, 
        m_surface->getSurface()->h/2 - texth/2, m_textColor);
    }

}

void Label::setLabel(std::string caption)
{
    m_caption = caption;
    m_redraw = true;
}

void Label::draw(Image * dest, SPoint off)
{
    if (!m_visible) return;

    if(m_redraw)
    {
        redraw();
	m_redraw = false;
    }

    m_surface->blitTo(dest, UPoint(off.x + x, off.y + y));

}

std::vector<std::string> Label::splitString(std::string ParseString, int maxLineLength)
{
	std::vector<std::string> retVector;
	unsigned int startpos = 0;
	int linelength;
	std::string str;

    while (startpos < ParseString.size())
    {
        linelength = maxLineLength;
        if (startpos+linelength < ParseString.size())
        {
            while (ParseString.compare(startpos + linelength, 1, " ") != 0)
            {
            linelength--;
            }
        }
        
        str = ParseString.substr(startpos, linelength);
        retVector.push_back(str);
        startpos += linelength;
    }

    return retVector;
}

TransparentLabel::TransparentLabel(std::string caption, int textColor, int maxLineLength) : Label(caption, textColor, 0, maxLineLength)
{
}

void TransparentLabel::drawBackground(Uint16 textw, Uint16 texth, Uint16 numLines)
{
    m_surface.reset(new Image(UPoint(textw + 4-(textw%4), texth * numLines) ) );
    m_surface->setColorKey();
    setSize(m_surface->getSize());
}

GraphicsLabel::GraphicsLabel(Image *background, std::string caption, int textColor, int maxLineLength) : Label(caption, textColor, maxLineLength)
{
    m_background = background->getCopy();
    setSize(m_background->getSize());    
}

void GraphicsLabel::drawBackground(Uint16 textw, Uint16 texth, Uint16 numLines)
{
//    TODO: implement proper operator for Point class
//    if(m_background->getSize() < (UPoint(textw + 4-(textw%4), texth * numLines)))

    if(m_background->getSize().x < textw + 4-(textw%4)
		    ||  m_background->getSize().y < texth * numLines)
        LOG_WARNING("GraphicsLabel:", "Background image is too small to fit all text!");
    m_surface.reset();
    m_surface = m_background->getCopy();
    setSize(m_surface->getSize());    
}

GraphicsLabel::~GraphicsLabel()
{
    m_background.reset();
    m_surface.reset();
}

AnimationLabel::AnimationLabel(Animation* pAnim)
{
    m_anim = pAnim;
	m_curFrame = 0;
	m_numFrames = m_anim->getNumFrames();
	m_frameDurationTime = m_anim->getFrameDurationTime();
	
	m_curFrameStartTime = SDL_GetTicks();
}

AnimationLabel::~AnimationLabel()
{
	delete m_anim;
}

void AnimationLabel::draw(Image * screen, SPoint off)
{
    if (!m_visible) return;

	if(m_animCache.empty() || m_animCache.size() < m_curFrame + 1)
	{
		Image *surface = new Image(m_anim->getFrame());
		m_animCache.push_back(surface->getResized());
	}

	screen->blitFrom(m_animCache[m_curFrame].get(), UPoint(off.x + x, off.y + y));

	if((SDL_GetTicks() - m_curFrameStartTime) > m_frameDurationTime) {
		m_curFrameStartTime = SDL_GetTicks();
		m_curFrame++;
		if(m_curFrame >= m_numFrames)
		{
			m_curFrame = 0;
		}
	}

}
