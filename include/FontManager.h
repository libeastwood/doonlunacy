#ifndef DUNE_FONTMANAGER_H
#define DUNE_FONTMANAGER_H

#include "singleton.h"
#include <SDL_ttf.h>
#include <Font.h>

class FontManager : public Singleton<FontManager>
{
    friend class Singleton<FontManager>;

    typedef std::map<std::string, Font*> FontList;

    protected:
        FontManager();
        ~FontManager();

    public:
        Font* getFont(std::string fn);

    private:
        FontList m_fonts;

        Font* loadFont(std::string fn);
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

#endif // DUNE_FONTMANAGER_H
