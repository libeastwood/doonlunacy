#include "DataCache.h"
#include "DrawImage.h"
#include "ResMan.h"
#include "GCObject.h"
#include "pakfile/Cpsfile.h"

DrawImage::DrawImage(ConstUPoint size, Uint32 color) : Image(size)
{
	if(color != (Uint32)-1)
		fillRect(color, Rect(0, 0, size.x, size.y));
}

DrawImage::DrawImage(Image *image) : Image(image->getSurface())
{
}

DrawImage::~DrawImage()
{
}


void DrawImage::drawBorders(ImagePtr corner_nw, ImagePtr corner_ne,
		ImagePtr corner_sw, ImagePtr corner_se, ImagePtr top,
		ImagePtr bottom, ImagePtr left, ImagePtr right,
		Uint16 edgeDistance)
{
    UPoint size = getSize();

    blitFrom(corner_nw.get(), UPoint(edgeDistance,edgeDistance));
    blitFrom(corner_ne.get(), UPoint(size.x - corner_ne->getSize().x - edgeDistance, edgeDistance));
    blitFrom(corner_sw.get(), UPoint(edgeDistance, size.y - corner_sw->getSize().y - edgeDistance));
    blitFrom(corner_se.get(), UPoint(size.x - corner_se->getSize().x - edgeDistance, size.y - corner_se->getSize().y - edgeDistance));

    for(int i = corner_se->getSize().x; i < size.x - corner_se->getSize().y; i++){
        blitFrom(top.get(), UPoint(i, edgeDistance));
        blitFrom(top.get(), UPoint(i, size.y - bottom->getSize().y - edgeDistance));
    }

    for(int i = corner_ne->getSize().y; i < size.y - corner_se->getSize().x; i++){
        blitFrom(left.get(), UPoint(edgeDistance, i));
        blitFrom(right.get(), UPoint(size.x - right->getSize().x - edgeDistance, i));
    }

}

void DrawImage::drawBorders(std::string nw, std::string ne, std::string sw,
			std::string se, Uint16 edgeDistance)
{
    ImagePtr corner_nw, corner_ne, corner_sw, corner_se, top, bottom, left, right;
    corner_nw = DataCache::Instance()->getGCObject(nw)->getImage();
    corner_ne = DataCache::Instance()->getGCObject(ne)->getImage();
    corner_sw = DataCache::Instance()->getGCObject(sw)->getImage();
    corner_se = DataCache::Instance()->getGCObject(se)->getImage();
    top = DataCache::Instance()->getGCObject("UI_TopBorder")->getImage();
    bottom = DataCache::Instance()->getGCObject("UI_BottomBorder")->getImage();
    left = DataCache::Instance()->getGCObject("UI_LeftBorder")->getImage();
    right = DataCache::Instance()->getGCObject("UI_RightBorder")->getImage();
    drawBorders(corner_nw, corner_ne, corner_sw, corner_se, top,
		bottom, left, right, edgeDistance);
}

void DrawImage::drawVBar(ConstUPoint start, int y2)
{
	ImagePtr screen = DataCache::Instance()->getGCObject("Screen")->getImage();

    ImagePtr sideBar(new Image(UPoint(12, y2 - start.y))); 
    ImagePtr tmp(screen->getPictureCrop(Rect(241, 52, 12, 6)));
    sideBar->blitFrom(tmp.get());
    tmp.reset(screen->getPictureCrop(Rect(241, 58, 12, 13)));
    for(int i = 6; i < y2 - 6; i += 13)
        sideBar->blitFrom(tmp.get(), UPoint(0, i));
    tmp.reset(screen->getPictureCrop(Rect(241, 117, 12, 6)));
    //FIXME: the line at end of bar and thingie needs to be adapted..
    sideBar->blitFrom(tmp.get(), UPoint(0,  y2 - start.y - 6));
    blitFrom(sideBar.get(), start);
}

void DrawImage::drawHBarSmall(ConstUPoint start, int x2)
{
	ImagePtr screen = DataCache::Instance()->getGCObject("Screen")->getImage();

    ImagePtr sideBar(new Image(UPoint(x2 - start.x, 6))); 
    ImagePtr tmp(screen->getPictureCrop(Rect(254, 127, 5, 6)));
    sideBar->blitFrom(tmp.get());
    tmp.reset(screen->getPictureCrop(Rect(260, 127, 10, 6)));
    for(int i = 5; i < x2 - 6; i += 10)
        sideBar->blitFrom(tmp.get(), UPoint(i, 0));
    tmp.reset(screen->getPictureCrop(Rect(313, 127, 6, 6)));
    //FIXME: the line at end of bar and thingie needs to be adapted..
    sideBar->blitFrom(tmp.get(), UPoint(x2 - start.x - 6, 0));
    blitFrom(sideBar.get(), start);
}

void DrawImage::drawTiles(ImagePtr tile)
{
	Rect area(0, 0, getSize().x, getSize().y);
	drawTiles(tile, area);
}

void DrawImage::drawTiles(ImagePtr tile, ConstRect area)
{
     ImagePtr tiledArea(new Image(UPoint(area.w, area.h)));
     UPoint size = getSize();
     UPoint bgSize = tile->getSize();
        for(int x = 0; x < size.x; x += bgSize.x - 1)
            for(int y = 0; y < size.y; y += bgSize.y - 1)
                tiledArea->blitFrom(tile.get(), UPoint(x,y));
     blitFrom(tiledArea.get(), UPoint(area.x, area.y));
}
