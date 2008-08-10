#include "GCObject.h"
#include "ResMan.h"
#include "PalFile.h"
#include "DataCache.h"
#include <CpsFile.h>
#include <ShpFile.h>

using namespace libconfig;

GCObject::GCObject(std::string path)
{
	m_path = path;
	// set this to true to avoid it being freed
	m_persistent = false;
	m_freeCounter = 0;
}

GCObject::~GCObject()
{
}

bool GCObject::freeIfUnique()
{
	if(!m_persistent && m_surface.unique()){
		m_surface.reset();
		m_freeCounter++;
		if(m_freeCounter > 50)
			LOG_WARNING("GCObject:", "%s has been freed over 50 times!", m_path.c_str());
		return true;
	}
	return false;
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

        std::string variable;
		
		SDL_Palette* palette;
        if(node.lookupValue("palette", variable))
		{
			data = ResMan::Instance()->readFile(variable, &len);
			PalfilePtr tmp(new PalFile(data, len));
			palette = DataCache::Instance()->getPalette(variable);
		}
		else
			palette = DataCache::Instance()->getPalette(IBM_PAL);

        if(node.lookupValue("filename", variable)){
			// TODO: autodetection would be nice..
			std::string type;
			if(!node.lookupValue("type", type))
				type = variable.substr(variable.length()-3, 3);

			data = ResMan::Instance()->readFile(variable, &len);
			
			if (type.compare("CPS") == 0)
			{
				CpsFile *cpsfile(new CpsFile(data, len, palette));
				
				m_surface.reset(new Image(cpsfile->getSurface()));
				
				delete cpsfile;
			}

			if (type.compare("SHP") == 0)
			{
				Uint32 index;
				if(node.lookupValue("index", index))
				{
					ShpFile *shpfile(new ShpFile(data, len, palette));
					
					m_surface.reset(new Image(shpfile->getSurface(index)));
					
					delete shpfile;
				}
				else
				{
					LOG_FATAL("DataCache", "No index specified for %s!", variable.c_str());
					exit(EXIT_FAILURE);
				}
			}
		}
		else if(node.lookupValue("gcobject", variable))
		{
			ImagePtr gcObj = DataCache::Instance()->getGCObject(variable)->getImage();
			if(node.lookupValue("crop", variable))
			{
				Rect rect;
				variable >> rect;
				m_surface.reset(gcObj->getPictureCrop(rect));
			}
		}
		node.lookupValue("persistent", m_persistent);
    
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
