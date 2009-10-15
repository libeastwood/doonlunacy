#ifndef DUNE_VALMATRIX_H
#define DUNE_VALMATRIX_H

#include <valarray>

template <typename T>

class ValMatrix : public std::valarray<T>
{
    public:
    	ValMatrix(size_t rows, size_t colons) :
	    std::valarray<T>(rows*colons), _rows(rows), _colons(colons) {};

	T & operator()(size_t r, size_t c) throw() {
	    return (*this)[r * _colons + c];
	} 
	T operator()(size_t r, size_t c) const throw() {
	    return (*this)[r * _colons + c];
	}

	size_t rows() const throw() { return _rows; };
	size_t colons() const throw() { return _colons; };

    protected:
	size_t _rows,
	       _colons;
};

#endif // DUNE_VALMATRIX_H
