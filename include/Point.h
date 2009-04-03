#ifndef DUNE_POINT_H
#define DUNE_POINT_H

#include "Settings.h"

#include <assert.h>
#include <iostream>

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
        Point(T x = 0, T y = 0) : x(x), y(y) {  }
        //! Copy constructor
        Point(const Point &point) { *this = point; }
        
        //@}

        //! @name Operators
        //@{
        
        //! operator =
        Point &operator=(const Point &point)
        {
            x = point.x;
            y = point.y;
            return *this;
        }

        //! operator =
        Point &operator=(T value)
        {
	    x = y = value;
            return *this;
        }

        //! operator +=
        Point &operator+=(const Point &point)
        {
            x += point.x;
            y += point.y;
            return *this;
        }
        //! operator -=
        Point &operator-=(const Point &point)
        {
            x += point.x;
            y += point.y;
            return *this;
        }
        //! operator *=
        Point &operator*=(T times)
        {
            x *= times;
            y *= times;
            return *this;
        }
        //! operator /=
        Point &operator/=(T times)
        {
	    assert(times != 0);
            x /= times;
            y /= times;
            return *this;
        }
        
        //! operator +
        Point operator+(const Point &point) const
        {
            return Point(x + point.x, y + point.y);
        }

        Point operator+(T value) const
        {
            return Point(x + value, y + value);
        }

        //! operator -
        Point operator-(const Point &point) const
        {
            return Point(x - point.x, y - point.y);
        }

        Point operator-(T value) const
        {
            return Point(x - value, y - value);
        }

        //! operator *
        Point operator*(T times) const
        {
            return Point(x*times, y*times);
        }
        //! operator /
        Point operator/(T times) const
        {
	    assert(times != 0);
            return Point(x/times, y/times);
        }
        
        friend bool operator==(const Point& a, const Point& b){
            return (a.x == b.x && a.y == b.y);
        }

        friend bool operator!=(const Point& a, const Point& b){
            return (a.x != b.x || a.y != b.y);
	}

	bool operator<(const Point &point) const {
	    return x < point.x && y < point.y;
	}

	bool operator<=(const Point &point) const {
	    return x <= point.x && y <= point.y;
	}

	bool operator>(const Point &point) const {
	    return !operator<(point);
	}

	bool operator>=(const Point &point) const {
	    return !operator<=(point);
	}

	friend std::ostream& operator<<(std::ostream& os, const Point& c){
	    return os << '(' << c.x << 'x' << c.y << ')';
	}

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
        }
        
        //! Return point with swapped x<->y coordinates
        Point getSwapped() const
        {
            return Point(y, x);
        }

	//! Return point scaled to current resolution
        Point getScaled() const
        {
            return Point((Settings::Instance()->GetWidth()  / 320.0f) * x,
                         (Settings::Instance()->GetHeight() / 200.0f) * y);
        }
             
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

#endif // DUNE_POINT_H
