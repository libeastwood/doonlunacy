#include "pakfile/Shpfile.h"
#include "Log.h"
#include "DataCache.h"

Shpfile::Shpfile(unsigned char * bufFiledata, int bufsize, SDL_Palette * palette) : ShpFile(bufFiledata, bufsize, palette)
{
}

Image *Shpfile::getPicture(Uint32 IndexOfFile)
{
	Image *img = new Image(getSurface(IndexOfFile));
	return img;
}

Image *Shpfile::getPictureArray(unsigned int tilesX, unsigned int tilesY, ...) {
	Uint32 *tiles;
	if((tilesX == 0) || (tilesY == 0)) {
		return NULL;
	}
	
	if((tiles = (Uint32*) malloc(tilesX*tilesY*sizeof(Uint32))) == NULL) {
		LOG_ERROR("ShpFile","ShpFile::getSurfaceArray(): Cannot allocate memory!");
		exit(EXIT_FAILURE);
	}

	va_list arg_ptr;
	va_start(arg_ptr, tilesY);
	
	for(Uint32 i = 0; i < tilesX*tilesY; i++) {
		tiles[i] = va_arg( arg_ptr, int );
		if(TILE_GETINDEX(tiles[i]) >= NumFiles) {
		free(tiles);
			LOG_ERROR("ShpFile","ShpFile::getSurfaceArray(): There exist only %d files in this *.shp.",NumFiles);
			return NULL;
		}
	}
	
	va_end(arg_ptr);

	Image *img = new Image(getSurfaceArray(tilesX, tilesY, tiles));
	return img;
}
