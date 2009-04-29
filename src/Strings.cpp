#include <stdarg.h>

#include "Strings.h"

#include "Log.h"
/*

NOTE:
LOG lines are commented out (the log would be one big mess), to debug code using
string caches, just uncomment the lines you need...
(and remebmer to comment them again before you commit)

*/

#include <string.h>

//------------------------------------------------------------------------------
// StringInputCache class
//------------------------------------------------------------------------------
StringInputCache::StringInputCache(const char *input) : data(NULL), length(0), index(0)
{
    assert(input != NULL);
    length = strlen(input) + 1;
    data = (char *) malloc(length);
    strncpy(data, input, length); // copy with NULL character from source
//    LOG(LV_INFO, "String", "StringInputCache: initialized !!! '%s' !!!", data);        
}
StringInputCache::StringInputCache(ConstString input) : data(NULL), length(0), index(0)
{
    const char *chars = input.c_str();
    length = strlen(chars) + 1;
    data = (char *) malloc(length);
    strncpy(data, chars, length); // copy with NULL character from source
//    LOG(LV_INFO, "String", "StringInputCache: initialized '%s'", data);        
}
StringInputCache::~StringInputCache()
{
    free(data);
}
        
String StringInputCache::getWord(ConstString delimiters)
{
    int wordLength; // relative to current position
    wordLength = strcspn(data + index, delimiters.c_str()); // stops on NULL
    
    index += wordLength;
//    LOG(LV_INFO, "String", "getWord: '%c' | '%c' '%c'", *(data+index-1), *(data+index), *(data+index+1));    
    return String(data + index - wordLength, wordLength);;
}

String StringInputCache::peekWord(ConstString delimiters)
{
    int wordLength; // relative to current position
    wordLength = strcspn(data + index, delimiters.c_str()); // stops on NULL
    
//    LOG(LV_INFO, "String", "peekWord: '%c' | '%c' '%c'", *(data+index-1-wordLength), *(data+index-wordLength), *(data+index+1-wordLength));    
    return String(data + index, wordLength);;
}

char StringInputCache::getChar()
{
//     LOG(LV_INFO, "String", "getChar: '%c'", *(data+index));    
     return *(data + (index++)); // TODO: remove this todo once sure that it really works ;-)
}

char StringInputCache::peekChar()
{
//     LOG(LV_INFO, "String", "peekChar: '%c'", *(data+index));    
    return *(data + index);
}

void StringInputCache::skipChars(ConstString chars)
{
    while ((*(data + index) != 0) && (chars.find((char)*(data+index), 0) != String::npos))
    {
//        LOG(LV_INFO, "String", "StringInputCache: skipping(normal = '%s') '%c'", chars.c_str(), *(data+index));    
        index++;
    }
}

void StringInputCache::skipCharsUntil(ConstString chars)
{
    while ((*(data + index) != 0) && (chars.find((char)*(data+index), 0) == String::npos))
    {
//        LOG(LV_INFO, "String", "StringInputCache: skipping(until = '%s') '%c'", chars.c_str(), *(data+index));    
        index++;
    }
}

void StringInputCache::skipWhitespace()
{
    skipChars();
    while (peekChar() == '/')
    {
        advance();
        switch (peekChar())
        {
            // C++ style comment
            case '/':
                // skip '/'
                advance();
                
                // get to the end of line
                skipCharsUntil(String("")+'\n');
                
                // skip it
                advance();
                                
                break;
            // C style comment
            case '*':
                // skip '*'
                advance();
                
                do {
                                
                    // get to the end of line
                    skipCharsUntil(String("")+'*');
                
                    // skip *
                    advance();
                    
                    // TODO: check also for EOF !!!
                    
                } while (peekChar() != '/');
                
                // skip '/'
                advance();
                                   
                break;
            // not a comment, we're done here...
            default:
                // put that char ('/') back to the cache
                advance(-1);
                return;
        };
        
        // remove whitespace that might be before next comment
        skipChars();        
    };
    
    // remove whitespace after comment
    skipChars();
}

