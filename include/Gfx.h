/*!
  \file Gfx.h
  \brief Various gfx routines.   

  @note Use Image class and coresponding methods. SDL_Surface-based functions will
  be removed by moving their code directly into the Image class.

*/    

#ifndef DUNE_GFX_H
#define DUNE_GFX_H

#include <SDL_video.h>
#include <string>
#include <eastwood/Font.h>

#include "Colours.h"
#include "Point.h"
#include "Rect.h"

//------------------------------------------------------------------------------
// Single pixel operations
//------------------------------------------------------------------------------
//! @name Single pixel operations
//@{

//! Set pixel on given surface
/*!
  @param surface surface to draw to
  @param x x-coord
  @param y y-coord
  @param color color

  @warning The surface has to be locked !
  */
void putPixel(SDL_Surface *surface, int x, int y, Uint32 color);

//! Get pixel from given surface
/*!
  @param surface surface to read from
  @param x x-coord
  @param y y-coord
  @return read color

  @warning The surface has to be locked !
  */
Uint32 getPixel(SDL_Surface *surface, int x, int y);

//@}

//------------------------------------------------------------------------------
// Drawing operations
//------------------------------------------------------------------------------
//! @name Drawing operations
//@{

//! Draw horizontal line
/*!
  @param surface surface to draw to
  @param x x-coord of start
  @param y y-coord of start
  @param x2 x-coord of finish
  @param color color to draw with
  @param lock whether to lock the surface (defaults to yes)
  */
void drawHLine(SDL_Surface *surface, int x, int y, int x2, Uint32 color, bool lock = true);

//! Draw vertical line
/*!
  @param surface surface to draw to
  @param x x-coord of start
  @param y y-coord of start
  @param y2 y-coord of finish
  @param color color to draw with
  @param lock whether to lock the surface (defaults to yes)
  */
void drawVLine(SDL_Surface *surface, int x, int y, int y2, Uint32 color, bool lock = true);

//! Draw rectangle
/*!
  @param surface surface to draw to
  @param rect rectangle to draw
  @param color color to draw with
  @param lock whether to lock the surface (defaults to yes)
  */
void drawRect(SDL_Surface *surface, const SDL_Rect &rect, Uint32 color, bool lock = true);

//@}

//------------------------------------------------------------------------------
// Surface operations
//------------------------------------------------------------------------------
//! @name Surface operations
//@{

//! Make copy of surface 
/*!
  @param surface original surface
  */
SDL_Surface* copySurface(SDL_Surface* surface);

//! Make resized copy of surface 
/*!
  @param surface original surface
  @param w new width
  @param h new height    
  */
SDL_Surface* resizeSurface(SDL_Surface *surface, Uint16 w, Uint16 h);


//! Make resized copy of surface 
/*!
  @param surface original surface
  @param ratio ratio of new surface to original (2 = double size)
  */
SDL_Surface* resizeSurface(SDL_Surface *surface, float ratio);

//@}

//------------------------------------------------------------------------------
// Color remapping
//------------------------------------------------------------------------------
//! @name Color mapping
//@{

#define DEFAULT_SURFACE_REMAP_LENGTH 7
#define DEFAULT_SURFACE_REMAP_BEGIN COLOUR_HARKONNEN

//! Remap colors of surface
/*!
  @param surface source (and destination) surface
  @param colorSrc first color to be replaced
  @param colorDst first color to be used as replacement
  @param colorNum number of colors to replace (last is colorSrc+colorNum-1)
  */
void remapSurface(SDL_Surface *surface, int colorSrc, int colorDst, int colorNum = DEFAULT_SURFACE_REMAP_LENGTH);

//! Remap colors of surface
/*!
  @param surface source (and destination) surface
  @param house house to remap colors to

  @note Starting color and color number defaults to hardcoded numbers.
  */
void remapSurfaceByHouse(SDL_Surface *surface, int house);

//! Remap colors of surface
/*!
  @param surface source (and destination) surface
  @param color first color to be used as replacement

  @note Starting color and color number defaults to hardcoded numbers.
  */
inline void remapSurfaceByColor(SDL_Surface *surface, int color)
{
    remapSurface(surface, DEFAULT_SURFACE_REMAP_BEGIN, color);
}

//@}

//------------------------------------------------------------------------------
// Image class
//------------------------------------------------------------------------------

class CutSceneState;

class Image : private SDL_Surface
{
    public:

	//! @name Constructors & Destructor
	//@{

	//! Constructor
	/*!
	  @warning Do not pass NULL parameter !
	  */
	Image(SDL_Surface *surface);

	//! Constructor
	/*!
	  @param size size of the image !
	  */
	Image(ConstUPoint size);

	//! Destructor
	~Image();

	//@}

	friend class CutSceneState;

	//! @name Access methods
	//@{

