#ifndef DUNE_PAKFILE_PALETTE_H
#define DUNE_PAKFILE_PALETTE_H

#include "SDL.h"

class Palettefile
{
    public:
        Palettefile(unsigned char* buf, int bufsize);
        ~Palettefile();

        SDL_Palette* getPalette() { return m_palette; }

    private:
        SDL_Palette* m_palette;
    
};

#endif // DUNE_PAKFILE_PALETTE_H
