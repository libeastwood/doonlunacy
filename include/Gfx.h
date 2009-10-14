/*!
  \file Gfx.h
  \brief Various gfx routines.   

  @note Use Image class and coresponding methods. SDL_Surface-based functions will
  be removed by moving their code directly into the Image class.

*/    

#ifndef DUNE_GFX_H
#define DUNE_GFX_H

#include "Definitions.h"
#include <string>
#include <eastwood/Font.h>
#include <eastwood/SDL/Surface.h>

#include "Colours.h"
#include "Point.h"
#include "Rect.h"

//------------------------------------------------------------------------------
// Color remapping
//------------------------------------------------------------------------------
//! @name Color mapping
//@{

#define DEFAULT_SURFACE_REMAP_LENGTH 7
#define DEFAULT_SURFACE_REMAP_BEGIN COLOUR_HARKONNEN

//------------------------------------------------------------------------------
// Image class
//------------------------------------------------------------------------------

class CutSceneState;

class Image : protected eastwood::SDL::Surface
{
    public:

	//! @name Constructors & Destructor
	//@{

	//! Constructor
	/*!
	  @warning Do not pass NULL parameter !
	  */
	Image(SDL_Surface *surface);

	Image(const eastwood::SDL::Surface &surface);

	Image() : eastwood::SDL::Surface() { }
	Image &operator=(const SDL_Surface *surface);

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

	//! Returns palette of the surface
	inline SDL_Palette *getPalette() const {
	    return format->palette;
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
	void putPixel(ConstUPoint point, uint32_t color);

	//! Get pixel from given surface
	/*!
	  @param point coordinates of the pixel
	  @return read color

	  @warning The surface has to be locked !
	  */
	uint32_t getPixel(ConstUPoint point) const;

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
	void drawHLine(UPoint start, int x2, uint32_t color, bool lock = true);

	//! Draw vertical line
	/*!
	  @param start coordinates of start
	  @param y2 y-coord of finish
	  @param color color to draw with
	  @param lock whether to lock the surface (defaults to yes)
	  */
	void drawVLine(UPoint start, int y2, uint32_t color, bool lock = true);

	//! Draw rectangle
	/*!
	  @param rect rectangle to draw
	  @param color color to draw with
	  @param lock whether to lock the surface (defaults to yes)
	  */
	void drawRect(ConstRect rect, uint32_t color, bool lock = true);

	//@}

	//! @name Image operations
	//@{

	//! Make copy of the image
	ImagePtr getCopy() const {
	    return ImagePtr(new Image(SDL_ConvertSurface((Image*)this, format, flags)));
	}

	//! Make resized copy of the image
	/*!
	  @param ratio ratio of new image to original (2 = double size)
	  */
	ImagePtr getResized(const float ratio) {//const ?? what's the difference?
	    return getResized(getSize()*ratio);
	}

	//! Make resized copy of the image
	/*!
	  @param size size of the new image
	  */
	ImagePtr getResized(ConstUPoint size);

	//! Make resized copy of the image relative to current resolution
	ImagePtr getResized() {
	    return getResized(UPoint(w, h).getScaled());
	}

	//! Cut out a rectangular region of the image
	/*!
	  @param dstRect geometry of rectangular region to cut out
	  */ 
	ImagePtr getPictureCrop(ConstRect dstRect);

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

	void fillRect(uint32_t color, Rect dstRect = Rect()) {
	    SDL_FillRect(this, !dstRect ? NULL : &dstRect, color);
	}

	void fillRectVGradient(uint32_t color1, uint32_t color2, ConstRect dstRect);
	void fillRectHGradient(uint32_t color1, uint32_t color2, ConstRect dstRect);       

	//@}

	//! @name Color mapping
	//@{

	//! Set colormap from palette
	/*!
	  @param colors color map
	  @param firstcolor
	  @param ncolors
	  */
	bool setPalette(SDL_Palette *palette, int firstColor = 0, int flags = (SDL_LOGPAL|SDL_PHYSPAL)) {
	    return SDL_SetPalette(this, flags, palette->colors, firstColor, palette->ncolors);
	}

	bool setPalette(eastwood::Palette palette, int firstColor = 0, int flags = (SDL_LOGPAL|SDL_PHYSPAL)) {
	    return eastwood::SDL::Surface::setPalette(palette, firstColor, flags);
	}

	//! Remap colors of surface
	/*!
	  @param colorSrc first color to be replaced
	  @param colorDst first color to be used as replacement
	  @param colorNum number of colors to replace (last is colorSrc+colorNum-1)
	  */
	void recolor(int colorSrc, int colorDst, int colorNum = DEFAULT_SURFACE_REMAP_LENGTH);

	//! Remap colors of surface
	/*!
	  @param house house to remap colors to

	  @note Starting color and color number defaults to hardcoded numbers.
	  */
	void recolorByHouse(int house);

	//! Remap colors of surface
	/*!
	  @param color first color to be used as replacement

	  @note Starting color and color number defaults to hardcoded numbers.
	  */
	inline void recolorByColor(int color) {
	    recolor(DEFAULT_SURFACE_REMAP_BEGIN, color);
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

	void greyOut(Rect area = Rect());
	void drawBorders(std::string nw, std::string ne, std::string sw,
		std::string se, uint16_t edgeDistance = 0);

	void drawBorders(ImagePtr corner_nw, ImagePtr corner_ne,
		ImagePtr corner_sw, ImagePtr corner_se, ImagePtr top,
		ImagePtr bottom, ImagePtr left, ImagePtr right,
		uint16_t edgeDistance = 0);

	void drawBorders1(uint16_t edgeDistance = 0) {
	    drawBorders("UI_Corner1NW", "UI_Corner1NE", "UI_Corner1SW",
		    "UI_Corner1SE", edgeDistance);
	}

	void drawBorders2(uint16_t edgeDistance = 0) {
	    drawBorders("UI_Corner2NW", "UI_Corner2NE", "UI_Corner2SW",
		    "UI_Corner2SE", edgeDistance);
	}

	void drawBorders3(uint16_t edgeDistance = 0) {
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

	void drawTiles(ImagePtr tile, Rect area = Rect());

	bool fadeIn(const int fadeAmt = 4);
	bool fadeOut(const int fadeAmt = 4);
	bool morph(ImagePtr morphImg, const int morphAmt = 4);

	void flipH();
	void flipV();

	inline int flip() {
	    return SDL_Flip(this);
	};

	int saveBMP(std::string filename) {
	    return SDL_SaveBMP(this, filename.c_str());
	}

	inline void renderText(std::string text, eastwood::Font *font, int offx, int offy, uint8_t paloff) {
	    font->render(text, *this, offx, offy, paloff);
	}

	inline bool mustLock() {
	    return SDL_MUSTLOCK(this);
	}

	inline int lockSurface() {
	    return SDL_LockSurface(this);
	}

	inline void unlockSurface() {
	    SDL_UnlockSurface(this);
	}

	//@}


    private:
	eastwood::Palette m_origPal, m_tmpPal;
};

#endif // DUNE_GFX_H
