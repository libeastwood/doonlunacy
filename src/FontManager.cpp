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
		LOG_INFO("Font", "Loading %s", fn.c_str());
		m_fonts[fn] = loadFont(fn);
	};
	return m_fonts[fn];
}

Font* FontManager::loadFont(std::string fn)
{
	LOG_INFO("Font", "LoadFont %s", fn.c_str());
	FileLike* file = ResMan::Instance()->readFile(fn);
	FNTHeader* header = new FNTHeader();
    
	file->read(header, sizeof(FNTHeader));

	// this checks if its a valid font
	if (header->unknown1 != 0x0500) LOG_WARNING("Font", "failed unknown1");
	if (header->unknown2 != 0x000e) LOG_WARNING("Font", "failed unknown2");
	if (header->unknown3 != 0x0014) LOG_WARNING("Font", "failed unknown3");

	LOG_INFO("Font", "nchars %u", header->nchars);

	word* dchar = new word[header->nchars+1];

	file->read(dchar, sizeof(word) * (header->nchars+1));

	byte* wchar = new byte[header->nchars+1];


	file->seek(header->wpos);
	file->read(wchar, sizeof(byte) * (header->nchars+1));

	if (wchar[0] != 8) LOG_WARNING("Font", "%d: bad!!", wchar[0]);

	word* hchar = new word[header->nchars+1];

	file->seek(header->hpos);
	file->read(hchar, sizeof(word) * (header->nchars+1));

	file->seek(header->cdata);

	FNTCharacter* characters = new FNTCharacter[header->nchars+1];    
    
	for (int i=0; i!=header->nchars+1; i++)
	{
		byte offset = hchar[i] & 0xFF;
		byte height = hchar[i] >> 8;
		byte width =( wchar[i] + 1)/ 2;
        
		characters[i].width = width;
		characters[i].height = height;
		characters[i].y_offset = offset;

		file->seek(dchar[i]); 
		byte* bitmap = new byte[width * height];
		file->read(bitmap, sizeof(byte) * (width * height));
		characters[i].bitmap = bitmap;       
	};

	delete file;
    
	Font* font = new Font(characters, header);

	return font;
}
