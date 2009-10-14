#include <assert.h>
#include <cmath>
#include <vector>

#include "Gfx.h"

#include "Application.h"    // for Screen
#include "GameData.h"
#include "houses.h"        // for house colors
#include "Log.h"
#include "ResMan.h"
#include "Settings.h"

//------------------------------------------------------------------------------
// Image class
//------------------------------------------------------------------------------

Image::Image(const eastwood::SDL::Surface &surface) :
    eastwood::SDL::Surface(surface), m_origPal(0), m_tmpPal(0)
{
    //FIXME: Why is this required??
    pixels = *_pixels.get();
}

Image::Image(SDL_Surface *surface) : eastwood::SDL::Surface(*surface), m_tmpPal(0) {
}

Image::Image(ConstUPoint size) :
    eastwood::SDL::Surface(size.x, size.y, 8, *Application::Instance()->Screen()->getPalette()),
    m_origPal(0), m_tmpPal(0)
{
    assert(size.x != 0);
    assert(size.y != 0);
}

Image& Image::operator=(const SDL_Surface *surface) 
{
    *(eastwood::SDL::Surface*)this = surface;

    return *this;
}

Image::~Image()
{
}

ImagePtr Image::getPictureCrop(ConstRect dstRect)
{
    if(((int) (dstRect.x + dstRect.w) > w) || ((int) (dstRect.y + dstRect.h) > h)) {
	LOG(LV_ERROR, "GFX", "getPictureCrop: Cannot create new x:%d y:%d %dx%d Picture!",
		dstRect.x, dstRect.y, dstRect.w, dstRect.h);
	exit(EXIT_FAILURE);	
    }

    Image *returnPic = new Image(UPoint(dstRect.w, dstRect.h));

    returnPic->setPalette(_palette);
    returnPic->blitFrom(this, dstRect, UPoint(0,0));

    return ImagePtr(returnPic);
}

void Image::blitToScreen(ConstRect srcRect, ConstUPoint dstPoint) const
{
    blitTo(Application::Instance()->Screen(), srcRect, dstPoint);
}
void Image::blitToScreen(ConstUPoint dstPoint) const
{
    blitTo(Application::Instance()->Screen(), dstPoint);
}
void Image::blitToScreen() const
{
    blitTo(Application::Instance()->Screen());
}
void Image::blitToScreenCentered() const
{
    blitToCentered(Application::Instance()->Screen());
}


void Image::fillRectVGradient(uint32_t color1, uint32_t color2, ConstRect dstRect)
{
    int numColors = color2 - color1 + 1;
    float stripeWidth = dstRect.w/(float)numColors;
    float begin;
    float end;
    Rect r(dstRect);
    for (int i = 0; i < numColors; i++)
    {
	// calculate begin & end of i-th strip
	begin = i*stripeWidth;
	end = (i+1)*stripeWidth;

	// round the border pixel to major one
	begin = (int)begin + (((begin - (int)begin) < 0.5f) ? 0 : 1);
	end = (int)end + (((end - (int)end) < 0.5f) ? 0 : 1);

	// calcuate the rect
	r.x = (int)begin;
	r.w = (int)end - r.x;

	// fillit
	fillRect(color1 + i, r);
    }
}
void Image::fillRectHGradient(uint32_t color1, uint32_t color2, ConstRect dstRect)
{
    int numColors = color2 - color1 + 1;
    float stripeHeight = dstRect.h/(float)numColors;
    float begin;
    float end;
    Rect r(dstRect);
    for (int i = 0; i < numColors; i++)
    {
	// calculate begin & end of i-th strip
	begin = i*stripeHeight;
	end = (i+1)*stripeHeight;

	// round the border pixel to major one
	begin = (int)begin + (((begin - (int)begin) < 0.5f) ? 0 : 1);
	end = (int)end + (((end - (int)end) < 0.5f) ? 0 : 1);

	// calcuate the rect
	r.y = (int)begin;
	r.h = (int)end - r.y;

	// fillit
	fillRect(color1 + i, r);
    }
}

