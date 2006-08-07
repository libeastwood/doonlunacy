/*!
    \file Strings.h
    \brief String related stuff

    @note
    There are only typedefs for now, but soon there will be caches to parse strings easier.
*/    
#ifndef DUNE_STRINGS_H
#define DUNE_STRINGS_H

#include <string>
#include <sstream>

//! Shortcut to std::string
typedef std::string String;
//! Shortcut to const std::string &
/*
    @warning It is reference, thus you cannot create array,map,... of type ConstString !
*/
typedef const std::string &ConstString;

//------------------------------------------------------------------------------
// Misc functions
//------------------------------------------------------------------------------
//! @name Misc functions
//@{

//! Convert something to string
/*
Tries to convert value to string (uses std::ostringstream).
*/
template <typename T>
String toString(const T &something)
{
    std::ostringstream s;
    s << something;
    return s.str();
};

//! Convert string to something
/*
Tries to convert string to something (uses std::ostringstream).

@note Use this way:
@code
    myvariable = fromStrint<mytype>(mystring);
@endcode
*/
template <typename T>
T fromString(ConstString string)
{
    T something;
    std::istringstream s(string);
    s >> something;
    return something;
};

//@}

#endif // DUNE_STRINGS_H
