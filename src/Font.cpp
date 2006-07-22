#include "Font.h"
#include "Application.h"

#include <assert.h>
#include <stdlib.h>

FontManager::FontManager()
{
    fprintf(stdout, "initializing font lib...\n");
    
    if (TTF_Init() < 0)
    {
        fprintf(stderr, "ERROR: Couldn't initialise font library: %s\n", 
                        SDL_GetError());
        Application::Instance()->Die();
    }

    fprintf(stdout, "loading fonts...\n");

    const char *fn = "data/font.ttf";

    for (int i=MIN_FONT_SIZE; i < MAX_FONT_SIZE; i++)
    {
        if ( (m_fonts[i - MIN_FONT_SIZE] = TTF_OpenFont(fn, i)) = NULL )
        {
            fprintf(stderr, "ERROR: unable to load %s size %d\n", fn, i);  
            Application::Instance()->Die();
        }
        else
        {
            fprintf(stdout, "loaded font %s %d\n", fn, i);
        };
    };
};

FontManager::~FontManager()
{
    /*
    for (int i=MIN_FONT_SIZE; i < MAX_FONT_SIZE; i++)
    {
       TTF_CloseFont(m_fonts[i]);     
    };
    */

    TTF_Quit();
};

TTF_Font* FontManager::GetFont(int size)
{
    if (size < MIN_FONT_SIZE || size >= MAX_FONT_SIZE)
    {
        assert(0);
        return NULL;
    };

    return m_fonts[size - MIN_FONT_SIZE];
};