void Image::drawBorders(ImagePtr corner_nw, ImagePtr corner_ne,
	ImagePtr corner_sw, ImagePtr corner_se, ImagePtr top,
	ImagePtr bottom, ImagePtr left, ImagePtr right,
	uint16_t edgeDistance)
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

void Image::drawBorders(std::string nw, std::string ne, std::string sw,
	std::string se, uint16_t edgeDistance)
{
    ImagePtr corner_nw, corner_ne, corner_sw, corner_se, top, bottom, left, right;
    corner_nw = DataCache::Instance()->getGameData(nw)->getImage();
    corner_ne = DataCache::Instance()->getGameData(ne)->getImage();
    corner_sw = DataCache::Instance()->getGameData(sw)->getImage();
    corner_se = DataCache::Instance()->getGameData(se)->getImage();
    top = DataCache::Instance()->getGameData("UI_TopBorder")->getImage();
    bottom = DataCache::Instance()->getGameData("UI_BottomBorder")->getImage();
    left = DataCache::Instance()->getGameData("UI_LeftBorder")->getImage();
    right = DataCache::Instance()->getGameData("UI_RightBorder")->getImage();
    drawBorders(corner_nw, corner_ne, corner_sw, corner_se, top,
	    bottom, left, right, edgeDistance);
}

void Image::drawVBar(ConstUPoint start, int y2)
{
    ImagePtr screen = DataCache::Instance()->getGameData("Screen")->getImage();

    ImagePtr sideBar(new Image(UPoint(12, y2 - start.y))); 
    ImagePtr tmp(screen->getPictureCrop(Rect(241, 52, 12, 6)));
    sideBar->blitFrom(tmp.get());
    tmp = screen->getPictureCrop(Rect(241, 58, 12, 13));
    for(UPoint iter(0, 6); iter.y < y2 - 6; iter.y += 13)
	sideBar->blitFrom(tmp.get(), iter);
    tmp = screen->getPictureCrop(Rect(241, 117, 12, 6));
    //FIXME: the line at end of bar and thingie needs to be adapted..
    sideBar->blitFrom(tmp.get(), UPoint(0,  y2 - start.y - 6));
    blitFrom(sideBar.get(), start);
}

void Image::drawHBarSmall(ConstUPoint start, int x2)
{
    ImagePtr screen = DataCache::Instance()->getGameData("Screen")->getImage();

    ImagePtr sideBar(new Image(UPoint(x2 - start.x, 6))); 
    ImagePtr tmp(screen->getPictureCrop(Rect(254, 127, 5, 6)));
    sideBar->blitFrom(tmp.get());
    tmp = screen->getPictureCrop(Rect(260, 127, 10, 6));
    for(UPoint iter(5, 0); iter.x < x2 - 6; iter.x += 10)
	sideBar->blitFrom(tmp.get(), iter);
    tmp = screen->getPictureCrop(Rect(313, 127, 6, 6));
    //FIXME: the line at end of bar and thingie needs to be adapted..
    sideBar->blitFrom(tmp.get(), UPoint(x2 - start.x - 6, 0));
    blitFrom(sideBar.get(), start);
}

void Image::drawTiles(ImagePtr tile, Rect area)
{
    if(area == Rect())
	area.setSize(getSize());

    ImagePtr tiledArea(new Image(UPoint(area.w, area.h)));
    UPoint size = getSize();
    UPoint tileSize = tile->getSize();
    UPoint iter;
    ImagePtr tileHFlipped = tile->getCopy();
    tileHFlipped->flipH();
    ImagePtr tileVFlipped = tile->getCopy();
    tileVFlipped->flipV();
    ImagePtr tileHVFlipped = tileVFlipped->getCopy();
    tileHVFlipped->flipH();
    for(iter.y = 0; iter.y < size.y; iter.y += tileSize.y)
	for(iter.x = 0; iter.x < size.x; iter.x += tileSize.x)
	{
			if((iter.x % (tileSize.x*2) == 0) && (iter.y % (tileSize.y*2) == 0)) {
			    tiledArea->blitFrom(tile.get(), iter);
			} else if((iter.x % (tileSize.x*2) != 0) && (iter.y % (tileSize.y*2) == 0)) {
			    tiledArea->blitFrom(tileHFlipped.get(), iter);
			} else if((iter.x % (tileSize.x*2) == 0) && (iter.y % (tileSize.y*2) != 0)) {
			    tiledArea->blitFrom(tileVFlipped.get(), iter);
			} else /*if((dest.x % (tileSize.x*2) != 0) && (dest.y % (tileSize.y*2) != 0))*/ {
			    tiledArea->blitFrom(tileHVFlipped.get(), iter);
			}
	}

    blitFrom(tiledArea.get(), UPoint(area.x, area.y));
}

