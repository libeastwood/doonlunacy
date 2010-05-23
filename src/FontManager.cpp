#include "FontManager.h"
#include "Log.h"
#include "ResMan.h"

FontManager::FontManager()
{

}

FontManager::~FontManager()
{

}

eastwood::FntFile* FontManager::getFont(std::string fn)
{

    std::map<std::string, eastwood::FntFile*>::iterator it = m_fonts.find(fn);
	if (it == m_fonts.end())
	{
		LOG(LV_INFO, "Font", "Loading %s", fn.c_str());
		m_fonts[fn] = loadFont(fn);
	};
	return m_fonts[fn];
}

eastwood::FntFile* FontManager::loadFont(std::string fn)
{
	std::istream *file = ResMan::Instance()->getFile(fn);
	eastwood::FntFile* font = new eastwood::FntFile(*file);

	return font;
}

