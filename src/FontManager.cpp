#include "FontManager.h"
#include "Log.h"
#include "ResMan.h"

FontManager::FontManager()
{

}

FontManager::~FontManager()
{

}

Font* FontManager::getFont(std::string fn)
{

	FontList::iterator it = m_fonts.find(fn);
	if (it == m_fonts.end())
	{
		LOG(LV_INFO, "Font", "Loading %s", fn.c_str());
		m_fonts[fn] = loadFont(fn);
	};
	return m_fonts[fn];
}

Font* FontManager::loadFont(std::string fn)
{
	LOG(LV_INFO, "Font", "LoadFont %s", fn.c_str());
	FileLike* file = ResMan::Instance()->readFile(fn);
	FNTHeader* header = new FNTHeader();
    
	file->read(header, sizeof(FNTHeader));

	// this checks if its a valid font
	if (header->unknown1 != 0x0500) LOG(LV_WARNING, "Font", "failed unknown1");
	if (header->unknown2 != 0x000e) LOG(LV_WARNING, "Font", "failed unknown2");
	if (header->unknown3 != 0x0014) LOG(LV_WARNING, "Font", "failed unknown3");

	LOG(LV_INFO, "Font", "nchars %u", header->nchars);

	uint16_t* dchar = new uint16_t[header->nchars+1];

	file->read(dchar, sizeof(uint16_t) * (header->nchars+1));

	uint8_t* wchar = new uint8_t[header->nchars+1];


	file->seek(header->wpos);
	file->read(wchar, sizeof(uint8_t) * (header->nchars+1));

	if (wchar[0] != 8) LOG(LV_WARNING, "Font", "%d: bad!!", wchar[0]);

	uint16_t* hchar = new uint16_t[header->nchars+1];

	file->seek(header->hpos);
	file->read(hchar, sizeof(uint16_t) * (header->nchars+1));

	file->seek(header->cdata);

	FNTCharacter* characters = new FNTCharacter[header->nchars+1];    
    
	for (int i=0; i!=header->nchars+1; i++)
	{
		uint8_t offset = hchar[i] & 0xFF;
		uint8_t height = hchar[i] >> 8;
		uint8_t width =( wchar[i] + 1)/ 2;
        
		characters[i].width = width;
		characters[i].height = height;
		characters[i].y_offset = offset;

		file->seek(dchar[i]); 
		uint8_t* bitmap = new uint8_t[width * height];
		file->read(bitmap, sizeof(uint8_t) * (width * height));
		characters[i].bitmap = bitmap;       
	};

	delete[] dchar;
	delete[] hchar;
	delete[] wchar;
	delete file;
    
	Font* font = new Font(characters, header);

	return font;
}
