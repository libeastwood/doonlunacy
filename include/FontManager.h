#ifndef DUNE_FONTMANAGER_H
#define DUNE_FONTMANAGER_H

#include <map>

#include "Definitions.h"

#include <eastwood/FntFile.h>

#include "singleton.h"

class FontManager : public Singleton<FontManager>
{
    friend class Singleton<FontManager>;

    protected:
        FontManager();
        ~FontManager();

    public:
	eastwood::FntFile* getFont(std::string fn);

    private:
	std::map<std::string, eastwood::FntFile*> m_fonts;

	eastwood::FntFile* loadFont(std::string fn);
};

#endif // DUNE_FONTMANAGER_H