bool Image::fadeIn(const int fadeAmt)
{
    eastwood::Color *src = &_palette[0];
    uint16_t ncolors = _palette.size();
    
    eastwood::Color *dest = &m_tmpPal[0];
    if(!m_tmpPal.size())
    {
	m_tmpPal = eastwood::Palette(ncolors);
	dest = &m_tmpPal[0];

	for (int i=0; i!=ncolors; i++, src++)
	{
	    src->r = 0;
	    src->g = 0;
	    src->b = 0;
	}
	setPalette(_palette);
	return true;
    }
    bool fade = false;

    for (int i=0; i!=ncolors; i++, src++, dest++)
    {
	if (src->r < dest->r || src->g < dest->g || src->b < dest->b)
	{
	    if (!fade) fade = true;
	    if (src->r < dest->r)
		// Make sure that we don't go beyond destination value
		(src->r < dest->r - fadeAmt) ? src->r += fadeAmt : 	src->r = dest->r;
	    if (src->g < dest->g)
		(src->g < dest->g - fadeAmt) ? src->g += fadeAmt : 	src->g = dest->g;
	    if (src->b < dest->b)
		(src->b < dest->b - fadeAmt) ? src->b += fadeAmt : 	src->b = dest->b;
	}
    }
    if(fade)
	setPalette(_palette);

    return fade;
}

bool Image::fadeOut(const int fadeAmt)
{
    bool fade = false;
    eastwood::Color *src = &_palette[0];

    for (uint16_t i=0; i!=_palette.size(); i++, src++)
    {
	if (src->r > 0 || src->g > 0 || src->b > 0)
	{
	    if (!fade) fade = true;
	    if (src->r > 0)
		// Make sure that we don't go beyond 0
		(src->r > 0 + fadeAmt) ? src->r -= fadeAmt : src->r = 0;
	    if (src->g > 0)
		(src->g > 0 + fadeAmt) ? src->g -= fadeAmt : src->g = 0;
	    if (src->b > 0)
		(src->b > 0 + fadeAmt) ? src->b -= fadeAmt : src->b = 0;
	}
    }
    if(fade)
	setPalette(_palette);

    return fade;
}

static inline uint16_t colDiff(eastwood::Color srcCol, eastwood::Color dstCol) {
    return abs(srcCol.r - dstCol.r) + abs(srcCol.g - dstCol.g) + abs(srcCol.b - dstCol.b);
}

bool Image::morph(ImagePtr morphImage, const int morphAmt) {
    bool morph = false;
    UPoint point;
    std::vector<uint16_t> newCol;
    newCol.assign(_palette.size(), -1);

    for(point.x = 0;  point.x != w; point.x++)
	for(point.y = 0; point.y != h; point.y++)
	{
	    uint8_t curPix = getPixel(point);
	    uint8_t dstPix = morphImage->getPixel(point);
	    if(curPix != dstPix){
		uint8_t newPix;
		if(newCol[curPix] != (uint16_t)-1)
		    newPix = (uint8_t)newCol[curPix];
		else
		{
		    eastwood::Color srcCol = _palette[curPix];
		    eastwood::Color dstCol = _palette[dstPix];
		    uint16_t minDiff = -1;
		    std::vector<uint8_t> newCols;
		    for(uint16_t i = 0; i != _palette.size(); i++)
		    {
			if(i == curPix) continue;
			eastwood::Color col = _palette[i];
			uint16_t diff = colDiff(srcCol, col);
			uint16_t dstDiff = colDiff(srcCol, dstCol);
			if(diff == dstDiff)
			{
			    newCols.insert(newCols.begin(), dstPix);
			    break;
			}
			if(diff < minDiff && colDiff(col, dstCol) < dstDiff)
			    minDiff = diff, newCols.insert(newCols.begin(), i);
		    }
		    int i = 0;
		    while(!newCols.empty() && i++ < morphAmt)
		    {
			newPix = newCols.back();
			newCols.pop_back();
		    }
		    newCol[curPix] = newPix;
		}
		if(curPix != dstPix)
		    morph = true;
		putPixel(point, newPix);
	    }
	}
    return morph;
}

