#include "Gfx.h"

#include "Application.h"    // for Screen
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
	};
}
Image::Image(ConstUPoint size)
{
    assert(size.x != 0);
    assert(size.y != 0);
    
    surface = SDL_CreateRGBSurface(
                                   SDL_SWSURFACE, // TODO: which one to use, HW or SW ? (otpetrik)
                                   size.x,  // width
                                   size.y,  // height
                                   8,       // bits per pixel
                                   0,0,0,0);// r,g,b,a masks

	assert(surface != NULL);		
	if (surface == NULL)
	{
	    // TODO: throw better exception !!
        throw "Image::Image(ConstUPoint) - unable to create SDL_Surface !";
	};
	
	// copy palette from the screen (otherwise you'll get only black image)
    SDL_SetColors(surface, Application::Instance()->Screen()->getSurface()->format->palette->colors, 0, 256);
}
Image::~Image()
{
    SDL_FreeSurface(surface);
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

UPoint sizeRelativeUPoint(int x, int y){
	float wratio = (float)Settings::Instance()->GetWidth() / 320;
	float hratio = (float)Settings::Instance()->GetHeight() / 200;
	return UPoint(wratio * x, hratio * y);
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
			SDL_SWSURFACE,
			w,              // width
			h,              // height 
			8,              // bits per pixel
			0,0,0,0);       // r,g,b,a masks	
			
	// TODO: throw an exception ?
	if (resized == NULL)
	{
	   return NULL;
	};
	
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
	    };
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
