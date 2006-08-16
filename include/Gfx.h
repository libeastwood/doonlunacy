/*!
    \file Gfx.h
    \brief Various gfx routines.   
    
    @note Use Image class and coresponding methods. SDL_Surface-based functions will
    be removed by moving their code directly into the Image class.
    
*/    

#ifndef DUNE_GFX_H
#define DUNE_GFX_H

#include "SDL.h"

#include "Colours.h"

#include <boost/shared_ptr.hpp>

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
SDL_Surface* resizeSurface(SDL_Surface *surface, double ratio);


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
// Classes & Structures
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Point struct
//------------------------------------------------------------------------------
//! The Point struct is basic representation of a point
/*!
    @note Implemented fully inline, so no need to worry about speed.
*/
template <typename T>
struct Point
{
    public:
        //! @name Member variables
        //@{

        //! x-coordinate
        T x;
        //! y-coordinate
        T y;

        //@}
        

        //! @name Constructors & Destructor
        //@{

        //! Constructor
        Point(T x = 0, T y = 0) : x(x), y(y) {  };
        //! Copy constructor
        Point(const Point &point) { *this = point; };
        
        //@}

        //! @name Operators
        //@{
        
        //! operator =
        Point &operator=(const Point &point)
        {
            x = point.x;
            y = point.y;
            return *this;
        };
        //! operator +=
        Point &operator+=(const Point &point)
        {
            x += point.x;
            y += point.y;
            return *this;
        };
        //! operator -=
        Point &operator-=(const Point &point)
        {
            x += point.x;
            y += point.y;
            return *this;
        };
        //! operator *=
        Point &operator*=(T times)
        {
            x *= times;
            y *= times;
            return *this;
        };
        //! operator /=
        /*
            @warning: Do not divide by zero !
        */
        Point &operator/=(T times)
        {
            // TODO: assert ?
            x /= times;
            y /= times;
            return *this;
        };
        
        //! operator +
        Point operator+(const Point &point) const
        {
            return Point(x + point.x, y + point.y);
        };
        //! operator -
        Point operator-(const Point &point) const
        {
            return Point(x - point.x, y - point.y);
        };
        //! operator *
        Point operator*(T times) const
        {
            return Point(x*times, y*times);
        };
        //! operator /
        /*!
            @warning: Do not divide by zero !
        */
        Point operator/(T times) const
        {
            // TODO: assert ?
            return Point(x/times, y/times);
        };
        
        //! Type-casting operator
        /*!
            If it doesn't compile, then the types are incompatible.
        */
        template <typename T2>
        operator Point<T2>() const
        {
            return Point<T2>(x, y);
        }

        //@}

        //! @name Miscellaneous methods
        //@{
        
        //! Swap x<->y coordinates
        const Point &swap()
        {
            T t = x;
            x = y;
            y = t;
        };
        
        //! Return point with swapped x<->y coordinates
        Point getSwapped() const
        {
            return Point(y, x);
        };
    
        //@}
};

//! Point with unsigned coordinates
typedef Point<Uint16> UPoint;
//! Constant Point with unsigned coordinates
typedef const UPoint &ConstUPoint;

//! Point with signed coordinates
typedef Point<Sint16> SPoint;
//! Constant Point with signed coordinates
typedef const SPoint &ConstSPoint;

//! Point with signed 32-bit coordinates
typedef Point<Sint32> SPointBig;
//! Constant Point with signed 32-bit coordinates
typedef const SPointBig &ConstSPointBig;

//! Point with float coordinates
typedef Point<float>  PointFloat;
//! Constant Point with float coordinates
typedef const PointFloat &ConstPointFloat;

//! @name Unary minus operators for signed points
//! @relates Point
//@{
inline SPoint operator-(const SPoint &point)
{
    return SPoint(point.x, point.y);
}

inline SPointBig operator-(const SPointBig &point)
{
    return SPointBig(point.x, point.y);
}

inline PointFloat operator-(const PointFloat &point)
{
    return PointFloat(point.x, point.y);
}
//@}


//------------------------------------------------------------------------------
// Rect struct
//------------------------------------------------------------------------------
//! The Rect struct is an extension of SDL_Rect
/*!
    This struct provides convenient constructors and methods to test if Rect
    contains another Rect or Point.
    
    @note Member wariables are x,y (Sint16) and w,h (Uint16) inherited from 
    SDL_Rect
    
    @note Implemented fully inline, so no need to worry about speed.
*/
struct Rect : public SDL_Rect
{
    public:
 
        //! @name Constructors & Destructor
        //@{

