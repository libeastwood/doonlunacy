#ifndef WSAFILE_H_INCLUDED
#define WSAFILE_H_INCLUDED

#include "Gfx.h"
#include "pakfile/Decode.h"
#include "pakfile/Animation.h"
#include "SDL.h"

#include <boost/shared_ptr.hpp>

class Wsafile;

typedef boost::shared_ptr<Wsafile> WsafilePtr;

class Wsafile : public Decode
{
public:
	Wsafile(unsigned char * bufFiledata, int bufsize, 
                SDL_Surface* lastframe = NULL);
	~Wsafile();

	Image * getPicture(Uint32 FrameNumber, SDL_Palette *palette);

	Animation* getAnimation(unsigned int startindex, unsigned int endindex, SDL_Palette *palette, bool SetColorKey=true);

	inline int getNumFrames() { return (int) NumFrames; };
	inline Uint32 getFramesPer1024ms() { return FramesPer1024ms; };
    inline float getFPS() { return fps; }

private:
	void decodeFrames();

	unsigned char *decodedFrames;

	unsigned char* Filedata;
	Uint32* Index;
	int WsaFilesize;

	Uint16 NumFrames;
	Uint16 SizeX;
	Uint16 SizeY;
	Uint32 FramesPer1024ms;
    float fps;
};

#endif // WSAFILE_H_INCLUDED
