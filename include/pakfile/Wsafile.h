#ifndef WSAFILE_H_INCLUDED
#define WSAFILE_H_INCLUDED

#include "WsaFile.h"
#include <boost/shared_ptr.hpp>

class Wsafile;
typedef boost::shared_ptr<Wsafile> WsafilePtr;

class Wsafile : public WsaFile
{
public:
	Wsafile(uint8_t * bufFiledata, int bufsize, 
                SDL_Surface* lastframe = NULL, float setFps = 0 );
	Wsafile();

	Image * getPicture(Uint32 FrameNumber, SDL_Palette *palette);

};

#endif // WSAFILE_H_INCLUDED
