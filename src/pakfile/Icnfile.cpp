#include "pakfile/Icnfile.h"

#include <SDL_endian.h>
#include <stdlib.h>
#include <string.h>

#define	SIZE_X	16
#define SIZE_Y	16

//extern SDL_Palette* palette;
    

Icnfile::Icnfile(unsigned char * bufFiledata, int bufsize)
{
	Filedata = bufFiledata;
	IcnFilesize = bufsize;
	

	// check if we can access first section;
	if(IcnFilesize < 0x20) {
		fprintf(stderr, "ERROR: Invalid ICN-File: No SSET-Section found!\n");
		exit(EXIT_FAILURE);
	}
	
	
	SSET = Filedata+0x18;
	
	// check SSET-Section
	if(		(SSET[0] != 'S')
		||	(SSET[1] != 'S')
		||	(SSET[2] != 'E')
		||	(SSET[3] != 'T')) {
		fprintf(stderr, "ERROR: Invalid ICN-File: No SSET-Section found!\n");
		exit(EXIT_FAILURE);			
	}
	
	SSET_Length = SDL_SwapBE32( *((Uint32*) (SSET + 4))) - 8;
	
	SSET += 16;
	
	if(Filedata + IcnFilesize < SSET + SSET_Length) {
		fprintf(stderr, "ERROR: Invalid ICN-File: SSET-Section is bigger than ICN-File!\n");
		exit(EXIT_FAILURE);					
	}
	
	RPAL = SSET + SSET_Length;
	
	// check RPAL-Section
	if(		(RPAL[0] != 'R')
		||	(RPAL[1] != 'P')
		||	(RPAL[2] != 'A')
		||	(RPAL[3] != 'L')) {
		fprintf(stderr, "ERROR: Invalid ICN-File: No RPAL-Section found!\n");
		exit(EXIT_FAILURE);			
	}
	
	RPAL_Length = SDL_SwapBE32( *((Uint32*) (RPAL + 4)));
	
	RPAL += 8;
	
	if(Filedata + IcnFilesize < RPAL + RPAL_Length) {
		fprintf(stderr, "ERROR: Invalid ICN-File: RPAL-Section is bigger than ICN-File!\n");
		exit(EXIT_FAILURE);					
	}
	
	RTBL = RPAL + RPAL_Length;
	
	// check RTBL-Section
	if(		(RTBL[0] != 'R')
		||	(RTBL[1] != 'T')
		||	(RTBL[2] != 'B')
		||	(RTBL[3] != 'L')) {
		fprintf(stderr, "ERROR: Invalid ICN-File: No RTBL-Section found!\n");
		exit(EXIT_FAILURE);			
	}
	

	
	RTBL_Length = SDL_SwapBE32( *((Uint32*) (RTBL + 4)));
	
	RTBL += 8;
	
	if(Filedata + IcnFilesize < RTBL + RTBL_Length) {
		fprintf(stderr, "ERROR: Invalid ICN-File: RTBL-Section is bigger than ICN-File!\n");
		exit(EXIT_FAILURE);					
	}
	
	NumFiles = SSET_Length / ((SIZE_X * SIZE_Y) / 2);
		
	if(RTBL_Length < NumFiles) {
		fprintf(stderr, "ERROR: Invalid ICN-File: RTBL-Section is too small!\n");
		exit(EXIT_FAILURE);			
	}
}

Icnfile::~Icnfile()
{	
	;
}

SDL_Surface* Icnfile::getPicture(Uint32 IndexOfFile, SDL_Palette *palette) {
	SDL_Surface * pic;
	
	if(IndexOfFile >= NumFiles) {
		return NULL;
	}
	
	// check if palette is in range
	if(RTBL[IndexOfFile] >= RPAL_Length / 16) {
		return NULL;
	}
	
	unsigned char* palettestart = RPAL + (16 * RTBL[IndexOfFile]);
		
	unsigned char * filestart = SSET + (IndexOfFile * ((SIZE_X * SIZE_Y)/2));
	
	// create new picture surface
	if((pic = SDL_CreateRGBSurface(SDL_SWSURFACE,SIZE_X,SIZE_Y,8,0,0,0,0))== NULL) {
		return NULL;
	}
			
	SDL_SetColors(pic, palette->colors, 0, palette->ncolors);
	SDL_LockSurface(pic);	

	//Now we can copy to surface
	unsigned char *dest = (unsigned char*) (pic->pixels);
	unsigned char pixel;
	for(int y = 0; y < SIZE_Y;y++) {
		for(int x = 0; x < SIZE_X; x+=2) {
			pixel = filestart[ (y*SIZE_X + x) / 2];
			pixel = pixel >> 4;
			dest[x] = palettestart[pixel];
			
			pixel = filestart[ (y*SIZE_X + x) / 2];
			pixel = pixel & 0x0F;
			dest[x+1] = palettestart[pixel];
		}
		dest += pic->pitch;
	}
		
	SDL_UnlockSurface(pic);
	
	printf("File Nr.: %d (Size: %dx%d)\n",IndexOfFile,SIZE_X,SIZE_Y);
	
	return pic;
}

int Icnfile::getNumFiles()
{
	return NumFiles;
}