        //! Constructor
        Rect(Sint16 x = 0, Sint16 y = 0, Uint16 w = 0, Uint16 h = 0)
        {
            this->x = x;
            this->y = y;
            this->w = w;
            this->h = h;
        };
        //! Constructor
        Rect(const SPoint &position, const UPoint &size)
        {
            setPosition(position);
            setSize(size);
        };
        //! Copy constructor
        Rect(const Rect &rect)
        {
            x = rect.x;
            y = rect.y;
            w = rect.w;
            h = rect.h;
        };
        //! Copy constructor
        Rect(const SDL_Rect &rect)
        {
            x = rect.x;
            y = rect.y;
            w = rect.w;
            h = rect.h;
        };

        //@}

        //! @name Operators
        //@{

        //! Type-casting operator to SDL_Rect
        operator SDL_Rect() const
        {
            return *this;
        };
        //! operator = (from SDL_Rect)
        Rect &operator=(const SDL_Rect &rect)
        {
            x = rect.x;
            y = rect.y;
            w = rect.w;
            h = rect.h;
            return *this;
        }

        //@}

        //! @name Placement & Size
        //@{
        
        //! Set x,y to given position
        void setPosition(const SPoint &position)
        {
            x = position.x;
            y = position.y;
        };
        //! Set w,h to given size
        void setSize(const UPoint &size)
        {
            w = size.x;
            h = size.y;
        };
        //! Return current positon
        SPoint getPosition() const
        {
            return SPoint(x, y);                        
        };
        //! Return current size
        UPoint getSize() const
        {
            return UPoint(x, y);
        };

        //@}
       
        //! @name Queries
        //@{

        //! Is point inside this Rect ?
        /*!
            @param p Point in question
        */
        template<typename T>
        bool contains(const Point<T> &p) const
        {
            return (
                    (x <= p.x && p.x <= x+w) &&
                    (y <= p.y && p.y <= y+h));
        }
        
        //! Is a rect inside this Rect ? (partial match required)
        /*!
            Whole Rect has to be inside or equal this Rect.
            @param r Roint in question
        */
        bool containsWhole(const Rect &r) const
        {
            return (
                    (x <= r.x && r.x+r.w <= x+w) &&
                    (y <= r.y && r.y+r.h <= y+h));                
        };
        //! Is a rect inside this Rect ? (partial match is enough)
        /*!
            Rect in question must have at least a pixel common with this Rect.
            @param r Rect in question
        */
        bool containsPartial(const Rect &r) const
        {
            return (
                    (x <= r.x+r.w && r.x <= x+w) &&
                    (y <= r.y+r.h && r.y <= y+h));                
        };

        //@}
};

//! Constant Rect
typedef const Rect &ConstRect;

//------------------------------------------------------------------------------
// Image class
//------------------------------------------------------------------------------

class Image;

//! Smart pointer around Image class instance
typedef boost::shared_ptr<Image> ImagePtr;

//! Smart pointer around Image class const instance
typedef boost::shared_ptr<const Image> ConstImagePtr;

