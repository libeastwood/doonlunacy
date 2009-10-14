#include "FontManager.h"
#include "Log.h"
#include "ResMan.h"

FontManager::FontManager()
{

}

FontManager::~FontManager()
{

}

eastwood::Font* FontManager::getFont(std::string fn)
{

    std::map<std::string, eastwood::Font*>::iterator it = m_fonts.find(fn);
	if (it == m_fonts.end())
	{
		LOG(LV_INFO, "Font", "Loading %s", fn.c_str());
		m_fonts[fn] = loadFont(fn);
	};
	return m_fonts[fn];
}

eastwood::Font* FontManager::loadFont(std::string fn)
{
	LOG(LV_INFO, "Font", "LoadFont %s", fn.c_str());
	std::istream *file = ResMan::Instance()->getFile(fn);
	eastwood::FNTHeader* header = new eastwood::FNTHeader();
    
	file->read((char*)header, sizeof(eastwood::FNTHeader));

	// this checks if its a valid font
	if (header->unknown1 != 0x0500) LOG(LV_WARNING, "Font", "failed unknown1");
	if (header->unknown2 != 0x000e) LOG(LV_WARNING, "Font", "failed unknown2");
	if (header->unknown3 != 0x0014) LOG(LV_WARNING, "Font", "failed unknown3");

	LOG(LV_INFO, "Font", "nchars %u", header->nchars);

	uint16_t* dchar = new uint16_t[header->nchars+1];

	file->read((char*)dchar, sizeof(uint16_t) * (header->nchars+1));

	uint8_t* wchar = new uint8_t[header->nchars+1];


	file->seekg(header->wpos, std::ios::beg);
	file->read((char*)wchar, sizeof(uint8_t) * (header->nchars+1));

	if (wchar[0] != 8) LOG(LV_WARNING, "Font", "%d: bad!!", wchar[0]);

	uint16_t* hchar = new uint16_t[header->nchars+1];

	file->seekg(header->hpos, std::ios::beg);
	file->read((char*)hchar, sizeof(uint16_t) * (header->nchars+1));

	file->seekg(header->cdata, std::ios::beg);

	eastwood::FNTCharacter* characters = new eastwood::FNTCharacter[header->nchars+1];    
    
	for (int i=0; i!=header->nchars+1; i++)
	{
		uint8_t offset = hchar[i] & 0xFF;
		uint8_t height = hchar[i] >> 8;
		uint8_t width =( wchar[i] + 1)/ 2;
        
		characters[i].width = width;
		characters[i].height = height;
		characters[i].y_offset = offset;

		file->seekg(dchar[i], std::ios::beg);
		uint8_t* bitmap = new uint8_t[width * height];
		file->read((char*)bitmap, sizeof(uint8_t) * (width * height));
		characters[i].bitmap = bitmap;       
	};

	delete[] dchar;
	delete[] hchar;
	delete[] wchar;
	delete file;
    
	eastwood::Font* font = new eastwood::Font(characters, header);

	return font;
}

