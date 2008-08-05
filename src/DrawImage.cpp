#include "DataCache.h"
#include "DrawImage.h"

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


void DrawImage::drawBorders(GuiPic_enum nw, GuiPic_enum ne, GuiPic_enum sw,
		GuiPic_enum se, Uint16 edgeDistance)
{
    ImagePtr corner_nw, corner_ne, corner_sw, corner_se, top, bottom, left, right;
    UPoint size = getSize();

    corner_nw = DataCache::Instance()->getGuiPic(nw);
    blitFrom(corner_nw.get(), UPoint(edgeDistance,edgeDistance));
    corner_ne = DataCache::Instance()->getGuiPic(ne);
    blitFrom(corner_ne.get(), UPoint(size.x - corner_ne->getSize().x - edgeDistance, edgeDistance));
    corner_sw = DataCache::Instance()->getGuiPic(sw);
    blitFrom(corner_sw.get(), UPoint(edgeDistance, size.y - corner_sw->getSize().y - edgeDistance));
    corner_se = DataCache::Instance()->getGuiPic(se);
    blitFrom(corner_se.get(), UPoint(size.x - corner_se->getSize().x - edgeDistance, size.y - corner_se->getSize().y - edgeDistance));

    top = DataCache::Instance()->getGuiPic(UI_TopBorder);
    bottom = DataCache::Instance()->getGuiPic(UI_BottomBorder);

    for(int i = corner_se->getSize().x; i < size.x - corner_se->getSize().y; i++){
        blitFrom(top.get(), UPoint(i, edgeDistance));
        blitFrom(top.get(), UPoint(i, size.y - bottom->getSize().y - edgeDistance));
    }

    left = DataCache::Instance()->getGuiPic(UI_LeftBorder);
    right = DataCache::Instance()->getGuiPic(UI_RightBorder);
    for(int i = corner_ne->getSize().y; i < size.y - corner_se->getSize().x; i++){
        blitFrom(left.get(), UPoint(edgeDistance, i));
        blitFrom(right.get(), UPoint(size.x - right->getSize().x - edgeDistance, i));
    }

}

void DrawImage::drawTiles(Image *tile)
{
     UPoint size = getSize();
     UPoint bgSize = tile->getSize();
        for(int x = 0; x < size.x; x += bgSize.x - 1)
            for(int y = 0; y < size.y; y += bgSize.y - 1)
                blitFrom(tile, UPoint(x,y));
}
