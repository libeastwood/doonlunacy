#ifndef PAKFILE_CPSFILE_H
#define PAKFILE_CPSFILE_H

#include "Gfx.h"
#include "DataCache.h"

#include <CpsFile.h>
#include <SDL.h>
#include <boost/shared_ptr.hpp>

class Cpsfile;
typedef boost::shared_ptr<Cpsfile> CpsfilePtr;

class Cpsfile : public CpsFile
{
public:
	Cpsfile(unsigned char * bufFiledata, int bufsize, SDL_Palette* palette = DataCache::Instance()->getPalette(IBM_PAL));

	Image *getPicture();

};

#endif // PAKFILE_CPSFILE_H