void Image::flipH() {
    if (!mustLock() || (lockSurface() == 0)) {
	for(int i = 0; i < w; i++) {
		for(int j = 0; j < h/2; j++) {
		    uint8_t *top = (uint8_t *)pixels + j*pitch + i*format->BytesPerPixel,
			  *bottom = (uint8_t *)pixels + (h-j-1)*pitch + i*format->BytesPerPixel;
		    swap(*top, *bottom);
		}
	}
	if (mustLock())
	    unlockSurface();
    }	
}


void Image::flipV() {
    if (!mustLock() || (lockSurface() == 0)) {
	for(int j = 0; j < h; j++) {
		for(int i = 0; i < w/2; i++) {
		    uint8_t *left = (uint8_t *)pixels + j*pitch + i*format->BytesPerPixel,
			  *right = (uint8_t *)pixels + j*pitch + (w-i-1)*format->BytesPerPixel;
		    swap(*left, *right);
		}
	}
	if (mustLock())
	    unlockSurface();
    }	
}

//------------------------------------------------------------------------------
// Single pixel operations
//------------------------------------------------------------------------------

void Image::putPixel(ConstUPoint point, uint32_t color) {
    UPoint screenSize = Application::Instance()->Screen()->getSize();
    if (point.x >= 0 && point.x < screenSize.x && point.y >=0 && point.y < screenSize.y)
    {
	int bpp = format->BytesPerPixel;
	// p is the address of the pixel to set
	uint8_t *p = (uint8_t *)pixels + point.y*pitch + point.x*bpp;
	switch(bpp) {
	    case 1:
		*p = color;
		break;

	    case 2:
		*(uint16_t *)p = color;
		break;

	    case 3:
		if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
		    p[0] = (color>> 16) & 0xff;
		    p[1] = (color>> 8) & 0xff;
		    p[2] = color& 0xff;
		} else {
		    p[0] = color& 0xff;
		    p[1] = (color>> 8) & 0xff;
		    p[2] = (color>> 16) & 0xff;
		}
		break;

	    case 4:
		*(uint32_t *)p = color;
		break;
	}
    }
}

uint32_t Image::getPixel(ConstUPoint point) const
{
    int bpp = format->BytesPerPixel;
    // p is the address of the pixel to retrieve
    uint8_t *p = (uint8_t *)pixels + point.y*pitch + point.x*bpp;
    switch(bpp) {
	case 1:
	    return *p;
	case 2:
	    return *(uint16_t *)p;
	case 3:
	    if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
		return p[0] << 16 | p[1] << 8 | p[2];
	    else
		return p[0] | p[1] << 8 | p[2] << 16;
	case 4:
	    return *(uint32_t *)p;
	default:
	    return 0;       // shouldn't happen, but avoids warnings

    }
}

//------------------------------------------------------------------------------
// Drawing operations
//------------------------------------------------------------------------------

void Image::drawHLine(UPoint start, int x2, uint32_t color, bool lock) {
    // MUSTLOCK == 0 means no need for locking, LockSurface == 0 means successful lock
    //
    if (lock == false || !mustLock() || (lockSurface() == 0))
    {
	if (start.x > x2)
	{
	    int t = start.x;
	    start.x = x2;
	    x2 = t;
	}
	for (; start.x <= x2; start.x++)
	    putPixel(start, color);
	if (lock == false || mustLock())
	    unlockSurface();
    }
}

