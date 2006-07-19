#ifndef DUNE_UI_H
#define DUNE_UI_H

#include "SDL.h"
#include "SDL_ttf.h"

#include "gui/VBox.h"
#include "gui/Button.h"

#define MAX_FONT_SIZE 50

extern SDL_Surface* screen;
extern SDL_Palette	*palette;
extern TTF_Font		*font[MAX_FONT_SIZE];

typedef enum
{
	CURSOR_NORMAL,
	CURSOR_RIGHT,
	CURSOR_UP,
	CURSOR_LEFT,
	CURSOR_DOWN,
	CURSOR_TARGET,
	CURSOR_SELECTION,
	NUM_CURSORS
} CURSORTYPE;

extern CURSORTYPE cursorFrame;

extern int	drawnMouseX,
		drawnMouseY;

typedef struct
{
	int	minX,
		maxX,
		minY,
		maxY,
		radarMinX,
		radarMaxX,
		radarMinY,
		radarMaxY;
} BORDERTYPE;

extern int	radarSizeX,
                radarSizeY,
                radarOffsetX,
                radarOffsetY;


extern BORDERTYPE* dborder;

extern SDL_Rect	gameBarPos,
                selectionRect;

extern Button unitIcon;

extern VBox	devastatorInterface,
                harvesterInterface,
                mcvInterface,
                palaceInterface,
                unitInterface;

#endif // DUNE_UI_H
