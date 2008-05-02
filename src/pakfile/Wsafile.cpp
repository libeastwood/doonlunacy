#include "Gfx.h"
#include "pakfile/Wsafile.h"
#include "pakfile/Cpsfile.h"
#include <SDL_endian.h>
#include <stdlib.h>
#include <string>
#include "Font.h"

Wsafile::Wsafile(uint8_t * bufFiledata, int bufsize, 
                SDL_Surface* lastframe, float setFps ) : Decode()
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
	} 
	else 
	{
		Index = (Uint32 *) (Filedata + 8);
		FramesPer1024ms = SDL_SwapLE16( *((Uint16*) (Filedata+6)) );
	}

    // surely /1000.0f not 100?!
	if(setFps)
		fps = setFps;
	else
		fps = (FramesPer1024ms / 1024.0f) / 100.0f;

    printf("FramesPer1024ms = %d\n", FramesPer1024ms);
    printf("FPS = %.3f\n", fps);
	
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

    if (lastframe != NULL)
    {
        memcpy(decodedFrames, lastframe->pixels, SizeX*SizeY);
    }
	
	decodeFrames();
}

Wsafile::Wsafile(std::string text, uint8_t textColor) : Decode()
{
	WsaFilesize = -1;

    printf("loading fake wsa...\n");
	
	NumFrames = 1;
	fps = 0.1;

    printf("FramesPer1024ms = %d\n", FramesPer1024ms);
    printf("FPS = %.3f\n", fps);
	decodedFrames = NULL;
	m_text = text;
	m_textColor = textColor;
}

Wsafile::Wsafile(Cpsfile* cpsFile) : Decode()
{
	WsaFilesize = -2;

    printf("loading cps file as wsa...\n");
	
	NumFrames = 1;
	fps = 0.1;

    printf("FramesPer1024ms = %d\n", FramesPer1024ms);
    printf("FPS = %.3f\n", fps);
	decodedFrames = NULL;
	m_cpsFile = cpsFile;
}

Wsafile::~Wsafile()
{
	free(decodedFrames);
}

Image * Wsafile::getPicture(Uint32 FrameNumber, SDL_Palette *palette)
{
	if(WsaFilesize == -1){
		Image* img = new Image(UPoint(320,240));
		
		std::string text = m_text;
		Font* font = FontManager::Instance()->getFont("INTRO:INTRO.FNT");
		
		Uint16 textw, texth;
		
		img->fillRect(0);
		
		uint8_t numLines = 0;
		int linebreak = text.find("\n",0)+ 1;
		std::string thisLine;
		while(text.substr(0, linebreak-1).length() > 0){
			thisLine = text.substr(0, linebreak-1);
			
			font->extents(thisLine, textw, texth);
			
			font->render(thisLine, img->getSurface(),
					img->getSurface()->w/2 - textw/2, 
					img->getSurface()->h/2+(numLines++*20) - texth/2, m_textColor);
			if(linebreak == -1)
				break;
			text = text.substr(linebreak, text.length()-linebreak);
			linebreak = text.find("\n",0);
		}
		return img;
	}

	if(WsaFilesize == -2){
		return m_cpsFile->getPicture();
	}

	if(FrameNumber >= NumFrames) {
		return NULL;
	}
	
	SDL_Surface * pic;
	uint8_t * Frame = decodedFrames + (FrameNumber * SizeX * SizeY);
	
	// create new picture surface
	if((pic = SDL_CreateRGBSurface(SDL_SWSURFACE,SizeX,SizeY,8,0,0,0,0))== NULL) 
	{
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
	for(int y = 0; y < SizeY;y++) 
	{
		memcpy(	((uint8_t*) (pic->pixels)) + y * pic->pitch , Frame + y * SizeX, SizeX);
	}
		
	SDL_UnlockSurface(pic);
	
	Image * img = new Image(pic);

	return img;

}

/// Returns an animation
/**
	This method returns a new animation object with all pictures from startindex to endindex
	in it. The returned pointer should be freed with delete if no longer needed. If an error
	occured, NULL is returned.
	\param	startindex	index of the first picture
	\param	endindex	index of the last picture
	\param	DoublePic	if true, the picture is scaled up by a factor of 2
	\param	SetColorKey	if true, black is set as transparency
	\return	a new animation object or NULL on error
*/
Animation* Wsafile::getAnimation(unsigned int startindex, unsigned int endindex, SDL_Palette *palette, bool SetColorKey)
{
	Animation* tmpAnimation;
	Image* tmp;
	
	if((tmpAnimation = new Animation()) == NULL) {
		return NULL;
	}
	
	for(unsigned int i = startindex; i <= endindex; i++) {
		if((tmp = getPicture(i, palette)) == NULL) {
			delete tmpAnimation;
			return NULL;
		}
		tmpAnimation->addFrame(tmp,SetColorKey);
	}
	return tmpAnimation;
}

void Wsafile::decodeFrames()
{
	unsigned char *dec80;
	
	for(int i=0;i<NumFrames;i++) 
	{
		if( (dec80 = (unsigned char*) calloc(1,SizeX*SizeY*2)) == NULL) 
		{
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
}