void Image::drawVLine(UPoint start, int y2, uint32_t color, bool lock) {
    // MUSTLOCK == 0 means no need for locking, LockSurface == 0 means successful lock
    if (lock == false || !mustLock() ||  (lockSurface() == 0))
    {
	if (start.y > y2)
	{
	    int t = start.y;
	    start.y = y2;
	    y2 = t;
	}
	for (; start.y <= y2; start.y++)
	    putPixel(start, color);
	if (lock == false || mustLock())
	    unlockSurface();
    }
}

void Image::drawRect(ConstRect rect, uint32_t color, bool lock) {
    // MUSTLOCK == 0 means no need for locking, LockSurface == 0 means successful lock
    if (lock == false || !mustLock() ||  (lockSurface() == 0))
    {
	drawHLine(UPoint(rect.x, rect.y), rect.x + rect.w-1, color, false);
	drawHLine(UPoint(rect.x, rect.y + rect.h-1), rect.x + rect.w-1, color, false);
	drawVLine(UPoint(rect.x, rect.y), rect.y + rect.h-1, color, false);
	drawVLine(UPoint(rect.x + rect.w-1, rect.y), rect.y + rect.h-1, color, false);
	if (lock == false || mustLock())
	    unlockSurface();
    }
}

//------------------------------------------------------------------------------
// Surface operations
//------------------------------------------------------------------------------

ImagePtr Image::getResized(ConstUPoint size)
{
    assert(size.x != 0);
    assert(size.y != 0);

    Image *resized = new Image(size);

    // copy palette (otherwise you'll get only black image)
    resized->setPalette(_palette);

    // copy colorkey (not sure what happens ;-) )
    resized->setColorKey(format->colorkey, (flags & SDL_SRCCOLORKEY) | (flags & SDL_RLEACCEL));

    if (resized->mustLock())
	resized->lockSurface();
    if (mustLock())
	lockSurface();

    for(int y = 0; y < resized->h; y++)
	for(int x = 0; x < resized->w; x++)
	    resized->putPixel(UPoint(x, y), getPixel(UPoint(w*x/resized->w, h*y/resized->h)));

    if (resized->mustLock())
	resized->unlockSurface();
    if (mustLock())
	unlockSurface();

    return ImagePtr(resized);
}

//------------------------------------------------------------------------------
// Color mapping
//------------------------------------------------------------------------------

void Image::recolor(int colorSrc, int colorDst, int colorNum) {
    // MUSTLOCK == 0 means no need for locking, LockSurface == 0 means successful lock
    if (!mustLock() || (lockSurface() == 0))
    {
	uint8_t *pixel;
	int x, y;
	for (y = 0; y < h; y++)
	    for (x = 0; x < w; x++)
	    {
		pixel = &(((uint8_t*)pixels)[y*pitch + x]);
		if ((*pixel >= colorSrc) && (*pixel < colorSrc + colorNum))

		    *pixel = *pixel - colorSrc + colorDst;
	    }
	if (mustLock())
	    unlockSurface();
    }    
}

void Image::recolorByHouse(int house)
{
    recolor(DEFAULT_SURFACE_REMAP_BEGIN, houseColour[house]);
}

void Image::greyOut(Rect area) {
    size_t len;
    uint8_t *index2greyindex;
    eastwood::IStream *data = ResMan::Instance()->getFile("DUNE:GRAYRMAP.TBL");
    len = data->sizeg();
    index2greyindex = (uint8_t*)malloc(len);
    data->read((char*)index2greyindex, len);

    if(area == Rect())
	area.setSize(getSize());

    if (!mustLock() || (lockSurface() == 0))
    {
	for(int i = area.x; i < area.w; i++)
	    for(int j = area.y; j < area.h; j++) {
		unsigned char inputIndex = *( ((unsigned char*) (pixels)) + j*pitch + i);
		unsigned char outputIndex = index2greyindex[inputIndex];
		*( ((unsigned char*) (pixels)) + j*pitch + i) = outputIndex;
	    }
	if (mustLock())
	    unlockSurface();
    }
    
    free(index2greyindex);
}
