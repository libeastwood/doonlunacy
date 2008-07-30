#ifndef SHPFILE_H_INCLUDED
#define SHPFILE_H_INCLUDED


#include "ShpFile.h"
#include "DataCache.h"
#include <boost/shared_ptr.hpp>

class Animation;
class Shpfile;
class SDL_Palette;
typedef boost::shared_ptr<Shpfile> ShpfilePtr;

class Shpfile : public ShpFile
{
public:
	Shpfile(unsigned char * bufFiledata, int bufsize, SDL_Palette * palette = DataCache::Instance()->getPalette(IBM_PAL));
	Image * getPicture(Uint32 IndexOfFile);

	Image * getPictureArray(unsigned int tilesX, unsigned int tilesY, ...);

};


#endif //SHPFILE_H_INCLUDED
