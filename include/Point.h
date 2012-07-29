#ifndef DUNE_POINT_H
#define DUNE_POINT_H

#include <assert.h>
#include <cmath>
#include <sstream>
#include <string>
#include <SDL.h>

#include "Definitions.h"


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

	//! Destructor
	virtual ~Point() { }

        //! Copy constructor
        Point(const Point &point) : x(point.x), y(point.y) { }
        
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

	Point &operator+=(T value)
        {
            x += value;
            y += value;
            return *this;
        }

        //! operator -=
        Point &operator-=(const Point &point)
        {
            x -= point.x;
            y -= point.y;
            return *this;
        }

        Point &operator-=(T value)
        {
            x -= value;
            y -= value;
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

	virtual operator std::string() const {
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
	    const SDL_VideoInfo *videoInfo = SDL_GetVideoInfo();
            return Point((videoInfo->current_w / 320.0f) * x,
                         (videoInfo->current_h / 200.0f) * y);
        }

	Point getAbs() const {
	    return Point(abs(x), abs(y));
	}

	float distance(const Point &point) const {
    	    float dx = x - point.x,
		  dy = y - point.y;
    	    return(std::sqrt(dx*dx + dy*dy));
	}

	float angle(const Point &point) const {
	    float dx = x-point.x, dy = y - point.y;
	    float angle = acos(-dx/distance(point)) * 180 / M_PI;
	    if(dy < 0)
		angle = 360-angle; 
	    return angle;
	}

        //@}
};

//! Point with unsigned coordinates
typedef Point<uint16_t> UPoint;
//! Constant Point with unsigned coordinates
typedef const UPoint &ConstUPoint;

//! Point with signed coordinates
typedef Point<int16_t> SPoint;
//! Constant Point with signed coordinates
typedef const SPoint &ConstSPoint;

//! Point with signed 32-bit coordinates
typedef Point<int32_t> SPointBig;
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
