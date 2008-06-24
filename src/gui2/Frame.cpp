#include <stdio.h>

#include "gui2/Frame.h"
#include "DataCache.h"

Frame::Frame(Image *image, UPoint size)
{
    if(size == NULL){
        m_surface.reset(image);
        size = m_surface->getSize();
    } else {
        m_surface.reset(new Image(size));
        m_surface->blitFrom(image, UPoint(0,0));
    }

    m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_Corner1NW).get(), UPoint(0,0));
    m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_Corner1NE).get(), UPoint(size.x - 8, 0));
    m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_Corner1SW).get(), UPoint(0, size.y - 8));
    m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_Corner1SE).get(), UPoint(size.x - 8,  size.y - 8));

    for(int i = 5; i < size.x - 8; i++){
        m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_TopBorder).get(), UPoint(i, 0));
        m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_BottomBorder).get(), UPoint(i, size.y - 4));
    }
   
    for(int i = 5; i < size.y - 8; i++){
        m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_LeftBorder).get(), UPoint(0, i));
        m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_RightBorder).get(), UPoint(size.x - 4, i));
    }
}

Frame::Frame(Uint32 color, ConstUPoint size, Image *background){
    m_surface.reset(new Image(size));
    m_surface->fillRect(color, Rect(0, 0, size.x, size.y));
    m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_Corner1NW).get(), UPoint(0,0));
    m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_Corner1NE).get(), UPoint(size.x - 8, 0));
    m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_Corner1SW).get(), UPoint(0, size.y - 8));
    m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_Corner1SE).get(), UPoint(size.x - 8,  size.y - 8));

    for(int i = 5; i < size.x - 8; i++){
        m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_TopBorder).get(), UPoint(i, 0));
        m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_BottomBorder).get(), UPoint(i, size.y - 4));
    }
   
    for(int i = 5; i < size.y - 8; i++){
        m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_LeftBorder).get(), UPoint(0, i));
        m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_RightBorder).get(), UPoint(size.x - 4, i));
    }
}

Frame::~Frame()
{

}

ImagePtr Frame::getPicture(){
    return m_surface;
}

Frame2::Frame2(Image *image, UPoint size) : Frame(image, size)
{
    if(size == NULL){
        m_surface.reset(image);
        size = m_surface->getSize();
    } else {
        m_surface.reset(new Image(size));
        m_surface->blitFrom(image, UPoint(0,0));
    }

    m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_Corner2NW).get(), UPoint(0,0));
    m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_Corner2NE).get(), UPoint(size.x - 12, 0));
    m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_Corner2SW).get(), UPoint(0, size.y - 11));
    m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_Corner2SE).get(), UPoint(size.x - 11,  size.y - 11));

    for(int i = 5; i < size.x - 12; i++){
        m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_TopBorder).get(), UPoint(i, 0));
        m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_BottomBorder).get(), UPoint(i, size.y - 4));
    }
   
    for(int i = 5; i < size.y - 10; i++){
        m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_LeftBorder).get(), UPoint(0, i));
        m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_RightBorder).get(), UPoint(size.x - 4, i));
    }
}

Frame2::Frame2(Uint32 color, ConstUPoint size, Image *background) : Frame(color, size, background){
    m_surface.reset(new Image(size));
    m_surface->fillRect(color, Rect(0, 0, size.x, size.y));

    m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_Corner2NW).get(), UPoint(0,0));
    m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_Corner2NE).get(), UPoint(size.x - 12, 0));
    m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_Corner2SW).get(), UPoint(0, size.y - 11));
    m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_Corner2SE).get(), UPoint(size.x - 11,  size.y - 11));

    for(int i = 9; i < size.x - 12; i++){
        m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_TopBorder).get(), UPoint(i, 0));
        m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_BottomBorder).get(), UPoint(i, size.y - 4));
    }
   
    for(int i = 12; i < size.y - 10; i++){
        m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_LeftBorder).get(), UPoint(0, i));
        m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_RightBorder).get(), UPoint(size.x - 4, i));
    }
}

Frame3::Frame3(Image *image, UPoint size) : Frame(image, size)
{
    if(size == NULL){
        m_surface.reset(image);
        size = m_surface->getSize();
    } else {
        m_surface.reset(new Image(size));
        m_surface->blitFrom(image, UPoint(0,0));
    }

    m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_Corner3NW).get(), UPoint(0,0));
    m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_Corner3NE).get(), UPoint(size.x - 12, 0));
    m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_Corner3SW).get(), UPoint(0, size.y - 12));
    m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_Corner3SE).get(), UPoint(size.x - 12,  size.y - 12));

    for(int i = 5; i < size.x - 12; i++){
        m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_TopBorder).get(), UPoint(i, 0));
        m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_BottomBorder).get(), UPoint(i, size.y - 4));
    }
   
    for(int i = 5; i < size.y - 10; i++){
        m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_LeftBorder).get(), UPoint(0, i));
        m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_RightBorder).get(), UPoint(size.x - 4, i));
    }
}

Frame3::Frame3(Uint32 color, ConstUPoint size, Image *background) : Frame(color, size, background){
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

    m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_Corner3NW).get(), UPoint(0,0));
    m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_Corner3NE).get(), UPoint(size.x - 12, 0));
    m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_Corner3SW).get(), UPoint(0, size.y - 12));
    m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_Corner3SE).get(), UPoint(size.x - 12,  size.y - 12));

    for(int i = 9; i < size.x - 12; i++){
        m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_TopBorder).get(), UPoint(i, 0));
        m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_BottomBorder).get(), UPoint(i, size.y - 4));
    }
   
    for(int i = 12; i < size.y - 10; i++){
        m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_LeftBorder).get(), UPoint(0, i));
        m_surface->blitFrom(DataCache::Instance()->getGuiPic(UI_RightBorder).get(), UPoint(size.x - 4, i));
    }
}


