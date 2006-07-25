#ifndef CPSFILE_H_INCLUDED
#define CPSFILE_H_INCLUDED

#include "pakfile/Decode.h"
#include "SDL.h"

class Cpsfile : public Decode
{
public:
	Cpsfile(unsigned char * bufFiledata, int bufsize);
	~Cpsfile();


	SDL_Surface * getPicture();

private:
	unsigned char* Filedata;
	Uint32 CpsFilesize;
};

#endif // CPSFILE_H_INCLUDED
