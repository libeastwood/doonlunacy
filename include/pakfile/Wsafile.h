#ifndef WSAFILE_H_INCLUDED
#define WSAFILE_H_INCLUDED

#include "pakfile/Decode.h"
#include "SDL.h"

//extern SDL_Palette* palette;

class Wsafile : public Decode
{
public:
	Wsafile(unsigned char * bufFiledata, int bufsize);
	~Wsafile();

	SDL_Surface * getPicture(Uint32 FrameNumber);

	inline int getNumFrames() { return (int) NumFrames; };
	inline Uint32 getFramesPer1024ms() { return FramesPer1024ms; };

private:
	void decodeFrames();

	unsigned char *decodedFrames;


	unsigned char* Filedata;
	Uint32* Index;
	Uint32 WsaFilesize;

	Uint16 NumFrames;
	Uint16 SizeX;
	Uint16 SizeY;
	Uint32 FramesPer1024ms;
};

#endif // WSAFILE_H_INCLUDED
