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
#include <eastwood/FntFile.h>
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
	  @warning Do not pass nullptr parameter !
	  */
	Image(const SDL_Surface *surface);

	Image(const eastwood::SDL::Surface &surface);

	Image() : eastwood::SDL::Surface(), m_origPal(0), m_tmpPal(0) { }
	Image &operator=(const SDL_Surface *surface);
	Image(const Image& image) :
		eastwood::SDL::Surface(image), m_origPal(image.m_origPal), m_tmpPal(image.m_tmpPal) {}

	//! Constructor
	/*!
	  @param size size of the image !
	  */
	Image(const UPoint& size);

	//! Destructor
	virtual ~Image() {};

	//@}

	friend class CutSceneState;

	//! @name Access methods
	//@{

	//! Returns size of the surface
	UPoint getSize() const {
	    return UPoint(_width, _height);
	}

	//! Returns palette of the surface
	inline eastwood::Palette getPalette() const {
	    return _palette;
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
	void putPixel(const UPoint& point, uint32_t color);

	//! Get pixel from given surface
	/*!
	  @param point coordinates of the pixel
	  @return read color

	  @warning The surface has to be locked !
	  */
	uint32_t getPixel(const UPoint& point) const;

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
	void drawRect(Rect& rect, uint32_t color, bool lock = true);

	//@}

	//! @name Image operations
	//@{

	//! Make copy of the image
	ImagePtr getCopy() const {
	    return std::make_shared<Image>(*this);
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
	ImagePtr getResized(const UPoint& size);

	//! Make resized copy of the image relative to current resolution
	ImagePtr getResized() {
	    return getResized(getSize().getScaled());
	}

	//! Cut out a rectangular region of the image
	/*!
	  @param dstRect geometry of rectangular region to cut out
	  */ 
	ImagePtr getPictureCrop(const Rect& dstRect);

	//! Set colorkey
	/*!
	  @param color color which will be considered transparent
	  @param flags OR'd flags (see SDL_SetColorKey documentation for
	  details)
	  @note If you need to disable colorkey, think twice about your 
	  code...            
	  */
	void setColorKey(int color = 0, int flags = SDL_SRCCOLORKEY | SDL_RLEACCEL) throw() {
	    SDL_SetColorKey(_surface, flags, color);
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
	void blitFrom(const Image& source, const Rect& srcRect, const UPoint& dstPoint) throw() {
	    Rect dstRect(Rect(dstPoint, source.getSize()));
	    SDL_BlitSurface(source, const_cast<Rect*>(&srcRect), _surface, &dstRect); 
	}
	//! Blit whole source image to this image
	/*!
	  @param source source image
	  @param dstPoint target coordinates (top-left corner)
	  */
	void blitFrom(const Image& source, const UPoint& dstPoint) throw() {
	    Rect dstRect(Rect(dstPoint, source.getSize()));
	    SDL_BlitSurface(source, nullptr, _surface, &dstRect); 
	}
	//! Blit whole source image to this image (to top-left corner)
	/*!
	  @param source source image
	  */
	void blitFrom(const Image& source) throw() {
	    if((void*)source != (void*)this)
		SDL_BlitSurface(source, nullptr, _surface, nullptr); 
	}
	//! Blit whole source image to this image (to center)
	/*!
	  @param source source image
	  */
	void blitFromCentered(const Image& source) throw() {
	    Rect dstRect(Rect(getSize()/2 - source.getSize()/2, source.getSize()));
	    SDL_BlitSurface(source, nullptr, _surface, &dstRect); 
	}
	//! Blit part of the image to destination image
	/*!
	  @param destination destination image
	  @param srcRect part of source image to copy
	  @param dstPoint target coordinates (top-left corner)
	  */
	void blitTo(Image& destination, const Rect& srcRect, const UPoint& dstPoint) const throw() {
	    destination.blitFrom(*this, srcRect, dstPoint);
	}
	//! Blit the whole image to destination image
	/*!
	  @param destination destination image
	  @param dstPoint target coordinates (top-left corner)
	  */
	void blitTo(Image& destination, const UPoint& dstPoint) const throw() {
	    destination.blitFrom(*this, dstPoint);
	}
	//! Blit the whole image to destination image (to top-left corner)
	/*!
	  @param destination destination image
	  */
	void blitTo(Image& destination) const throw() {
	    destination.blitFrom(*this);
	}
	//! Blit the whole image to destination image (to center)
	/*!
	  @param destination destination image
	  */
	void blitToCentered(Image& destination) const throw() {
	    destination.blitFromCentered(*this);
	}
	//! Blit part of the image to screen
	/*!
	  @param srcRect part of sou	rce image to copy
	  @param dstPoint target coordinates (top-left corner)
	  */
	void blitToScreen(const Rect& srcRect, const UPoint& dstPoint) const;
	//! Blit the whole image to screen
	/*!
	  @param dstPoint target coordinates (top-left corner)
	  */
	void blitToScreen(const UPoint& dstPoint) const;

	//! Blit the whole image to destination image (to top-left corner)
	void blitToScreen() const;

	//! Blit the whole image to destination image (to center)
	void blitToScreenCentered() const;

	//@}

	//! @name Fill operations
	//@{

	void fillRect(uint32_t color, Rect dstRect = Rect()) throw() {
	    SDL_FillRect(_surface, !dstRect ? nullptr : &dstRect, color);
	}

	void fillRectVGradient(uint32_t color1, uint32_t color2, const Rect& dstRect);
	void fillRectHGradient(uint32_t color1, uint32_t color2, const Rect& dstRect);       

	//@}

	//! @name Color mapping
	//@{

	//! Set colormap from palette
	/*!
	  @param colors color map
	  @param firstcolor
	  @param ncolors
	  */
	bool setPalette(const eastwood::Palette &palette, int firstColor = 0, int flags = (SDL_LOGPAL|SDL_PHYSPAL)) {
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
	void recolorByColor(int color) throw() {
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

	void drawBorders(const Image& corner_nw, const Image& corner_ne,
		const Image& corner_sw, const Image& corner_se, const Image& top,
		const Image& bottom, const Image& left, const Image& right,
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
	void drawHBarSmall(const UPoint& start, int x2);

	//! Draw vertical bar
	/*!
	  @param start coordinates of start
	  @param y2 y-coord of finish
	  */
	void drawVBar(const UPoint& start, int y2);

	void drawTiles(const Image& tile, Rect area = Rect());

	bool fadeIn(const int fadeAmt = 4);
	bool fadeOut(const int fadeAmt = 4);
	bool morph(ImagePtr morphImg, const int morphAmt = 4);

	void flipH();
	void flipV();

	int flip() throw() {
	    return SDL_Flip(_surface);
	};

	int saveBMP(std::string filename) throw() {
	    return SDL_SaveBMP(_surface, filename.c_str());
	}

	inline void renderText(std::string text, eastwood::FntFile *font, int offx, int offy, uint8_t paloff) {
	    font->render(text, *this, offx, offy, paloff);
	}

	inline bool mustLock() throw() {
	    return SDL_MUSTLOCK(_surface);
	}

	inline int lockSurface() throw() {
	    return SDL_LockSurface(_surface);
	}

	inline void unlockSurface() throw() {
	    SDL_UnlockSurface(_surface);
	}

	//@}


    private:

	eastwood::Palette m_origPal, m_tmpPal;
};

#endif // DUNE_GFX_H
