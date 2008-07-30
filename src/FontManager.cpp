#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "Application.h"
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

// ------------------------------------------------------------------

TTFFontManager::TTFFontManager()
{
	LOG_INFO("Font", "Initializing font lib...");
    
	if (TTF_Init() < 0)
	{
		LOG_ERROR("Font", "Couldn't initialise font library: %s", 
				SDL_GetError());
	Application::Instance()->Die();
	}

	LOG_INFO("Font", "Loading fonts...");

	std::string fn = "data/font.ttf";

	for (int i=MIN_FONT_SIZE; i < MAX_FONT_SIZE; i++)
	{
		if ( (m_fonts[i - MIN_FONT_SIZE] = TTF_OpenFont(fn.c_str(), i)) = NULL )
		{
			LOG_ERROR("Font", "unable to load %s size %d", fn.c_str(), i);
			Application::Instance()->Die();
		}
		else
		{
			LOG_INFO("Font", "loaded font %s %i", fn.c_str(), i);
		};
	};
}

TTFFontManager::~TTFFontManager()
{
	/*
	for (int i=MIN_FONT_SIZE; i < MAX_FONT_SIZE; i++)
	{
	TTF_CloseFont(m_fonts[i]);     
	};
	*/
	TTF_Quit();
}

TTF_Font* TTFFontManager::GetFont(int size)
{
	if (size < MIN_FONT_SIZE || size >= MAX_FONT_SIZE)
	{
		assert(0);
		return NULL;
	};

	return m_fonts[size - MIN_FONT_SIZE];
}