//! The Image class is a wrapper around SDL_Surface
/*!
    It allows using SDL_Surfaces with smart pointers.
    Use ConstImagePtr and ImagePtr where possible (former is more
    favorable), it avoids memory leaks...
        
*/
class Image
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
            @param Size of the image !
        */
        Image(ConstUPoint size);
        
        //! Destructor
        ~Image();

        //@}

        //! @name Operators
        //@{

        //! Returns the surface
        operator SDL_Surface*();

        //@}
        
        //! @name Access methods
        //@{

        //! Returns the surface
        SDL_Surface *getSurface();
        
        //! Returns size of the surface
        UPoint getSize() const
        {
            return UPoint(surface->w, surface->h);
        };

        //@}
        
        //! @name Single pixel operations
        //@{

        //! Set pixel on given surface
        /*!
            @param point coordinates of the pixel
            @param color color
    
            @warning The surface has to be locked !
        */
        void putPixel(ConstUPoint point, Uint32 color)
        {
            ::putPixel(surface, point.x, point.y, color);
        };

        //! Get pixel from given surface
        /*!
            @param point coordinates of the pixel
            @return read color
    
            @warning The surface has to be locked !
        */
        Uint32 getPixel(ConstUPoint point) const
        {
            return ::getPixel(surface, point.x, point.y);
        };

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
        void drawHLine(ConstUPoint start, int x2, Uint32 color, bool lock = true)
        {
            ::drawHLine(surface, start.x, start.y, x2, color, lock);
        };

        //! Draw vertical line
        /*!
            @param start coordinates of start
            @param y2 y-coord of finish
            @param color color to draw with
            @param lock whether to lock the surface (defaults to yes)
        */
        void drawVLine(ConstUPoint start, int y2, Uint32 color, bool lock = true)
        {
            ::drawVLine(surface, start.x, start.y, y2, color, lock);
        };

        //! Draw rectangle
        /*!
            @param rect rectangle to draw
            @param color color to draw with
            @param lock whether to lock the surface (defaults to yes)
        */
        void drawRect(ConstRect rect, Uint32 color, bool lock = true)
        {
            ::drawRect(surface, rect, color, lock);
        };

        //@}
        
        //! @name Image operations
        //@{

        //! Make copy of the image
        ImagePtr getCopy() const
        {
            return ImagePtr(new Image(copySurface(surface)));
        };

        //! Make resized copy of the image
        /*!
            @param ratio ratio of new image to original (2 = double size)
        */
        ImagePtr getResized(double ratio) const
        {
            return ImagePtr(new Image(resizeSurface(surface, ratio)));
        };

        //! Make resized copy of the image
        /*!
            @param size size of the new image
        */
        ImagePtr getResized(ConstUPoint size)
        {
            return ImagePtr(new Image(resizeSurface(surface, size.x, size.y)));
        };

        
        //! Set colorkey
        /*!
            @param color color which will be considered transparent
            @param flags OR'd flags (see SDL_SetColorKey documentation for
            details)
            @note If you need to disable colorkey, think twice about your 
            code...            
        */
        void setColorKey(int color = 0, int flags = SDL_SRCCOLORKEY | SDL_RLEACCEL)
        {
            SDL_SetColorKey(surface, flags, color);
        };

        //@}

        //! @name Blit operations
        //@{
        
        //! Blit part of source image to this image
        /*!
            @param source source image
            @param srcRect part of source image to copy
            @param dstPoint target coordinates (top-left corner)
        */
        void blitFrom(const Image *source, ConstRect srcRect, ConstUPoint dstPoint)
        {
            assert(source != NULL);
            Rect dstRect(Rect(dstPoint, source->getSize()));
            SDL_BlitSurface(source->surface, const_cast<SDL_Rect *>(static_cast<const SDL_Rect *>(&srcRect)), surface, &dstRect); 
        };
        //! Blit whole source image to this image
        /*!
            @param source source image
            @param dstPoint target coordinates (top-left corner)
        */
        void blitFrom(const Image *source, ConstUPoint dstPoint)
        {
            assert(source != NULL);
            Rect dstRect(Rect(dstPoint, source->getSize()));
            SDL_BlitSurface(source->surface, NULL, surface, &dstRect); 
        };
        //! Blit whole source image to this image (to top-left corner)
        /*!
            @param source source image
        */
        void blitFrom(const Image *source)
        {
            assert(source != NULL);
            SDL_BlitSurface(source->surface, NULL, surface, NULL); 
        };
        //! Blit whole source image to this image (to center)
        /*!
            @param source source image
        */
        void blitFromCentered(const Image *source)
        {
            assert(source != NULL);
            Rect dstRect(Rect(getSize()/2 - source->getSize()/2, source->getSize()));
            SDL_BlitSurface(source->surface, NULL, surface, &dstRect); 
        };
        //! Blit part of the image to destination image
        /*!
            @param destination destination image
            @param srcRect part of source image to copy
            @param dstPoint target coordinates (top-left corner)
        */
        void blitTo(Image *destination, ConstRect srcRect, ConstUPoint dstPoint) const
        {
            assert(destination != NULL);
            destination->blitFrom(this, srcRect, dstPoint);
        };
        //! Blit the whole image to destination image
        /*!
            @param destination destination image
            @param dstPoint target coordinates (top-left corner)
        */
        void blitTo(Image *destination, ConstUPoint dstPoint) const
        {
            assert(destination != NULL);
            destination->blitFrom(this, dstPoint);
        }
        //! Blit the whole image to destination image (to top-left corner)
        /*!
            @param destination destination image
        */
        void blitTo(Image *destination) const
        {
            assert(destination != NULL);
            destination->blitFrom(this);
        }
        //! Blit the whole image to destination image (to center)
        /*!
            @param destination destination image
        */
        void blitToCentered(Image *destination) const
        {
            assert(destination != NULL);
            destination->blitFromCentered(this);
        }
        //! Blit part of the image to screen
        /*!
            @param destination destination image
            @param srcRect part of source image to copy
            @param dstPoint target coordinates (top-left corner)
        */
        void blitToScreen(ConstRect srcRect, ConstUPoint dstPoint) const;
        //! Blit the whole image to screen
        /*!
            @param destination destination image
            @param dstPoint target coordinates (top-left corner)
        */
        void blitToScreen(ConstUPoint dstPoint) const;
        //! Blit the whole image to destination image (to top-left corner)
        /*!
            @param destination destination image
        */
        void blitToScreen() const;
        //! Blit the whole image to destination image (to center)
        /*!
            @param destination destination image
        */
        void blitToScreenCentered() const;

        //@}

        //! @name Fill operations
        //@{
        
        void fillRect(Uint32 color, Rect dstRect)
        {
            SDL_FillRect(surface, &dstRect, color);
        };
        void fillRect(Uint32 color)
        {
            SDL_FillRect(surface, NULL, color);
        };
        
        void fillRectVGradient(Uint32 color1, Uint32 color2, ConstRect dstRect);
        void fillRectHGradient(Uint32 color1, Uint32 color2, ConstRect dstRect);       

        //@}

        //! @name Color mapping
        //@{

        //! Remap colors of surface
        /*!
            @param colorSrc first color to be replaced
            @param colorDst first color to be used as replacement
            @param colorNum number of colors to replace (last is colorSrc+colorNum-1)
        */
        void recolor(int colorSrc, int colorDst, int colorNum = DEFAULT_SURFACE_REMAP_LENGTH)
        {
            ::remapSurface(surface, colorSrc, colorDst, colorNum);
        };

        //! Remap colors of surface
        /*!
            @param house house to remap colors to

            @note Starting color and color number defaults to hardcoded numbers.
        */
        void recolorByHouse(int house)
        {
            ::remapSurfaceByHouse(surface, house);
        }

        //! Remap colors of surface
        /*!
            @param color first color to be used as replacement

            @note Starting color and color number defaults to hardcoded numbers.
        */
        inline void recolorByColor(int color)
        {
            ::remapSurfaceByColor(surface, color);
        };

        //! Return copy with remapped colors of surface
        /*!
            @param colorSrc first color to be replaced
            @param colorDst first color to be used as replacement
            @param colorNum number of colors to replace (last is colorSrc+colorNum-1)
        */
        ImagePtr getRecolored(int colorSrc, int colorDst, int colorNum = DEFAULT_SURFACE_REMAP_LENGTH) const
        {
            ImagePtr copy(getCopy());
            copy->recolor(colorSrc, colorDst, colorNum);
            return copy;
        };

        //! Return copy with remapped colors of surface
        /*!
            @param house house to remap colors to

            @note Starting color and color number defaults to hardcoded numbers.
        */
        ImagePtr getRecoloredByHouse(int house) const
        {
            ImagePtr copy(getCopy());
            copy->recolorByHouse(house);
            return copy;
        };

        //! Return copy with remapped colors of surface
        /*!
            @param color first color to be used as replacement

            @note Starting color and color number defaults to hardcoded numbers.
        */
        ImagePtr getRecoloredByColor(int color) const
        {
            ImagePtr copy(getCopy());
            copy->recolorByColor(color);
            return copy;
        };


        //@}
        
        
    private:
        SDL_Surface *surface;
        Image() { };
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
inline void hlineNoLock(SDL_Surface *surface, int x1, int y, int x2, Uint32 colour)
{
    drawHLine(surface, x1, y, x2, colour, false);
}

/*!
    @deprecated Use void drawHLine(SDL_Surface *surface, int x, int y, int x2, Uint32 color, bool lock = true) instead.
*/
inline void drawhline(SDL_Surface *surface, int x1, int y, int x2, Uint32 colour)
{
    drawHLine(surface, x1, y, x2, colour, true);
}

/*!
    @deprecated Use drawRect(SDL_Surface *surface, const SDL_Rect &rect, Uint32 color, bool lock = true) instead.
*/
inline void drawrect(SDL_Surface *surface, int x1, int y1, int x2, int y2, Uint32 colour)
{ 
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
inline void mapImageHouseColour(SDL_Surface* graphic, int house)
{
    remapSurfaceByHouse(graphic, house);
}

/*!
    @deprecated If you really need to use this function, then your code is bad !
    @note <b>PENDING FOR REMOVAL:</b> it is used only by Game.cpp in doubtful way (otpetrik)
*/
inline void mapImageHouseColourBase(SDL_Surface* graphic, int house, int baseCol)
{
    // slow, but it avoids additional include ;-) (it has to be enough till removed)
    remapSurface(graphic, baseCol, 144); // remap back to harkonnen
    remapSurfaceByHouse(graphic, house);
}

//@}

#endif // DUNE_GFX_H
