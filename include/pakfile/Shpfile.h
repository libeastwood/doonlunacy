#ifndef SHPFILE_H_INCLUDED
#define SHPFILE_H_INCLUDED

#include "SDL.h"
#include "pakfile/Decode.h"

struct ShpfileEntry
{
	Uint32 StartOffset;
	Uint32 EndOffset;
};

class Shpfile : public Decode
{
public:
	Shpfile(unsigned char * bufFiledata, int bufsize);
	~Shpfile();

	SDL_Surface* getPicture(Uint32 IndexOfFile);

	inline int getNumFiles() {return (int) NumFiles;};

private:
	void readIndex();

	ShpfileEntry * Index;
	unsigned char* Filedata;
	Uint32 ShpFilesize;
	Uint16 NumFiles;
};


#endif //SHPFILE_H_INCLUDED
