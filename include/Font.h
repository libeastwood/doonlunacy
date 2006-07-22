#ifndef DUNE_FONT_H
#define DUNE_FONT_H

#include "singleton.h"
#include "SDL.h"
#include "SDL_ttf.h"

#define MIN_FONT_SIZE 6
#define MAX_FONT_SIZE 24
#define FONTS (MAX_FONT_SIZE - MIN_FONT_SIZE)

class FontManager : public Singleton<FontManager>
{
    friend class Singleton<FontManager>;

    protected:
        FontManager();
        ~FontManager();

    public:
        TTF_Font* GetFont(int size);

    private:
        TTF_Font* m_fonts[FONTS];
};

#endif // DUNE_FONT_H