bool StringInputCache::isEos()
{
    // NOTE: if this assert fails, then some method crossed the EOS boundary !!!
    assert(index < length);
    return (index == length-1);
}

int StringInputCache::getPosition()
{
    return index;
}
void StringInputCache::seek(int position)
{
    index = position;
    if (index < 0)
        index = 0;
    if (index >= length)
        index = length - 1;
}

void StringInputCache::advance(int number)
{
    // TODO: this probably won't work on windows while skipping '\n' character :(
    // if the file was read in binary mode !!!
    // otpetrik: need someone with Windows to check and fix somehow if possible... 
    
    seek(index + number);
}

// returns rest of the string (make your own copy !)
const char *StringInputCache::c_str()
{
    return (data + index);
}


//------------------------------------------------------------------------------
// StringOutputCache class
//------------------------------------------------------------------------------
StringOutputCache::StringOutputCache() : indentLevel(0)
{
}
StringOutputCache::~StringOutputCache()
{
}
                
void StringOutputCache::add(ConstString str)
{
    size_t index = 0; // where to start searching form
    size_t nlIndex = 0;
    
    while ((nlIndex = str.find('\n', index)) != String::npos)
    {
        data += str.substr(index, nlIndex-index);
        data += '\n';
        for (int i = 0; i < indentLevel; i++)
            data += "    "; // 4 spaces for one indent level
        // start next search after the newline
        index = nlIndex+1; 
    }
    data += str.substr(index);    
}

size_t snPrintf(char *str, size_t size, const char *format, ...) {
    size_t ret;
    va_list ap;

    va_start(ap, format);
    ret = vsnPrintf(str, size, format, ap);
    va_end(ap);

    return ret;
}

size_t vsnPrintf(char *str, size_t size, const char *format, va_list ap) {
    size_t ret = 0;

    while(*format != '\0' && (size -= ret) > 0)
    {
	size_t written = 1;
	if(*format == '%')
	{
	    const char *tmp = va_arg(ap, const char*);
	    char fmt[10] = {*(format++)};
	    for(size_t i = 1; i < sizeof(fmt); i++)
		fmt[i] = (*format != '\0' && *format != ' ' && *format != '%') ? *(format++) : '\0';

	    if(fmt[1] == 'S')
	    {
		fmt[1] = 's';
		tmp = (*((String*)tmp)).c_str();
	    }
	    // Windows is inherently insecure (no vsnprintf & snprintf)
	    // TODO: if you are using mingw, tell us whether you have snprintf & vsnprint available, thanks !
#if defined(_WIN32) || defined(WIN32) || defined(_MSC_VER)
	    written = sprintf(str, fmt, tmp);
#else
	    written = snprintf(str, size, fmt, tmp);
#endif
	}
	else
	    *(str) = *(format++);
	str += written;
	ret += written;
    }
    va_end(ap);
    *(str) = '\0';

    return ret;
}

int sScanf(ConstString str, const char *format, ...) {
    int ret;
    va_list ap;

    va_start(ap, format);
    ret = vsScanf(str, format, ap);
    va_end(ap);

    return ret;
}

int vsScanf(ConstString str, const char *format, va_list ap) {
    int ret = 0;
    const char *cstr = str.c_str();

    for (; *format != '\0'; format++)
	if(*format == '%') {
	    char delim = *(format+2);
	    char fmt[6] = {*format, *(++format), delim, '\0', '\0', '\0'};
	    void *tmp = va_arg(ap, void*);

	    if (fmt[1] == 'S') {
		String *ptr = (String*)tmp;
		while(*cstr != delim && *cstr != 0)
		    *ptr += *(cstr++);
		ret++;
	    }
	    else {
    		if(fmt[1] == 's')
    		    fmt[1] = '[', fmt[2] = '^', fmt[3] = delim, fmt[4] = ']';
		ret += sscanf(cstr, fmt, tmp);
		cstr = (const char*)index(cstr, delim);
	    }
	    cstr++;
	}
    va_end(ap);

    return ret;
}
