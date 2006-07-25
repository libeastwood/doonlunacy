#include "pakfile/Palette.h"

Palettefile::Palettefile(unsigned char* buf, int bufsize)
{
    m_palette = new SDL_Palette;
    m_palette->ncolors = bufsize / 3;

    m_palette->colors = new SDL_Color[m_palette->ncolors];
    SDL_Color* c = m_palette->colors;
    unsigned char* dunepal = buf;
    
    for (int i=0; i!=m_palette->ncolors; i++, c++, dunepal+=3)
    {
        c->r = dunepal[0];
        c->g = dunepal[1];
        c->b = dunepal[2];
    };

    delete [] buf;
};

Palettefile::~Palettefile()
{
    delete m_palette;
};
