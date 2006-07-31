
#include "pakfile/Shpfile.h"
#include <SDL_endian.h>
#include <stdlib.h>
#include <string.h>

//extern SDL_Palette* palette;

Shpfile::Shpfile(unsigned char * bufFiledata, int bufsize) : Decode()
{
	Filedata = bufFiledata;
	ShpFilesize = bufsize;
	Index = NULL;
	readIndex();
};

Shpfile::~Shpfile()
{
	if(Index != NULL) {
		free(Index);
	}
};

SDL_Surface *Shpfile::getPicture(Uint32 IndexOfFile, SDL_Palette *palette)
{
	SDL_Surface *pic = NULL;
	unsigned char *DecodeDestination = NULL;
	unsigned char *ImageOut = NULL;
	
	if(IndexOfFile >= NumFiles) {
		return NULL;
	}
	
	unsigned char * Fileheader = Filedata + Index[IndexOfFile].StartOffset;
	
	unsigned char type = Fileheader[0];
	
	unsigned char sizeY = Fileheader[2];
	unsigned char sizeX = Fileheader[3];
	
	/* size and also checksum */
	Uint16 size = SDL_SwapLE16(*((Uint16*) (Fileheader + 8)));
	
	
	printf("File Nr.: %d (Size: %dx%d)\n",IndexOfFile,sizeX,sizeY);
	/*
	printf("Type: %d\n",type);
	printf("SizeX: %d SizeY: %d\n",sizeX,sizeY);
	printf("Size: %d Filesize: %d\n",size,Index[IndexOfFile].EndOffset-Index[IndexOfFile].StartOffset);
	fflush(stdout);
	*/
	
	if((ImageOut = (unsigned char*) calloc(1,sizeX*sizeY)) == NULL) {
				return NULL;
	}
	
	switch(type) {
	
		case 0:
		{
			if( (DecodeDestination = (unsigned char*) calloc(1,size)) == NULL) {
				free(ImageOut);
				return NULL;
			}
			
			if(decode80(Fileheader + 10,DecodeDestination,size) == -1) {
				fprintf(stderr,"Warning: Checksum-Error in Shp-File\n");
			}
						
			shp_correct_lf(DecodeDestination,ImageOut, size);
			
			free(DecodeDestination);
		} break;
		
		case 1:
		{
			if( (DecodeDestination = (unsigned char*) calloc(1,size)) == NULL) {
				free(ImageOut);
				return NULL;
			}
			
			if(decode80(Fileheader + 10 + 16,DecodeDestination,size) == -1) {
				fprintf(stderr,"Warning: Checksum-Error in Shp-File\n");
			}
			
			shp_correct_lf(DecodeDestination, ImageOut, size);
			
			apply_pal_offsets(Fileheader + 10,ImageOut,sizeX*sizeY);
			
			free(DecodeDestination);			
		} break;
		
		case 2:
		{
			shp_correct_lf(Fileheader+10, ImageOut,size);
		} break;
		
		case 3:
		{
		
			shp_correct_lf(Fileheader + 10 + 16, ImageOut,size);
		
			apply_pal_offsets(Fileheader + 10,ImageOut,sizeX*sizeY);
		} break;
		
		default:
		{
			fprintf(stderr,"Error: Type %d in SHP-Files not supported!\n",type);
			exit(EXIT_FAILURE);
		}
	}
	
	// create new picture surface
	if((pic = SDL_CreateRGBSurface(SDL_SWSURFACE,sizeX,sizeY,8,0,0,0,0))== NULL) {
		return NULL;
	}
			
	SDL_SetColors(pic, palette->colors, 0, palette->ncolors);
	SDL_LockSurface(pic);	

	//Now we can copy line by line
	for(int y = 0; y < sizeY;y++) {
		memcpy(	((char*) (pic->pixels)) + y * pic->pitch , ImageOut + y * sizeX, sizeX);
	}
		
	SDL_UnlockSurface(pic);

        SDL_SetColorKey(pic, SDL_SRCCOLORKEY, 0);
	
	if(ImageOut != NULL) {
		free(ImageOut);
	}
	
	return pic;
};

void Shpfile::readIndex()
{
	// First get number of files in shp-file
	NumFiles = SDL_SwapLE16( ((Uint16*) Filedata)[0]);
	
	if(NumFiles == 0) {
		fprintf(stderr, "Error: There is no file in this shp-File!\n");
		exit(EXIT_FAILURE);
	}
	
	if(NumFiles == 1) {
		/* files with only one image might be different */
		
		// create array with one entry
		if((Index = (ShpfileEntry*) malloc(sizeof(ShpfileEntry) * 1)) == NULL) {
			perror("Shpfile::readIndex");
			exit(EXIT_FAILURE);
		}
		
		if (((Uint16*) Filedata)[2] != 0) {
			/* File has special header with only 2 byte offset */
			
			Index[0].StartOffset = ((Uint32) SDL_SwapLE16(((Uint16*) Filedata)[1]));
			Index[0].EndOffset = ((Uint32) SDL_SwapLE16(((Uint16*) Filedata)[2])) - 1;


		} else {
			/* File has normal 4 byte offsets */
			Index[0].StartOffset = ((Uint32) SDL_SwapLE32(*((Uint32*) (Filedata+2)))) + 2;
			Index[0].EndOffset = ((Uint32) SDL_SwapLE16(((Uint16*) Filedata)[3])) - 1 + 2;
		}

	} else {
		/* File contains more than one image */
	
		if( ShpFilesize < (Uint32) ((NumFiles * 4) + 2 + 2)) {
			fprintf(stderr, "Error:Shp-File-Header is not complete! Header should be %d bytes big, but Shp-File is only %d bytes long.\n",(NumFiles * 4) + 2 + 2,ShpFilesize);
			exit(EXIT_FAILURE);				
		}
		
		// create array
		if((Index = (ShpfileEntry*) malloc(sizeof(ShpfileEntry) * NumFiles)) == NULL) {
			perror("Shpfile::readIndex");
			exit(EXIT_FAILURE);
		}
		
		// now fill Index with start and end-offsets
		for(int i = 0; i < NumFiles; i++) {
			Index[i].StartOffset = SDL_SwapLE32( ((Uint32*)(Filedata+2))[i]) + 2;
			
			if(i > 0) {
				Index[i-1].EndOffset = Index[i].StartOffset - 1;
				
				if(Index[i-1].EndOffset > ShpFilesize) {
					fprintf(stderr, "Error:The File with Index %d, goes until byte %d, but this SHP-File is only %d bytes big.\n",i,Index[i-1].EndOffset,ShpFilesize);
					exit(EXIT_FAILURE);						
				}
			}
		}
		
		// Add the EndOffset for the last file
		Index[NumFiles-1].EndOffset = ((Uint32) SDL_SwapLE16( *((Uint16*) (Filedata + 2 + (NumFiles * 4))))) - 1 + 2;
	}
};
