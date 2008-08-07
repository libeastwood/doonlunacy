#include "GCObject.h"
#include "ResMan.h"
#include "PalFile.h"
#include "DataCache.h"
#include "CpsFile.h"

using namespace libconfig;

GCObject::GCObject(std::string path)
{
	m_path = path;
}

void GCObject::freeIfUnique()
{
	if(m_surface.unique())
		m_surface.reset();
}

void GCObject::drawImage()
{
	libconfig::Config * dataConfig = new Config();

    try
    {
        dataConfig->readFile("data.dunetxt");
    }
    catch(ParseException& ex)
    {
        LOG_FATAL("GCObject", "Fatal error loading configuration file on line %d: %s", 
            ex.getLine(), ex.getError());

        exit(EXIT_FAILURE);
    }


	std::string fullpath = "gcobjects.";
    fullpath += m_path;

	try
    {
        Setting& node = dataConfig->lookup(fullpath);
		
		int len;
		uint8_t *data;

        std::string fileName;
		
		SDL_Palette* palette;
        if(node.lookupValue("palette", fileName))
		{
			data = ResMan::Instance()->readFile(fileName, &len);
			PalfilePtr tmp(new PalFile(data, len));
			palette = DataCache::Instance()->getPalette(fileName);
		}
		else
			palette = DataCache::Instance()->getPalette(IBM_PAL);

        node.lookupValue("filename", fileName);
        std::string type = fileName.substr(fileName.length()-3, 3);

        data = ResMan::Instance()->readFile(fileName, &len);


        if (type.compare("CPS") == 0)
        {
            CpsFile *cpsfile(new CpsFile(data, len, palette));
        	
			m_surface.reset(new Image(cpsfile->getSurface()));

            delete cpsfile;
        }
    
    }

    catch(ParseException& ex)
    {
        LOG_FATAL("DataCache", "Setting not found %d: %s", 
            ex.getLine(), ex.getError());

        exit(EXIT_FAILURE);
    }

	delete dataConfig;
}

ImagePtr GCObject::getImage()
{
	if(!m_surface)
	{
		drawImage();
	}
	return m_surface;
}
