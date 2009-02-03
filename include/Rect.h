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
        }
        //! Constructor
        Rect(const SPoint &position, const UPoint &size)
        {
            setPosition(position);
            setSize(size);
        }
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

        friend std::ostream& operator<<(std::ostream& os, const Rect& c){
            return os << "Rect(" << c.x << ',' << c.y << ',' << c.w << ',' << c.h << ')';
        }

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

        //@}
};

//! Constant Rect
typedef const Rect &ConstRect;

#endif // DUNE_RECT_H
