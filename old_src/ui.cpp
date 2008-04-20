#include "ui.h"

#include "SDL.h"
#include "SDL_ttf.h"
#include "gui/Button.h"

TTF_Font            *font[MAX_FONT_SIZE];
SDL_Palette         *palette;

BORDERTYPE          *dborder;

VBox    	devastatorInterface,
                harvesterInterface,
                mcvInterface,
                palaceInterface,
                unitInterface;

Button unitIcon;


