#include "Font.h"
#include "Application.h"
#include "ResMan.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

Font::Font(FNTCharacter* characters, FNTHeader* header)
{
    m_characters = characters;
    m_header = header;
};

Font::~Font()
{
    delete [] m_characters;
    delete m_header;
};

void Font::render(const char* text, SDL_Surface* surface, Uint16 offx, Uint16 offy, Uint8 paloff)
{
    FNTCharacter* ch;
    byte* bitmap;
    Uint8* pixels = (Uint8*)surface->pixels;

    for (unsigned int c=0; c!=strlen(text); c++)
    {
        ch = &m_characters[text[c]];
        bitmap = ch->bitmap;

/*        byte ox;
        

        for (byte y=0; y!=ch->y_offset; y++)
        {
            ox = offx;
            for (byte x=0; x!=ch->width; x++)
            {
                pixels[(ox) + ((y + offy) * surface->w)] = 0;
                ++ox;
                pixels[(ox) + ((y + offy) * surface->w)] = 0;
                ++ox;
            };
        };
	*/
        
        for (byte y=0; y!=ch->height; y++)
        {
            for (byte x=0; x!=ch->width*2; x+=2)
            {
                byte lobyte = bitmap[(x/2) + (y*ch->width)] >> 4;
                byte hibyte = bitmap[(x/2) + (y*ch->width)] & 0x0F;
                
                /*
                if (hibyte==0) 
                {
                    printf("  ");    
                }
                else
                {
                    printf("%2hd", hibyte);
                }
                printf(".");

                if (lobyte==0)
                {
                    printf("  ");
                }
                else
                {
                    printf("%2hd", lobyte);
                };  
                printf(".");
                */

                if (hibyte!=0)
                {
                    pixels[(offx + x) + ((ch->y_offset + y + offy) * surface->w)] = paloff + Uint8(hibyte);
                };

                if (lobyte!=0) //(2 < ch->width) lobyte!=0)
                {
                    pixels[(offx + x + 1) + ((ch->y_offset + y + offy) * surface->w)] = paloff + Uint8(lobyte);
                };
            };

        };
        offx += (2*ch->width) + 1;
    };

};

FontManager::FontManager()
{

};

FontManager::~FontManager()
{

};

Font* FontManager::getFont(const char* fn)
{
    FontList::iterator it = m_fonts.find(fn);
    if (it == m_fonts.end())
    {
        printf("loading %s\n", fn);
        m_fonts[fn] = loadFont(fn);
    };

    return m_fonts[fn];
};

Font* FontManager::loadFont(const char* fn)
{
    printf("loadFont  %s\n", fn);
    //FILE* file = fopen(fn, "rb");
    FileLike* file = ResMan::Instance()->readFile(fn);

    printf("sizeof word %u, sizeof byte %u\n", sizeof(word), sizeof(byte));

    FNTHeader* header = new FNTHeader();
    
    file->read(header, sizeof(FNTHeader));

    printf("fsize %d\n", header->fsize);

    // this checks if its a valid font
    if (header->unknown1 != 0x0500) printf("failed unknown1\n");
    if (header->unknown2 != 0x000e) printf("failed unknown2\n");
    if (header->unknown3 != 0x0014) printf("failed unknown3\n");

    printf("nchars %u\n", header->nchars);

    word* dchar = new word[header->nchars+1];

    file->read(dchar, sizeof(word) * (header->nchars+1));

    byte* wchar = new byte[header->nchars+1];

    printf("wpos %d\n", header->wpos);
    file->seek(header->wpos);
    file->read(wchar, sizeof(byte) * (header->nchars+1));

    if (wchar[0] != 8) printf("bad!!\n");

    word* hchar = new word[header->nchars+1];

    file->seek(header->hpos);
    file->read(hchar, sizeof(word) * (header->nchars+1));

    file->seek(header->cdata);

    FNTCharacter* characters = new FNTCharacter[header->nchars+1];    
    
    for (int i=0; i!=header->nchars+1; i++)
    {
        byte offset = hchar[i] & 0xFF;
        byte height = hchar[i] >> 8;
        byte width =( wchar[i] + 1) / 2;
        printf("%d width = %hd offset = %hd height = %hd\n", i, width, offset, height);
        
        characters[i].width = width;
        characters[i].height = height;
        characters[i].y_offset = offset;
        printf("size %hd\n", width * height);
        printf("dchar %hd\n", dchar[i]);

        file->seek(dchar[i]); 
        byte* bitmap = new byte[width * height];
        file->read(bitmap, sizeof(byte) * (width * height));
        characters[i].bitmap = bitmap;       
    };

    delete file;
    
    Font* font = new Font(characters, header);

    return font;
 
};

// ------------------------------------------------------------------

TTFFontManager::TTFFontManager()
{
    fprintf(stdout, "initializing font lib...\n");
    
    if (TTF_Init() < 0)
    {
        fprintf(stderr, "ERROR: Couldn't initialise font library: %s\n", 
                        SDL_GetError());
        Application::Instance()->Die();
    }

    fprintf(stdout, "loading fonts...\n");

    const char *fn = "data/font.ttf";

    for (int i=MIN_FONT_SIZE; i < MAX_FONT_SIZE; i++)
    {
        if ( (m_fonts[i - MIN_FONT_SIZE] = TTF_OpenFont(fn, i)) = NULL )
        {
            fprintf(stderr, "ERROR: unable to load %s size %d\n", fn, i);  
            Application::Instance()->Die();
        }
        else
        {
            fprintf(stdout, "loaded font %s %d\n", fn, i);
        };
    };
};

TTFFontManager::~TTFFontManager()
{
    /*
    for (int i=MIN_FONT_SIZE; i < MAX_FONT_SIZE; i++)
    {
       TTF_CloseFont(m_fonts[i]);     
    };
    */

    TTF_Quit();
};

TTF_Font* TTFFontManager::GetFont(int size)
{
    if (size < MIN_FONT_SIZE || size >= MAX_FONT_SIZE)
    {
        assert(0);
        return NULL;
    };

    return m_fonts[size - MIN_FONT_SIZE];
};
