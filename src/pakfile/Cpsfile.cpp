#include "pakfile/Cpsfile.h"
#include <SDL_endian.h>
#include "DataCache.h"
#include <stdlib.h>
#include <string.h>

#define	SIZE_X	320
#define SIZE_Y	240

Cpsfile::Cpsfile(unsigned char * bufFiledata, int bufsize, SDL_Palette* palette) : Decode()
{
	Filedata = bufFiledata;
	CpsFilesize = bufsize;
	if (palette == NULL)
		m_palette = DataCache::Instance()->getPalette(IBM_PAL);
	else
		m_palette = palette;
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
	
	
	SDL_SetColors(pic, m_palette->colors, 0, m_palette->ncolors);
	
	SDL_LockSurface(pic);	

	//Now we can copy line by line
	for(int y = 0; y < SIZE_Y;y++) {
		memcpy(	((char*) (pic->pixels)) + y * pic->pitch , ImageOut + y * SIZE_X, SIZE_X);
	}
		
	SDL_UnlockSurface(pic);

	free(ImageOut);

	Image * img = new Image(pic);
	
	return img;
}

Image * Cpsfile::getSubPicture(unsigned int left, unsigned int top, unsigned int width, unsigned int height)
{
	SDL_Surface *Pic = getPicture()->getSurface();
	if(Pic == NULL) {
		return NULL;
	}
	
	if(((int) (left+width) > Pic->w) || ((int) (top+height) > Pic->h)) {
		return NULL;
	}
	
	SDL_Surface *returnPic;
	
	// create new picture surface
	if((returnPic = SDL_CreateRGBSurface(SDL_HWSURFACE,width,height,8,0,0,0,0))== NULL) {
		fprintf(stderr,"GetSubPicture: Cannot create new Picture!\n");
		exit(EXIT_FAILURE);	
	}
			
	SDL_SetColors(returnPic, Pic->format->palette->colors, 0, Pic->format->palette->ncolors);

	SDL_Rect srcRect = {left,top,width,height};
	SDL_BlitSurface(Pic,&srcRect,returnPic,NULL); 

	Image * img = new Image(returnPic);

	return img;
}
