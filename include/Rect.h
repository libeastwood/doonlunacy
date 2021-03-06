#ifndef DUNE_RECT_H
#define DUNE_RECT_H

#include <SDL.h>

#include "Point.h"

//------------------------------------------------------------------------------
// Rect struct
//------------------------------------------------------------------------------
//! The Rect struct is an extension of SDL_Rect
/*!
    This struct provides convenient constructors and methods to test if Rect
    contains another Rect or Point.
    
    @note Member wariables are x,y (int16_t) and w,h (uint16_t) inherited from 
    SDL_Rect
    
    @note Implemented fully inline, so no need to worry about speed.
*/
struct Rect : public SDL_Rect
{
    public:
 
        //! @name Constructors & Destructor
        //@{

        //! Constructor
        Rect(int16_t x = 0, int16_t y = 0, uint16_t w = 0, uint16_t h = 0)
        {
            this->x = x;
            this->y = y;
            this->w = w;
            this->h = h;
        }
        //! Constructor
        Rect(const SPoint &position, const UPoint &size)
        {
            setPosition(position);
            setSize(size);
        }
	//! Destructor
	virtual ~Rect() { }

        //! Copy constructor
        Rect(const Rect &rect)
        {
            x = rect.x;
            y = rect.y;
            w = rect.w;
            h = rect.h;
        }
        //! Copy constructor
        Rect(const SDL_Rect &rect)
        {
            x = rect.x;
            y = rect.y;
            w = rect.w;
            h = rect.h;
        }

        //@}

        //! @name Operators
        //@{

        //! Type-casting operator to SDL_Rect
        operator SDL_Rect() const
        {
            return *this;
        }
        //! operator = (from SDL_Rect)
        Rect &operator=(const SDL_Rect &rect)
        {
            x = rect.x;
            y = rect.y;
            w = rect.w;
            h = rect.h;
            return *this;
	}

	//! operator !
	bool operator!()
	{
	    return ((x == 0) && (y == 0) &&
		    (w == 0) && (h == 0));
	}

	//! Compare two rectangles for equality.
	/*!
	  \return true if x / y coordinates and h / w are identical in both rectangles.
	  */  
	bool operator==(const SDL_Rect& rect) const {
	    return ((x == rect.x) && (y == rect.y) &&
		    (w == rect.w) && (h == rect.h));
	}

	bool operator!=(const SDL_Rect& rect) const {
	    return ((x != rect.x) || (y != rect.y) ||
		    (w != rect.w) || (h != rect.h));
	}

	friend std::ostream& operator<<(std::ostream& os, const Rect& c){
	    return os << "Rect(" << c.x << ',' << c.y << ',' << c.w << ',' << c.h << ')';
        }

	virtual operator std::string () const {
	    std::stringstream ss(std::stringstream::in | std::stringstream::out);
	    ss << *this;
	    return ss.str();
	}

	/*
	virtual operator const char *() const {
	    std::string ret = *this;
	    //FIXME: memory leakage..
	    return strdup(ret.c_str());
	}*/

        //@}

        //! @name Placement & Size
        //@{
        
        //! Set x,y to given position
        void setPosition(const SPoint &position)
        {
            x = position.x;
            y = position.y;
        }
        //! Set w,h to given size
        void setSize(const UPoint &size)
        {
            w = size.x;
            h = size.y;
        }
        //! Return current positon
        SPoint getPosition() const
        {
            return SPoint(x, y);                        
        }
        //! Return current size
        UPoint getSize() const
        {
            return UPoint(w, h);
        }

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

	//! How much of a rect inside this Rect?
        /*!
            @param r Rect in question
	    @return percent (1/100)
        */
	float contains (Rect r) {
	    float max;
	    Rect rect(*this);
	    if(getPosition() < r.getPosition())
		swap(rect, r);
	    if(!rect.containsPartial(r))
		return 0;
	    if(rect.containsWhole(r))
		return 1;
	    
	    if(getSize() < r.getSize())
		max = w*h;
	    else
		max = r.w*r.h;
	    SPoint containsPoint(rect.intersectRect(r).getSize());
	    
	    return (containsPoint.x*containsPoint.y)/max;
	}

        //! Is a rect inside this Rect ? (partial match required)
        /*!
            Whole Rect has to be inside or equal this Rect.
            @param r Rect in question
        */
        bool containsWhole(const Rect &r) const
        {
            return (
                    (x <= r.x && r.x+r.w <= x+w) &&
                    (y <= r.y && r.y+r.h <= y+h));                
        }
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
        }

	Rect intersectRect(const Rect &r) const
	{
	    int Amin, Amax, Bmin, Bmax;
	    Rect result;

	    /* Horizontal intersection */
	    Amin = x;
	    Amax = Amin + w;
	    Bmin = r.x;
	    Bmax = Bmin + r.w;
	    if (Bmin > Amin)
		Amin = Bmin;
	    result.x = Amin;
	    if (Bmax < Amax)
		Amax = Bmax;
	    result.w = Amax - Amin;

	    /* Vertical intersection */
	    Amin = y;
	    Amax = Amin + h;
	    Bmin = r.y;
	    Bmax = Bmin + r.h;
	    if (Bmin > Amin)
		Amin = Bmin;
	    result.y = Amin;
	    if (Bmax < Amax)
		Amax = Bmax;
	    result.h = Amax - Amin;

	    return result;
	}

        //@}
};

//! Constant Rect
typedef const Rect &ConstRect;

#endif // DUNE_RECT_H
