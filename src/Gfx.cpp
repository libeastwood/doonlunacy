#include "Gfx.h"

#include "Application.h"    // for Screen
#include "GCObject.h"
#include "houses.h"        // for house colors
#include "Log.h"
#include "Settings.h"

#include <assert.h>

//------------------------------------------------------------------------------
// Image class
//------------------------------------------------------------------------------

Image::Image(SDL_Surface *surface) : surface(surface)
{
    assert(surface != NULL);
	if (surface == NULL)
	{
	    // TODO: throw better exception !!
        throw "Image::Image(SDL_Surface *) - got NULL !";
	}
    
	m_tmpPal = NULL;
}

Image::Image(ConstUPoint size)
{
    assert(size.x != 0);
    assert(size.y != 0);
    
    surface = SDL_CreateRGBSurface(
                                   SDL_HWSURFACE, // TODO: which one to use, HW or SW ? (otpetrik)
                                   size.x,  // width
                                   size.y,  // height
                                   8,       // bits per pixel
                                   0,0,0,0);// r,g,b,a masks

	assert(surface != NULL);		
	if (surface == NULL)
	{
	    // TODO: throw better exception !!
        throw "Image::Image(ConstUPoint) - unable to create SDL_Surface !";
	}
	// copy palette from the screen (otherwise you'll get only black image)
    SDL_SetColors(surface, Application::Instance()->Screen()->getSurface()->format->palette->colors, 0, 256);

	m_tmpPal = NULL;

}

Image::~Image()
{
    SDL_FreeSurface(surface);
	if(m_tmpPal != NULL)
		delete m_tmpPal;
}

Image::operator SDL_Surface*()
{
    return surface;
}
SDL_Surface *Image::getSurface()
{
    return surface;
}