	//! Returns size of the surface
	UPoint getSize() const {
	    return UPoint(w, h);
	}

	//! Returns colormap of the surface
	SDL_Color *getColors() const {
	    return format->palette->colors;
	}

	//@}

	//! @name Single pixel operations
	//@{

	//! Set pixel on given surface
	/*!
	  @param point coordinates of the pixel
	  @param color color

	  @warning The surface has to be locked !
	  */
	friend void ::putPixel(SDL_Surface *surface, int x, int y, Uint32 color);
	void putPixel(ConstUPoint point, Uint32 color) {
	    ::putPixel(this, point.x, point.y, color);
	}

	//! Get pixel from given surface
	/*!
	  @param point coordinates of the pixel
	  @return read color

	  @warning The surface has to be locked !
	  */
	Uint32 getPixel(ConstUPoint point) const {
	    return ::getPixel((Image*)this, point.x, point.y);
	}

	//@}

	//! @name Drawing operations
	//@{

	//! Draw horizontal line
	/*!
	  @param start coordinates of start
	  @param x2 x-coord of finish
	  @param color color to draw with
	  @param lock whether to lock the surface (defaults to yes)
	  */
	void drawHLine(ConstUPoint start, int x2, Uint32 color, bool lock = true) {
	    ::drawHLine(this, start.x, start.y, x2, color, lock);
	}

	//! Draw vertical line
	/*!
	  @param start coordinates of start
	  @param y2 y-coord of finish
	  @param color color to draw with
	  @param lock whether to lock the surface (defaults to yes)
	  */
	void drawVLine(ConstUPoint start, int y2, Uint32 color, bool lock = true) {
	    ::drawVLine(this, start.x, start.y, y2, color, lock);
	}

	//! Draw rectangle
	/*!
	  @param rect rectangle to draw
	  @param color color to draw with
	  @param lock whether to lock the surface (defaults to yes)
	  */
	void drawRect(ConstRect rect, Uint32 color, bool lock = true) {
	    ::drawRect(this, rect, color, lock);
	}

	//@}

	//! @name Image operations
	//@{

	//! Make copy of the image
	ImagePtr getCopy() const {
	    return ImagePtr(new Image(copySurface((Image*)this)));
	}

	//! Make resized copy of the image
	/*!
	  @param ratio ratio of new image to original (2 = double size)
	  */
	ImagePtr getResized(const float ratio) {//const ?? what's the difference?
	    return ImagePtr(new Image(resizeSurface(this, ratio)));
	}

	//! Make resized copy of the image
	/*!
	  @param size size of the new image
	  */
	ImagePtr getResized(ConstUPoint size) {
	    return ImagePtr(new Image(resizeSurface(this, size.x, size.y)));
	}

	//! Make resized copy of the image relative to current resolution
	ImagePtr getResized() {
	    return getResized(UPoint(w, h).getScaled());
	}

	//! Cut out a rectangular region of the image
	/*!
	  @param dstRect geometry of rectangular region to cut out
	  */ 
	Image *getPictureCrop(ConstRect dstRect);

	//! Set colorkey
	/*!
	  @param color color which will be considered transparent
	  @param flags OR'd flags (see SDL_SetColorKey documentation for
	  details)
	  @note If you need to disable colorkey, think twice about your 
	  code...            
	  */
	void setColorKey(int color = 0, int flags = SDL_SRCCOLORKEY | SDL_RLEACCEL) {
	    SDL_SetColorKey(this, flags, color);
	}

	//@}

	//! @name Blit operations
	//@{

