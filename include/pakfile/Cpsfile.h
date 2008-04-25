#ifndef CPSFILE_H_INCLUDED
#define CPSFILE_H_INCLUDED

#include "Gfx.h"
#include "pakfile/Decode.h"
#include "SDL.h"

#include <boost/shared_ptr.hpp>

class Cpsfile;
typedef boost::shared_ptr<Cpsfile> CpsfilePtr;

class Cpsfile : public Decode
{
public:
	Cpsfile(unsigned char * bufFiledata, int bufsize);
	~Cpsfile();


	Image * getPicture();
	Image * getDoublePicture();
	Image * getSubPicture(unsigned int left, unsigned int top, unsigned int width, unsigned int height);


private:
	unsigned char* Filedata;
	Uint32 CpsFilesize;
};

#endif // CPSFILE_H_INCLUDED
