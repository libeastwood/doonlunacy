#include <SDL.h>

#include "Gfx.h"
#include "pakfile/Wsafile.h"


Wsafile::Wsafile(uint8_t * bufFiledata, int bufsize, 
                SDL_Surface* lastframe, float setFps ) : WsaFile(bufFiledata, bufsize, lastframe, setFps)
{
}

Wsafile::Wsafile() : WsaFile()
{
}

Image * Wsafile::getPicture(Uint32 FrameNumber, SDL_Palette *palette)
{
	Image *img = new Image(getSurface(FrameNumber, palette));
	return img;
}
