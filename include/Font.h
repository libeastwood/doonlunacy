#ifndef DUNE_FONT_H
#define DUNE_FONT_H

#include "singleton.h"
#include "SDL.h"
#include "SDL_ttf.h"

#include <map>
#include <string>

#define MIN_FONT_SIZE 6
#define MAX_FONT_SIZE 24
#define FONTS (MAX_FONT_SIZE - MIN_FONT_SIZE)

typedef unsigned short word;
typedef unsigned char byte;

struct FNTHeader
{
    word fsize;    /* Size of the file              */
    word unknown1; /* Unknown entry (always 0x0500) */
    word unknown2; /* Unknown entry (always 0x000e) */
    word unknown3; /* Unknown entry (always 0x0014) */
    word wpos;     /* Offset of char. widths array  (abs. from beg. of file) */
    word cdata;    /* Offset of char. graphics data (abs. from beg. of file) */
    word hpos;     /* Offset of char. heights array (abs. from beg. of file) */
    word unknown4; /* Unknown entry (always 0x1012) */
    byte unknown5; // dunk- had to add this to get nchars read correctly 
    byte nchars;   /* Number of characters in font minus 1*/ // dunk- the doc says word 
    byte height;   /* Font height                   */
    byte maxw;     /* Max. character width          */
};

struct FNTCharacter
{
    byte width;
    byte height;
    byte y_offset;
    byte *bitmap;
};

class Font 
{
    public:
        Font(FNTCharacter* characters, FNTHeader* header);
        ~Font();

        void render(const char* text, SDL_Surface* surface, Uint16 x, Uint16 y, Uint8 paloff);

    private:
        FNTHeader* m_header;
        FNTCharacter* m_characters;
        word m_nchars;
};


class FontManager : public Singleton<FontManager>
{
    friend class Singleton<FontManager>;

    typedef std::map<const char*, Font*> FontList;

    protected:
        FontManager();
        ~FontManager();

    public:
        Font* getFont(const char* fn);

    private:
        FontList m_fonts;

        Font* loadFont(const char* fn);
};

class TTFFontManager : public Singleton<TTFFontManager>
{
    friend class Singleton<TTFFontManager>;

    protected:
        TTFFontManager();
        ~TTFFontManager();

    public:
        TTF_Font* GetFont(int size);

    private:
        TTF_Font* m_fonts[FONTS];
};

#endif // DUNE_FONT_H
