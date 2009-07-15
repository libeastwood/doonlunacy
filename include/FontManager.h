#ifndef DUNE_FONTMANAGER_H
#define DUNE_FONTMANAGER_H

#include <map>

#include "Definitions.h"

#include <eastwood/Font.h>

#include "singleton.h"


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

#endif // DUNE_FONTMANAGER_H
