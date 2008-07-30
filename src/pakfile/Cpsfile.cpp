#include "pakfile/Cpsfile.h"

Cpsfile::Cpsfile(unsigned char * bufFiledata, int bufsize, SDL_Palette* palette) : CpsFile(bufFiledata, bufsize, palette)
{
}

Image *Cpsfile::getPicture()
{
	Image *img = new Image(getSurface());
	return img;
}
