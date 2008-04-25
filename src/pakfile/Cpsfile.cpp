#include "pakfile/Cpsfile.h"
#include "Application.h"
#include <SDL_endian.h>
#include <stdlib.h>
#include <string.h>

#define	SIZE_X	320
#define SIZE_Y	240

//extern SDL_Palette* palette;

Cpsfile::Cpsfile(unsigned char * bufFiledata, int bufsize) : Decode()
{
	Filedata = bufFiledata;
	CpsFilesize = bufsize;
}

Cpsfile::~Cpsfile()
{	
	;
}

Image * Cpsfile::getPicture()
{
	unsigned char * ImageOut;
	SDL_Surface *pic = NULL;

	// check for valid file
	if( SDL_SwapLE16(*(unsigned short *)(Filedata + 2)) != 0x0004) {
		return NULL;
	}
	
	if( SDL_SwapLE16(*(unsigned short *)(Filedata + 4)) != 0xFA00) {
		return NULL;
	}
	
	Uint16 PaletteSize = SDL_SwapLE16(*((unsigned short *)(Filedata + 8)));
	
	if( (ImageOut = (unsigned char*) calloc(1,SIZE_X*SIZE_Y)) == NULL) {
		return NULL;
	}
	
	if(decode80(Filedata + 10 + PaletteSize,ImageOut,0) == -2) {
		fprintf(stderr,"Error: Cannot decode Cps-File\n");
	}
	
	// create new picture surface
	if((pic = SDL_CreateRGBSurface(SDL_SWSURFACE,SIZE_X,SIZE_Y,8,0,0,0,0))== NULL) {
		return NULL;
	}
	
	
	SDL_Palette* palette = Application::Instance()->Screen()->getSurface()->format->palette;		
	SDL_SetColors(pic, palette->colors, 0, palette->ncolors);
	
	SDL_LockSurface(pic);	

	//Now we can copy line by line
	for(int y = 0; y < SIZE_Y;y++) {
		memcpy(	((char*) (pic->pixels)) + y * pic->pitch , ImageOut + y * SIZE_X, SIZE_X);
	}
		
	SDL_UnlockSurface(pic);

	Image * img = new Image(pic);

//	free(ImageOut);
	
	return img;
}