	//! Blit part of source image to this image
	/*!
	  @param source source image
	  @param srcRect part of source image to copy
	  @param dstPoint target coordinates (top-left corner)
	  */
	void blitFrom(Image *source, ConstRect srcRect, ConstUPoint dstPoint) {
	    assert(source != NULL);
	    Rect dstRect(Rect(dstPoint, source->getSize()));
	    SDL_BlitSurface(source, const_cast<SDL_Rect *>(static_cast<const SDL_Rect *>(&srcRect)), this, &dstRect); 
	}
	//! Blit whole source image to this image
	/*!
	  @param source source image
	  @param dstPoint target coordinates (top-left corner)
	  */
	void blitFrom(Image *source, ConstUPoint dstPoint) {
	    assert(source != NULL);
	    Rect dstRect(Rect(dstPoint, source->getSize()));
	    SDL_BlitSurface(source, NULL, this, &dstRect); 
	}
	//! Blit whole source image to this image (to top-left corner)
	/*!
	  @param source source image
	  */
	void blitFrom(Image *source) {
	    assert(source != NULL);
	    SDL_BlitSurface(source, NULL, this, NULL); 
	}
	//! Blit whole source image to this image (to center)
	/*!
	  @param source source image
	  */
	void blitFromCentered(Image *source) {
	    assert(source != NULL);
	    Rect dstRect(Rect(getSize()/2 - source->getSize()/2, source->getSize()));
	    SDL_BlitSurface(source, NULL, this, &dstRect); 
	}
	//! Blit part of the image to destination image
	/*!
	  @param destination destination image
	  @param srcRect part of source image to copy
	  @param dstPoint target coordinates (top-left corner)
	  */
	void blitTo(Image *destination, ConstRect srcRect, ConstUPoint dstPoint) const {
	    assert(destination != NULL);
	    destination->blitFrom((Image*)this, srcRect, dstPoint);
	}
	//! Blit the whole image to destination image
	/*!
	  @param destination destination image
	  @param dstPoint target coordinates (top-left corner)
	  */
	void blitTo(Image *destination, ConstUPoint dstPoint) const {
	    assert(destination != NULL);
	    destination->blitFrom((Image*)this, dstPoint);
	}
	//! Blit the whole image to destination image (to top-left corner)
	/*!
	  @param destination destination image
	  */
	void blitTo(Image *destination) const {
	    assert(destination != NULL);
	    destination->blitFrom((Image*)this);
	}
	//! Blit the whole image to destination image (to center)
	/*!
	  @param destination destination image
	  */
	void blitToCentered(Image *destination) const {
	    assert(destination != NULL);
	    destination->blitFromCentered((Image*)this);
	}
	//! Blit part of the image to screen
	/*!
	  @param srcRect part of source image to copy
	  @param dstPoint target coordinates (top-left corner)
	  */
	void blitToScreen(ConstRect srcRect, ConstUPoint dstPoint) const;
	//! Blit the whole image to screen
	/*!
	  @param dstPoint target coordinates (top-left corner)
	  */
	void blitToScreen(ConstUPoint dstPoint) const;

	//! Blit the whole image to destination image (to top-left corner)
	void blitToScreen() const;

	//! Blit the whole image to destination image (to center)
	void blitToScreenCentered() const;

	//@}

	//! @name Fill operations
	//@{

	void fillRect(Uint32 color, Rect *dstRect = NULL) {
	    SDL_FillRect(this, dstRect, color);
	}

	void fillRectVGradient(Uint32 color1, Uint32 color2, ConstRect dstRect);
	void fillRectHGradient(Uint32 color1, Uint32 color2, ConstRect dstRect);       

	//@}

	//! @name Color mapping
	//@{

	//! Set colormap from palette
	/*!
	  @param colors color map
	  @param firstcolor
	  @param ncolors
	  */
	int setColors(SDL_Color *colors, int firstcolor, int ncolors) {
	    return SDL_SetColors(this, colors, firstcolor, ncolors);
	}

	//! Remap colors of surface
	/*!
	  @param colorSrc first color to be replaced
	  @param colorDst first color to be used as replacement
	  @param colorNum number of colors to replace (last is colorSrc+colorNum-1)
	  */
	void recolor(int colorSrc, int colorDst, int colorNum = DEFAULT_SURFACE_REMAP_LENGTH) {
	    ::remapSurface(this, colorSrc, colorDst, colorNum);
	}

	//! Remap colors of surface
	/*!
	  @param house house to remap colors to

	  @note Starting color and color number defaults to hardcoded numbers.
	  */
	void recolorByHouse(int house) {
	    ::remapSurfaceByHouse(this, house);
	}

	//! Remap colors of surface
	/*!
	  @param color first color to be used as replacement

	  @note Starting color and color number defaults to hardcoded numbers.
	  */
	inline void recolorByColor(int color) {
	    ::remapSurfaceByColor(this, color);
	}

	//! Return copy with remapped colors of surface
	/*!
	  @param colorSrc first color to be replaced
	  @param colorDst first color to be used as replacement
	  @param colorNum number of colors to replace (last is colorSrc+colorNum-1)
	  */
	ImagePtr getRecolored(int colorSrc, int colorDst, int colorNum = DEFAULT_SURFACE_REMAP_LENGTH) const {
	    ImagePtr copy(getCopy());
	    copy->recolor(colorSrc, colorDst, colorNum);
	    return copy;
	}

	//! Return copy with remapped colors of surface
	/*!
	  @param house house to remap colors to

	  @note Starting color and color number defaults to hardcoded numbers.
	  */
	ImagePtr getRecoloredByHouse(int house) const {
	    ImagePtr copy(getCopy());
	    copy->recolorByHouse(house);
	    return copy;
	}

	//! Return copy with remapped colors of surface
	/*!
	  @param color first color to be used as replacement

	  @note Starting color and color number defaults to hardcoded numbers.
	  */
	ImagePtr getRecoloredByColor(int color) const {
	    ImagePtr copy(getCopy());
	    copy->recolorByColor(color);
	    return copy;
	}

