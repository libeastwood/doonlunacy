/*!
    \file Strings.h
    \brief String related stuff

    TODO: add something to get position from the input caches (row & column) !
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
inline String toString(const T &something)
{
    std::ostringstream s;
    s << something;
    return s.str();
}
template <>
inline String toString(const bool &something)
{
    std::ostringstream s;
    if (something)
        s << "true";
    else
        s << "false";
    return s.str();
}

//! Convert string to something
/*
Tries to convert string to something (uses std::ostringstream).

@note Use this way:
@code
    myvariable = fromStrint<mytype>(mystring);
@endcode
*/
template <typename T>
inline T fromString(ConstString string)
{
    T something;
    std::istringstream s(string);
    s >> something;
    return something;
}
template <>
inline bool fromString(ConstString string)
{
    String something;
    std::istringstream s(string);
    s >> something;
    if (something == "true")
        return true;
    else
        return false;
}

//@}

//------------------------------------------------------------------------------
// StringInputCache class
//------------------------------------------------------------------------------
//! Cache for reading strings
/*!
    Allows easier way to read a string by parts.
*/
class StringInputCache
{
    public:
            
        //! @name Constructors & Destructor
        //@{
        
        //! Constructor
        /*
            @param input string to parse

            @note Local copy of input is made.
        */
        StringInputCache(const char *input);
        //! Constructor
        /*
            @param input string to parse

            @note Local copy of input is made.
        */
        StringInputCache(ConstString input);
        
        //! Destructor
        ~StringInputCache();
        
        //@}
        
        //! @name Get & Peek
        //@{
        
        //! Return string from the cache and advance position
        /*!
            @note The delimiter is not part of returned string.
        */
        String getWord(ConstString delimiters = "\n\t /*,.[=");
        //! Return string from the cache and do NOT advance position
        /*!
            @note The delimiter is not part of returned string.
        */
        String peekWord(ConstString delimiters = "\n\t /*,.[=");
        //! Return char from the cache and advance position
        char getChar();
        //! Return char from the cache and do NOT advance position
        char peekChar();

        //@}

        //! @name Skipping
        //@{

        //! Skip given characters
        /*!
            While current character is one of the chars, advance.
            
            @param chars characters to skip
            
            @note Defaults to whitespace
        */
        void skipChars(ConstString chars = "\n\t ");
        //! Skip anything but given characters
        /*!
            While current character is NOT one of the chars, advance.
            
            @param chars characters to stop on
        */
        void skipCharsUntil(ConstString chars);

        //! Skip whitespace and comments
        /*!
            Advance till non-whitespace and non-comment is encountered.
            
            @note
            Two types of comments are recognized, C and C++.
            The C comments cannot be nested.
        */
        void skipWhitespace();

        //@}

        //! @name Seeking
        //@{

        //! Is end of string reached ?
        bool isEos(); // end of string ;-)
        
        //! Get current position in cache
        int getPosition();
        
        //! Seek to given position
        void seek(int position);
        
        //! Advance number of characters
        /*!
            Move number characters forward.
            
            @param number number of characters to move forvard (defaults to 1)
            
            @note
                Negative number is allowed for backward movement;            
        */
        void advance(int number = 1);
        
        //@}
        
        // returns rest of the string (make your own copy !)
        //! return C-like string
        /*!
            @note
            Inner copy is returned, do not modify !
        */
        const char *c_str();
                
    protected:
        char *data;
        int length;
        int index;
};

//------------------------------------------------------------------------------
// StringOutputCache class
//------------------------------------------------------------------------------
//! Cache for writing strings
/*!
    Allows easier way to write strings.
*/
class StringOutputCache
{
    public:
        //! @name Constructors & Destructor
        //@{
        
        //! Constructor
        StringOutputCache();
        //! Destructor
        ~StringOutputCache();

        //@{
        
        //! @name Indentation
        //@{
        
        //! Indent one level more
        void indent()
        {
            indentLevel++;
        };
        //! Indent one level less
        void unindent()
        {
            if (indentLevel > 0)
                indentLevel--;
        };
        //@}
        
        //! Add given string
        /*!
            @param str string to add
            
            @note
            After each newline character, a number of spaces is added
            (depending on current indentation)
        */
        void add(ConstString str);

        //! Get resulting string
        inline String getString()
        {
            return data;
        }
    protected:
        String data;
        int indentLevel;
};

#endif // DUNE_STRINGS_H