Image *Image::getPictureCrop(ConstRect dstRect)
{
	if(((int) (dstRect.x + dstRect.w) > surface->w) || ((int) (dstRect.y + dstRect.h) > surface->h)) {
		LOG_ERROR("GFX", "getPictureCrop: Cannot create new x:%d y:%d %dx%d Picture!",
				dstRect.x, dstRect.y, dstRect.w, dstRect.y);
		exit(EXIT_FAILURE);	
	}
	
	SDL_Surface *returnPic;
	
	// create new picture surface
	if((returnPic = SDL_CreateRGBSurface(SDL_HWSURFACE,dstRect.w, dstRect.h,8,0,0,0,0))== NULL) {
		LOG_ERROR("GFX", "getPictureCrop: Cannot create new x:%d y:%d %dx%d Picture!",
				dstRect.x, dstRect.y, dstRect.w, dstRect.y);

		exit(EXIT_FAILURE);	
	}
			
	SDL_SetColors(returnPic, surface->format->palette->colors, 0, surface->format->palette->ncolors);

	Rect r(dstRect);
	SDL_BlitSurface(surface,&r,returnPic,NULL); 

	return new Image(returnPic);
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


void Image::fillRectVGradient(Uint32 color1, Uint32 color2, ConstRect dstRect)
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
void Image::fillRectHGradient(Uint32 color1, Uint32 color2, ConstRect dstRect)
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

void Image::drawBorders(std::string nw, std::string ne, std::string sw,
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

void Image::drawVBar(ConstUPoint start, int y2)
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

void Image::drawHBarSmall(ConstUPoint start, int x2)
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

void Image::drawTiles(ImagePtr tile)
{
	Rect area(0, 0, getSize().x, getSize().y);
	drawTiles(tile, area);
}

void Image::drawTiles(ImagePtr tile, ConstRect area)
{
     ImagePtr tiledArea(new Image(UPoint(area.w, area.h)));
     UPoint size = getSize();
     UPoint bgSize = tile->getSize();
        for(int x = 0; x < size.x; x += bgSize.x - 1)
            for(int y = 0; y < size.y; y += bgSize.y - 1)
                tiledArea->blitFrom(tile.get(), UPoint(x,y));
     blitFrom(tiledArea.get(), UPoint(area.x, area.y));
}

bool Image::fadeIn(const int fadeAmt)
{
	SDL_Color *src = surface->format->palette->colors;
	SDL_Color *dest = m_tmpPal;
	if(m_tmpPal == NULL)
	{
		m_tmpPal = new SDL_Color[256];
		memcpy((unsigned char*)m_tmpPal,
			surface->format->palette->colors,
            sizeof(SDL_Color) * 256);
		dest = m_tmpPal;

		for (int i=0; i!=256; i++, src++)
		{
			src->r = 0;
			src->g = 0;
			src->b = 0;
		}
		SDL_SetColors(surface, surface->format->palette->colors, 0, 256);
		return true;
	}
	bool fade = false;

    for (int i=0; i!=256; i++, src++, dest++)
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
		SDL_SetColors(surface, surface->format->palette->colors, 0, 256);
	return fade;
}

bool Image::fadeOut(const int fadeAmt)
{
	bool fade = false;
	SDL_Color *src = surface->format->palette->colors;

    for (int i=0; i!=256; i++, src++)
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
		SDL_SetColors(surface, surface->format->palette->colors, 0, 256);
	return fade;
}

//------------------------------------------------------------------------------
// Single pixel operations
//------------------------------------------------------------------------------

void putPixel(SDL_Surface *surface, int x, int y, Uint32 color)
{
    assert(surface != NULL);
    SDL_Surface *screen = Application::Instance()->Screen()->getSurface();
	if (x >= 0 && x < screen->w && y >=0 && y < screen->h)
	{
		int bpp = surface->format->BytesPerPixel;
		// p is the address of the pixel to set
		Uint8 *p = (Uint8 *)surface->pixels + y*surface->pitch + x*bpp;
		switch(bpp) {
		case 1:
			*p = color;
			break;

		case 2:
			*(Uint16 *)p = color;
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
			*(Uint32 *)p = color;
			break;
		}
	}
}

Uint32 getPixel(SDL_Surface *surface, int x, int y)
{
    assert(surface != NULL);
    int bpp = surface->format->BytesPerPixel;
    // p is the address of the pixel to retrieve
    Uint8 *p = (Uint8 *)surface->pixels + y*surface->pitch + x*bpp;
    switch(bpp) {
    case 1:
        return *p;
    case 2:
        return *(Uint16 *)p;
    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
    case 4:
        return *(Uint32 *)p;
    default:
        return 0;       // shouldn't happen, but avoids warnings

    }
}

//------------------------------------------------------------------------------
// Drawing operations
//------------------------------------------------------------------------------

void drawHLine(SDL_Surface *surface, int x, int y, int x2, Uint32 color, bool lock)
{
    assert(surface != NULL);

    // MUSTLOCK == 0 means no need for locking, LockSurface == 0 means successful lock
    if (lock == false || (SDL_MUSTLOCK(surface) == 0) ||  (SDL_LockSurface(surface) == 0))
    {
        if (x > x2)
        {
            int t = x;
            x = x2;
            x2 = t;
        }
        for (int i = x; i <= x2; i++)
            putpixel(surface, i, y, color);
        if (lock == false || SDL_MUSTLOCK(surface))
            SDL_UnlockSurface(surface);
    }
}

void drawVLine(SDL_Surface *surface, int x, int y, int y2, Uint32 color, bool lock)
{
    assert(surface != NULL);

    // MUSTLOCK == 0 means no need for locking, LockSurface == 0 means successful lock
    if (lock == false || (SDL_MUSTLOCK(surface) == 0) ||  (SDL_LockSurface(surface) == 0))
    {
        if (y > y2)
        {
            int t = y;
            y = y2;
            y2 = t;
        }
        for (int i = y; i <= y2; i++)
            putpixel(surface, x, i, color);
        if (lock == false || SDL_MUSTLOCK(surface))
            SDL_UnlockSurface(surface);
    }
}

void drawRect(SDL_Surface *surface, const SDL_Rect &rect, Uint32 color, bool lock)
{
    assert(surface != NULL);

    // MUSTLOCK == 0 means no need for locking, LockSurface == 0 means successful lock
    if (lock == false || (SDL_MUSTLOCK(surface) == 0) ||  (SDL_LockSurface(surface) == 0))
    {
        drawHLine(surface, rect.x, rect.y, rect.x + rect.w-1, color, false);
        drawHLine(surface, rect.x, rect.y + rect.h-1, rect.x + rect.w-1, color, false);
        drawVLine(surface, rect.x, rect.y, rect.y + rect.h, color, false);
        drawVLine(surface, rect.x + rect.w-1, rect.y, rect.y + rect.h-1, color, false);
        if (lock == false || SDL_MUSTLOCK(surface))
            SDL_UnlockSurface(surface);
    }
}

//------------------------------------------------------------------------------
// Surface operations
//------------------------------------------------------------------------------

SDL_Surface* copySurface(SDL_Surface* surface)
{
    assert(surface != NULL);
    
	//return SDL_DisplayFormat(surface);
	return SDL_ConvertSurface(surface, surface->format, surface->flags);
}

SDL_Surface* resizeSurface(SDL_Surface *surface, Uint16 w, Uint16 h)
{
    assert(surface != NULL);
    assert(w != 0);
    assert(h != 0);
    
    SDL_Surface *resized = 
    SDL_CreateRGBSurface(
			SDL_HWSURFACE,
			w,              // width
			h,              // height 
			8,              // bits per pixel
			0,0,0,0);       // r,g,b,a masks	
			
	// TODO: throw an exception ?
	if (resized == NULL)
	{
	   return NULL;
	}
	
	// copy palette (otherwise you'll get only black image)
    SDL_SetColors(resized, surface->format->palette->colors, 0, 256);
      
    // copy colorkey (not sure what happens ;-) )
    SDL_SetColorKey(resized, (surface->flags & SDL_SRCCOLORKEY) | (surface->flags & SDL_RLEACCEL), surface->format->colorkey);

    if (SDL_MUSTLOCK(resized))
        SDL_LockSurface(resized);
    if (SDL_MUSTLOCK(surface))
        SDL_LockSurface(surface);       

    for(int y = 0; y < resized->h; y++)
    {
        for(int x = 0; x < resized->w; x++)
        {
	       putPixel(resized, x, y, getPixel(surface, surface->w*x/resized->w, surface->h*y/resized->h));
	    }
	}

    if (SDL_MUSTLOCK(resized))
        SDL_UnlockSurface(resized);
    if (SDL_MUSTLOCK(surface))
        SDL_UnlockSurface(surface);
    return resized;
}

SDL_Surface* resizeSurface(SDL_Surface *surface, float ratio)
{
    assert(surface != NULL);
    return resizeSurface(surface, (Uint16)(surface->w*ratio), (Uint16)(surface->h*ratio));
}

//------------------------------------------------------------------------------
// Color mapping
//------------------------------------------------------------------------------

void remapSurface(SDL_Surface *surface, int colorSrc, int colorDst, int colorNum)
{
    assert(surface != NULL);
    assert(surface->format->BitsPerPixel == 8);

    // MUSTLOCK == 0 means no need for locking, LockSurface == 0 means successful lock
    if ((SDL_MUSTLOCK(surface) == 0) ||  (SDL_LockSurface(surface) == 0))
    {
        Uint8 *pixel;
        int x, y;
        for (y = 0; y < surface->h; y++)
            for (x = 0; x < surface->w; x++)
                {
				pixel = &(((Uint8*)surface->pixels)[y*surface->pitch + x]);
				if ((*pixel >= colorSrc) && (*pixel < colorSrc + colorNum))

					*pixel = *pixel - colorSrc + colorDst;
                }
        if (SDL_MUSTLOCK(surface))
            SDL_UnlockSurface(surface);
    }    
}

void remapSurfaceByHouse(SDL_Surface *surface, int house)
{
    remapSurface(surface, COLOUR_HARKONNEN, houseColour[house]);
}