	void drawBorders(std::string nw, std::string ne, std::string sw,
		std::string se, Uint16 edgeDistance = 0);

	void drawBorders(ImagePtr corner_nw, ImagePtr corner_ne,
		ImagePtr corner_sw, ImagePtr corner_se, ImagePtr top,
		ImagePtr bottom, ImagePtr left, ImagePtr right,
		Uint16 edgeDistance = 0);

	void drawBorders1(Uint16 edgeDistance = 0) {
	    drawBorders("UI_Corner1NW", "UI_Corner1NE", "UI_Corner1SW",
		    "UI_Corner1SE", edgeDistance);
	}

	void drawBorders2(Uint16 edgeDistance = 0) {
	    drawBorders("UI_Corner2NW", "UI_Corner2NE", "UI_Corner2SW",
		    "UI_Corner2SE", edgeDistance);
	}

	void drawBorders3(Uint16 edgeDistance = 0) {
	    drawBorders("UI_Corner3NW", "UI_Corner3NE", "UI_Corner3SW",
		    "UI_Corner3SE", edgeDistance);
	}

	//! Draw small horizontal bar
	/*!
	  @param start coordinates of start
	  @param x2 x-coord of finish
	  */
	void drawHBarSmall(ConstUPoint start, int x2);

	//! Draw vertical bar
	/*!
	  @param start coordinates of start
	  @param y2 y-coord of finish
	  */
	void drawVBar(ConstUPoint start, int y2);

	void drawTiles(ImagePtr tile);
	void drawTiles(ImagePtr tile, ConstRect area);

	bool fadeIn(const int fadeAmt = 4);
	bool fadeOut(const int fadeAmt = 4);
	bool morph(ImagePtr morphImg, const int morphAmt = 4);

	inline int flip() {
	    return SDL_Flip(this);
	};

	int saveBMP(std::string filename) {
	    return SDL_SaveBMP(this, filename.c_str());
	}

	inline void renderText(std::string text, Font *font, int offx, int offy, Uint8 paloff) {
	    font->render(text, this, offx, offy, paloff);
	}

	//@}


    private:
	Image() { }
	SDL_Color *m_origPal, *m_tmpPal;
};

//------------------------------------------------------------------------------
// DEPRECATED:
//------------------------------------------------------------------------------
//! @name DEPRECATED (do not use !)
//@{

/*!
  @deprecated Use putPixel(SDL_Surface *surface, int x, int y, Uint32 color) instead.
  */
inline void putpixel(SDL_Surface *surface, int x, int y, Uint32 colour) { putPixel(surface, x, y, colour); }

/*!
  @deprecated Use getPixel(SDL_Surface *surface, int x, int y); instead.
  */
inline Uint32 getpixel(SDL_Surface *surface, int x, int y) { return getPixel(surface, x, y); }

/*!
  @deprecated Use drawHLine(SDL_Surface *surface, int x, int y, int x2, Uint32 color, bool lock = true) with lock = false instead.
  */
inline void hlineNoLock(SDL_Surface *surface, int x1, int y, int x2, Uint32 colour) {
    drawHLine(surface, x1, y, x2, colour, false);
}

/*!
  @deprecated Use void drawHLine(SDL_Surface *surface, int x, int y, int x2, Uint32 color, bool lock = true) instead.
  */
inline void drawhline(SDL_Surface *surface, int x1, int y, int x2, Uint32 colour) {
    drawHLine(surface, x1, y, x2, colour, true);
}

/*!
  @deprecated Use drawRect(SDL_Surface *surface, const SDL_Rect &rect, Uint32 color, bool lock = true) instead.
  */
inline void drawrect(SDL_Surface *surface, int x1, int y1, int x2, int y2, Uint32 colour) { 
    SDL_Rect r;
    r.x = x1;
    r.y = y1;
    r.w = x2-x1;
    r.h = y2-y1;
    drawRect(surface, r, colour, true);
}

// DEPRECATED (works with surface, not with the 'Image' wrapper):
/*!
  @deprecated Use remapSurfaceByHouse(SDL_Surface *surface, int house) instead.
  */
inline void mapImageHouseColour(SDL_Surface* graphic, int house) {
    remapSurfaceByHouse(graphic, house);
}

/*!
  @deprecated If you really need to use this function, then your code is bad !
  @note <b>PENDING FOR REMOVAL:</b> it is used only by Game.cpp in doubtful way (otpetrik)
  */
inline void mapImageHouseColourBase(SDL_Surface* graphic, int house, int baseCol) {
    // slow, but it avoids additional include ;-) (it has to be enough till removed)
    remapSurface(graphic, baseCol, 144); // remap back to harkonnen
    remapSurfaceByHouse(graphic, house);
}

//@}

#endif // DUNE_GFX_H
