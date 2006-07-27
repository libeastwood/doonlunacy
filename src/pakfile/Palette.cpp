#include "pakfile/Palette.h"

Palettefile::Palettefile(unsigned char* buf, int bufsize)
{
    printf("loading palette...\n");
    
    m_palette = new SDL_Palette;
    m_palette->ncolors = bufsize / 3;

    m_palette->colors = new SDL_Color[m_palette->ncolors];
    SDL_Color* c = m_palette->colors;
    unsigned char* dunepal = buf;
    
    for (int i=0; i!=m_palette->ncolors; i++, c++, dunepal+=3)
    {
        c->r = (dunepal[0] << 2) + 3;
        c->g = (dunepal[1] << 2) + 3;
        c->b = (dunepal[2] << 2) + 3;

        printf("%d %03d %03d %03d\n", i, c->r, c->g, c->b);
    };

    printf("%u %u %u\n", m_palette->colors[12].r,
                         m_palette->colors[12].g,
                         m_palette->colors[12].b);

    delete [] buf;
};

Palettefile::~Palettefile()
{
    delete m_palette;
};
