#ifndef DUNE_FONTMANAGER_H
#define DUNE_FONTMANAGER_H

#include <map>

#include "Definitions.h"

#include <eastwood/Font.h>

#include "singleton.h"

class FontManager : public Singleton<FontManager>
{
    friend class Singleton<FontManager>;

    protected:
        FontManager();
        ~FontManager();

    public:
	eastwood::Font* getFont(std::string fn);

    private:
	std::map<std::string, eastwood::Font*> m_fonts;

	eastwood::Font* loadFont(std::string fn);
};

#endif // DUNE_FONTMANAGER_H
