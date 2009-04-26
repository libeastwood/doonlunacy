#include "FontManager.h"
#include "Gfx.h"
#include "Log.h"
#include "Strings.h"

#include "gui2/Label.h"

Label::Label(std::string caption, int textColor, int bgColor, int maxLineLength)
{
    m_textColor = textColor;
    m_bgColor = bgColor;
    m_maxLineLength = maxLineLength;
    m_caption = caption;
    m_redraw = true;
	m_resizeRatio = 0;
	m_resize = false;
}

Label::~Label()
{
}

void Label::drawBackground(Uint16 textw, Uint16 texth, Uint16 numLines)
{
    m_surface.reset(new Image(UPoint(textw + 4-(textw%4), texth * numLines) ) );
    m_surface->recolor(0, m_bgColor);
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


        LOG(LV_INFO, "Label", "Text has %d lines.", numLines);
        LOG(LV_INFO, "Label", "Width is %d", width);

        // If surface width was not %4 == 0 then you'd get a text in italics 
        drawBackground(width, texth, numLines);
        
        for (int i=0; i < numLines; i++)
            m_surface->renderText(lines[i].c_str(), font, 0, 0 + texth * i, m_textColor);
    } else
    {
        font->extents(m_caption.c_str(), textw, texth);
        drawBackground(textw, texth);
	m_surface->renderText(m_caption.c_str(), font,
        m_surface->getSize().x/2 - textw/2, 
        m_surface->getSize().y/2 - texth/2, m_textColor);
    }

	// This is quite lame, but will do for now..
	if(m_resizeRatio)
		m_surface = m_surface->getResized(m_resizeRatio);
	if(m_resize)
		m_surface = m_surface->getResized();
	setSize(m_surface->getSize());
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

TransparentLabel::TransparentLabel(std::string caption, int textColor, int maxLineLength) : Label(caption, textColor, 0, maxLineLength)
{
}

void TransparentLabel::drawBackground(Uint16 textw, Uint16 texth, Uint16 numLines)
{
    m_surface.reset(new Image(UPoint(textw + 4-(textw%4), texth * numLines) ) );
    m_surface->setColorKey();
}

GraphicsLabel::GraphicsLabel(ImagePtr background, std::string caption, int textColor, int maxLineLength) : Label(caption, textColor, maxLineLength)
{
    m_background = background;
	redraw();
}


void GraphicsLabel::drawBackground(Uint16 textw, Uint16 texth, Uint16 numLines)
{
//    TODO: implement proper operator for Point class
//    if(m_background->getSize() < (UPoint(textw + 4-(textw%4), texth * numLines)))

    if(m_background->getSize().x < textw + 4-(textw%4)
		    ||  m_background->getSize().y < texth * numLines)
        LOG(LV_WARNING, "GraphicsLabel:", "Background image is too small to fit all text!");

	if(!m_surface)
		m_surface = m_background->getCopy();
	else
		m_surface->blitFrom(m_background.get());
}

GraphicsLabel::~GraphicsLabel()
{
}

AnimationLabel::AnimationLabel(ConstUPoint origSize)
{
	m_curFrameStartTime = SDL_GetTicks();
	m_frameDurationTime = 1;
	m_curFrame = 0;
	m_origSize = origSize;
	setSize(m_origSize.getScaled());
}

AnimationLabel::AnimationLabel()
{
	m_curFrameStartTime = SDL_GetTicks();
	m_frameDurationTime = 1;
	m_curFrame = 0;
	m_origSize = UPoint(0,0);
}

AnimationLabel::~AnimationLabel()
{
}

void AnimationLabel::addFrame(ImagePtr animFrame, bool setColorKey) {
	if(m_origSize == UPoint(0,0))
		m_origSize = animFrame->getSize(), setSize(m_origSize.getScaled());
	else if(animFrame->getSize() != m_origSize)
        LOG(LV_ERROR, "AnimationLabel:", "Frame size %dx%d doesn't match %dx%d!",
				animFrame->getSize().x, animFrame->getSize().y, m_origSize.x, m_origSize.y);
	assert(animFrame->getSize() == m_origSize);

	if(setColorKey)
		animFrame->setColorKey();
	
	m_animFrames.push_back(animFrame);
}


void AnimationLabel::draw(Image * screen, SPoint off)
{
    if (!m_visible || m_animFrames.empty()) return;

	m_surface = m_animFrames[m_curFrame];
	if(m_surface->getSize() != getSize())
	{
		m_surface = m_surface->getResized();
		m_animFrames[m_curFrame] = m_surface;
	}

	screen->blitFrom(m_surface.get(), UPoint(off.x + x, off.y + y));

	if((SDL_GetTicks() - m_curFrameStartTime) > m_frameDurationTime && m_enabled) {
		m_curFrameStartTime = SDL_GetTicks();
		m_curFrame++;
		if(m_curFrame >= m_animFrames.size()) {
			m_curFrame = 0;
		}		
	}
}
