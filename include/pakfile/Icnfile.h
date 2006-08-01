#ifndef ICNFILE_H_INCLUDED
#define ICNFILE_H_INCLUDED

#include "SDL.h"

class Icnfile
{
public:
	Icnfile(unsigned char * bufFiledata, int bufsize);
	~Icnfile();


	SDL_Surface * getPicture(Uint32 IndexOfFile, SDL_Palette *palette);

	int getNumFiles();

private:
	unsigned char* Filedata;
	Uint32 IcnFilesize;

	Uint32 NumFiles;

	unsigned char* SSET;
	Uint32 SSET_Length;
	unsigned char* RPAL;
	Uint32 RPAL_Length;
	unsigned char* RTBL;
	Uint32 RTBL_Length;

};

#endif // ICNFILE_H_INCLUDED
