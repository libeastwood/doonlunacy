#include <stdio.h>

#include "Definitions.h"

#include "gui2/Frame.h"
#include "DataCache.h"

Frame::Frame(Image *image, UPoint size, GuiPic_enum nw, GuiPic_enum ne,
            GuiPic_enum sw, GuiPic_enum se)
{
    if(size == NULL){
        m_surface.reset(image);
        size = m_surface->getSize();
    } else {
        m_surface.reset(new Image(size));
        m_surface->blitFrom(image, UPoint(0,0));
    }
    drawFrame(size, nw, ne, sw, se);
}
Frame::Frame(Uint32 color, ConstUPoint size, GuiPic_enum nw, GuiPic_enum ne,
            GuiPic_enum sw, GuiPic_enum se, Image *background)
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
    drawFrame(size, nw, ne, sw, se);
}
void Frame::drawFrame(ConstUPoint size, GuiPic_enum nw, GuiPic_enum ne,
            GuiPic_enum sw, GuiPic_enum se)
{
    ImagePtr tmp1;
    ImagePtr tmp2;

    tmp1 = DataCache::Instance()->getGuiPic(nw); //.get());
    m_surface->blitFrom(tmp1.get(), UPoint(0,0));
    tmp1 = DataCache::Instance()->getGuiPic(ne);
    m_surface->blitFrom(tmp1.get(), UPoint(size.x - tmp1->getSize().x, 0));
    tmp1 = DataCache::Instance()->getGuiPic(sw);
    m_surface->blitFrom(tmp1.get(), UPoint(0, size.y - tmp1->getSize().y));
    tmp1 = DataCache::Instance()->getGuiPic(se);
    m_surface->blitFrom(tmp1.get(), UPoint(size.x - tmp1->getSize().x, size.y - tmp1->getSize().y));

    tmp1 = DataCache::Instance()->getGuiPic(UI_TopBorder);
    tmp2 = DataCache::Instance()->getGuiPic(UI_BottomBorder);

    for(int i = 9; i < size.x - 12; i++){
        m_surface->blitFrom(tmp1.get(), UPoint(i, 0));
        m_surface->blitFrom(tmp2.get(), UPoint(i, size.y - tmp2->getSize().y));
    }

    tmp1 = DataCache::Instance()->getGuiPic(UI_LeftBorder);
    tmp2 = DataCache::Instance()->getGuiPic(UI_RightBorder);
    for(int i = 12; i < size.y - 10; i++){
        m_surface->blitFrom(tmp1.get(), UPoint(0, i));
        m_surface->blitFrom(tmp2.get(), UPoint(size.x - tmp2->getSize().x, i));
    }
}

Frame::~Frame()
{

}

ImagePtr Frame::getPicture(){
    return m_surface;
}

Frame1::Frame1(Image *image, UPoint size) : Frame(image,
        size, UI_Corner1NW, UI_Corner1NE, UI_Corner1SW, UI_Corner1SE)
{
}

Frame1::Frame1(Uint32 color, ConstUPoint size, Image *background) : Frame(color,
        size, UI_Corner1NW, UI_Corner1NE, UI_Corner1SW, UI_Corner1SE, background)
{
}

Frame2::Frame2(Image *image, UPoint size) : Frame(image,
        size, UI_Corner2NW, UI_Corner2NE, UI_Corner2SW, UI_Corner2SE)
{
}

Frame2::Frame2(Uint32 color, ConstUPoint size, Image *background) : Frame(color,
        size, UI_Corner3NW, UI_Corner2NE, UI_Corner2SW, UI_Corner2SE, background)
{
}

Frame3::Frame3(Image *image, UPoint size) : Frame(image,
        size, UI_Corner3NW, UI_Corner3NE, UI_Corner3SW, UI_Corner3SE)
{
}

Frame3::Frame3(Uint32 color, ConstUPoint size, Image *background) : Frame(color,
        size, UI_Corner3NW, UI_Corner3NE, UI_Corner3SW, UI_Corner3SE, background)
{
}

