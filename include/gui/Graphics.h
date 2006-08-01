#ifndef DUNE_GUI_GRAPHICS_H
#define DUNE_GUI_GRAPHICS_H

#include "SDL.h"

// TODO: remove once everyone uses Gfx.h instead of gui/Graphics.h
#include "Gfx.h"

//
// these were already moved to Gfx.h/cpp :
//
//void mapImageHouseColour(SDL_Surface* graphic, int house);
//void mapImageHouseColourBase(SDL_Surface* graphic, int house, int baseCol);
//void putpixel(SDL_Surface *surface, int x, int y, Uint32 colour);
//Uint32 getpixel(SDL_Surface *surface, int x, int y);
//void hlineNoLock(SDL_Surface *surface, int x1, int y, int x2, Uint32 colour);
//void drawhline(SDL_Surface *surface, int x1, int y, int x2, Uint32 colour);
//void drawrect(SDL_Surface *surface, int x1, int y1, int x2, int y2, Uint32 colour);
//SDL_Surface* copySurface(SDL_Surface* inSurface);
//SDL_Surface* scaleSurface(SDL_Surface *surf, double ratio);

// these doesn't fit there..., probably to GameState or something like that...
void catch_stdin();
void clearMessage();
void showMessage(char* newMessage);

#endif // DUNE_GUI_GRAPHICS_H
