#include "pakfile/Wsafile.h"
#include <SDL_endian.h>
#include <stdlib.h>
#include <string.h>

Wsafile::Wsafile(unsigned char * bufFiledata, int bufsize) : Decode()
{
	Filedata = bufFiledata;
	WsaFilesize = bufsize;
	
        printf("loading wsa with size %d...\n", bufsize);
        
	if(WsaFilesize < 10) {
		fprintf(stderr, "Error: No valid WSA-File: File too small!\n");
		exit(EXIT_FAILURE);
	}
	
	NumFrames = SDL_SwapLE16(*((Uint16*) Filedata) );
        printf("numframes = %d\n", NumFrames);
	SizeX = SDL_SwapLE16(*((Uint16*) (Filedata + 2)) );
	SizeY = SDL_SwapLE16(*((Uint16*) (Filedata + 4)) );
        printf("size %d x %d\n", SizeX, SizeY);
	
	if( ((unsigned short *) Filedata)[4] == 0) {
		Index = (Uint32 *) (Filedata + 10);
		FramesPer1024ms = SDL_SwapLE32( *((Uint32*) (Filedata+6)) );
	} else {
		Index = (Uint32 *) (Filedata + 8);
		FramesPer1024ms = SDL_SwapLE16( *((Uint16*) (Filedata+6)) );		
	}

        printf("FramesPer1024ms = %d\n", FramesPer1024ms);
	
	if(Index[0] == 0) {
		Index++;
		NumFrames--;
	}
	
	if(Filedata + WsaFilesize < (((unsigned char *) Index) + 4 * NumFrames)) {
		fprintf(stderr, "Error: No valid WSA-File: File too small -2-!\n");
		exit(EXIT_FAILURE);		
	}
	
	if( (decodedFrames = (unsigned char*) calloc(1,SizeX*SizeY*NumFrames)) == NULL) {
		fprintf(stderr, "Error: Unable to allocate memory for decoded WSA-Frames!\n");
		exit(EXIT_FAILURE);				
	}
	
	decodeFrames();
};

Wsafile::~Wsafile()
{
	free(decodedFrames);
};

SDL_Surface * Wsafile::getPicture(Uint32 FrameNumber, SDL_Palette *palette)
{
	if(FrameNumber >= NumFrames) {
		return NULL;
	}
	
	SDL_Surface * pic;
	unsigned char * Image = decodedFrames + (FrameNumber * SizeX * SizeY);
	
	// create new picture surface
	if((pic = SDL_CreateRGBSurface(SDL_SWSURFACE,SizeX,SizeY,8,0,0,0,0))== NULL) {
		return NULL;
	}
	
	//printf("Frame Nr.: %d (Size: %dx%d)\n",FrameNumber,SizeX,SizeY);
			
    /*
        printf("%u %u %u\n", palette->colors[12].r,
                             palette->colors[12].g,
                             palette->colors[12].b);
    */
        
	SDL_SetColors(pic, palette->colors, 0, palette->ncolors);
	SDL_LockSurface(pic);

        //printf("%u\n", Image[0]);

	//Now we can copy line by line
	for(int y = 0; y < SizeY;y++) {
		memcpy(	((unsigned char*) (pic->pixels)) + y * pic->pitch , Image + y * SizeX, SizeX);
	}
		
	SDL_UnlockSurface(pic);
	
	return pic;

};

void Wsafile::decodeFrames()
{
	unsigned char *dec80;
	
	for(int i=0;i<NumFrames;i++) {
		if( (dec80 = (unsigned char*) calloc(1,SizeX*SizeY*2)) == NULL) {
			fprintf(stderr, "Error: Unable to allocate memory for decoded WSA-Frames!\n");
			exit(EXIT_FAILURE);	
		}

		decode80(Filedata + SDL_SwapLE32(Index[i]), dec80, 0);
	
		decode40(dec80, decodedFrames + i * SizeX * SizeY);

		free(dec80);
		
		if (i < NumFrames - 1) {
			memcpy(decodedFrames + (i+1) * SizeX * SizeY, decodedFrames + i * SizeX * SizeY,SizeX * SizeY);
		}
	}
};
