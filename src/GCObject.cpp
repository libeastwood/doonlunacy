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
			LOG_WARNING("GCObject", "%s has been freed over 50 times!", m_path.c_str());
		return true;
	}
	return false;
}

void GCObject::drawImage()
{
	libconfig::Config *dataConfig = DataCache::Instance()->getConfig();

	std::string fullpath = "gcobjects.";
    fullpath += m_path;

	try
    {
        Setting& node = dataConfig->lookup(fullpath);
		
		int len;
		uint8_t *data;

        std::string variable;
		int value;
		
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
				if(node.lookupValue("index", value))
				{
					ShpFile *shpfile(new ShpFile(data, len, palette));
					
					m_surface.reset(new Image(shpfile->getSurface(value)));
					
					delete shpfile;
				}
				else
				{
					LOG_FATAL("GCObject", "No index specified for %s!", variable.c_str());
					exit(EXIT_FAILURE);
				}
			}
		}
		else if(node.lookupValue("gcobject", variable))
		{
			ImagePtr gcObj = DataCache::Instance()->getGCObject(variable)->getImage();
			if(node.exists("crop"))
			{
				Setting& s = dataConfig->lookup(fullpath + ".crop");
				m_surface.reset(gcObj->getPictureCrop(Rect(
								(int)s["point"][0], (int)s["point"][1],
								(int)s["size"][0], (int)s["size"][1])));
			} else
				m_surface = gcObj->getCopy();

			if(node.lookupValue("colorkey", value))
				m_surface->setColorKey(value);

			if(node.exists("putpixel"))
			{
				Setting& s = dataConfig->lookup(fullpath + ".putpixel");
				for(int i = 0; i < s.getLength(); i++)
					m_surface->putPixel(UPoint((int)s[i]["point"][0], (int)s[i]["point"][1]),
							(int)s[i]["color"]);
			}

			if(node.exists("drawvline"))
			{
				Setting& s = dataConfig->lookup(fullpath + ".drawvline");
				for(int i = 0; i < s.getLength(); i++)
					m_surface->drawVLine(UPoint((int)s[i]["point"][0], (int)s[i]["point"][1]),
								(int)s[i]["length"], (int)s[i]["color"]);
			}

			if(node.exists("drawhline"))
			{
				Setting& s = dataConfig->lookup(fullpath + ".drawhline");
				for(int i = 0; i < s.getLength(); i++)
					m_surface->drawVLine(UPoint((int)s[i]["point"][0], (int)s[i]["point"][1]),
								(int)s[i]["length"], (int)s[i]["color"]);
			}

			if(node.exists("fillrect"))
			{
				Setting& s = dataConfig->lookup(fullpath + ".fillrect");
				for(int i = 0; i < s.getLength(); i++)
					m_surface->fillRect((int)s[i]["color"], Rect(
								(int)s[i]["point"][0], (int)s[i]["point"][1],
								(int)s[i]["size"][0], (int)s[i]["size"][1]));
			}
		}
		node.lookupValue("persistent", m_persistent);
    
    }

    catch(ParseException& ex)
    {
        LOG_FATAL("GCObject", "Setting not found %d: %s", 
            ex.getLine(), ex.getError());

        exit(EXIT_FAILURE);
    }
}

ImagePtr GCObject::getImage()
{
	if(!m_surface)
	{
		drawImage();
	}
	return m_surface;
}
