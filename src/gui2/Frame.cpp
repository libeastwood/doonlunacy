#include <stdio.h>

#include "Definitions.h"

#include "gui2/Container.h"
#include "gui2/Frame.h"
#include "DataCache.h"

Frame::Frame(Image *image, UPoint size, GuiPic_enum nw, GuiPic_enum ne,
            GuiPic_enum sw, GuiPic_enum se, Uint16 edgeDistance)
{
    if(size == NULL){
        m_surface.reset(image);
        size = m_surface->getSize();
    } else {
        m_surface.reset(new Image(size));
        m_surface->blitFrom(image);
    }
    drawFrame(size, nw, ne, sw, se, edgeDistance);
}

Frame::Frame(Image *image)
{
    m_surface.reset(image);
}

Frame::Frame()
{
    // Needs to be initialized with a proper surface..
    m_surface.reset(new Image(UPoint(1,1)));
}

Frame::Frame(Uint32 color, ConstUPoint size, GuiPic_enum nw, GuiPic_enum ne,
            GuiPic_enum sw, GuiPic_enum se, Image *background, Uint16 edgeDistance)
{
    m_surface.reset(new Image(size));
    if(background == NULL)
        m_surface->fillRect(color, Rect(0, 0, size.x, size.y));
    else
    {
        UPoint bgSize = background->getSize();
        for(int x = 0; x < size.x; x += bgSize.x - 1)
            for(int y = 0; y < size.y; y += bgSize.y - 1)
                m_surface->blitFrom(background, UPoint(x,y));
    }
    drawFrame(size, nw, ne, sw, se, edgeDistance);
}
void Frame::drawFrame(ConstUPoint size, GuiPic_enum nw, GuiPic_enum ne,
            GuiPic_enum sw, GuiPic_enum se, Uint16 edgeDistance)
{
    ImagePtr corner_nw, corner_ne, corner_sw, corner_se, top, bottom, left, right;
    
    corner_nw = DataCache::Instance()->getGuiPic(nw);
    m_surface->blitFrom(corner_nw.get(), UPoint(edgeDistance,edgeDistance));
    corner_ne = DataCache::Instance()->getGuiPic(ne);
    m_surface->blitFrom(corner_ne.get(), UPoint(size.x - corner_ne->getSize().x - edgeDistance, edgeDistance));
    corner_sw = DataCache::Instance()->getGuiPic(sw);
    m_surface->blitFrom(corner_sw.get(), UPoint(edgeDistance, size.y - corner_sw->getSize().y - edgeDistance));
    corner_se = DataCache::Instance()->getGuiPic(se);
    m_surface->blitFrom(corner_se.get(), UPoint(size.x - corner_se->getSize().x - edgeDistance, size.y - corner_se->getSize().y - edgeDistance));

    top = DataCache::Instance()->getGuiPic(UI_TopBorder);
    bottom = DataCache::Instance()->getGuiPic(UI_BottomBorder);

    for(int i = corner_se->getSize().x; i < size.x - corner_se->getSize().y; i++){
        m_surface->blitFrom(top.get(), UPoint(i, edgeDistance));
        m_surface->blitFrom(top.get(), UPoint(i, size.y - bottom->getSize().y - edgeDistance));
    }

    left = DataCache::Instance()->getGuiPic(UI_LeftBorder);
    right = DataCache::Instance()->getGuiPic(UI_RightBorder);
    for(int i = corner_ne->getSize().y; i < size.y - corner_se->getSize().x; i++){
        m_surface->blitFrom(left.get(), UPoint(edgeDistance, i));
        m_surface->blitFrom(right.get(), UPoint(size.x - right->getSize().x - edgeDistance, i));
    }
}

void Frame::draw(Image *dest, SPoint off){
    if (!m_visible) return;

    m_surface->blitTo(dest, UPoint(off.x + x, off.y + y));

    WidgetList::iterator it;
    for ( it  = m_children.begin();
          it != m_children.end();
          ++it )
    {
        (*it)->draw(dest, SPoint(off.x + x, off.y + y));
    };
}

Frame::~Frame()
{

}

void Frame::changeBackground(Image *background)
{
    m_surface.reset(background);
}

ImagePtr Frame::getPicture(){
    return m_surface;
}

Frame1::Frame1(Image *image, UPoint size, Uint16 edgeDistance) : Frame(image,
        size, UI_Corner1NW, UI_Corner1NE, UI_Corner1SW, UI_Corner1SE, edgeDistance)
{
}

Frame1::Frame1(Uint32 color, ConstUPoint size, Image *background, Uint16 edgeDistance) : Frame(color,
        size, UI_Corner1NW, UI_Corner1NE, UI_Corner1SW, UI_Corner1SE, background, edgeDistance)
{
}

Frame2::Frame2(Image *image, UPoint size, Uint16 edgeDistance) : Frame(image,
        size, UI_Corner2NW, UI_Corner2NE, UI_Corner2SW, UI_Corner2SE, edgeDistance)
{
}

Frame2::Frame2(Uint32 color, ConstUPoint size, Image *background, Uint16 edgeDistance) : Frame(color,
        size, UI_Corner2NW, UI_Corner2NE, UI_Corner2SW, UI_Corner2SE, background, edgeDistance)
{
}

Frame3::Frame3(Image *image, UPoint size, Uint16 edgeDistance) : Frame(image,
        size, UI_Corner3NW, UI_Corner3NE, UI_Corner3SW, UI_Corner3SE, edgeDistance)
{
}

Frame3::Frame3(Uint32 color, ConstUPoint size, Image *background, Uint16 edgeDistance) : Frame(color,
        size, UI_Corner3NW, UI_Corner3NE, UI_Corner3SW, UI_Corner3SE, background, edgeDistance)
{
}

