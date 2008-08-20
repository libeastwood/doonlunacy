#include "GCObject.h"

#include "DataCache.h"
#include "ResMan.h"

#include <eastwood/CpsFile.h>
#include <eastwood/IcnFile.h>
#include <eastwood/PalFile.h>
#include <eastwood/ShpFile.h>

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

inline std::vector<Uint32> airunit_row(Uint32 i){
	std::vector<Uint32> row(8);
	row[0] = (i+2)|TILE_NORMAL, row[1] = (i+1)|TILE_NORMAL, row[2] = i|TILE_NORMAL, row[3] = (i+1)|TILE_FLIPV,
	row[4] = (i+2)|TILE_FLIPV,  row[5] = (i+1)|TILE_ROTATE, row[6] = i|TILE_FLIPH,  row[7] = (i+1)|TILE_FLIPH;
	return row;
}

inline std::vector<Uint32> infantry_row(Uint32 i){
	std::vector<Uint32> row(4);
	row[0] = (i+3)|TILE_NORMAL, row[1] = i|TILE_NORMAL, row[2] = (i+3)|TILE_FLIPV, row[3] = (i+6)|TILE_NORMAL;
	return row;
}

inline std::vector<Uint32> groundunit_row(Uint32 i){
	std::vector<Uint32> row(8);
	row[0] = (i+2)|TILE_NORMAL, row[1] = (i+1)|TILE_NORMAL, row[2] = i|TILE_NORMAL,     row[3] = (i+1)|TILE_FLIPV,
	row[4] = (i+2)|TILE_FLIPV,  row[5] = (i+3)|TILE_FLIPV,  row[6] = (i+4)|TILE_NORMAL, row[7] = (i+3)|TILE_NORMAL;
	return row;
}

inline std::vector<Uint32> ornithopter_row(Uint32 i){
	std::vector<Uint32> row(8);
	row[0] = (i+6)|TILE_NORMAL, row[1] = (i+3)|TILE_NORMAL, row[2] = i|TILE_NORMAL, row[3] = (i+3)|TILE_FLIPV,
	row[4] = (i+6)|TILE_FLIPV,  row[5] = (i+3)|TILE_ROTATE, row[6] = i|TILE_FLIPH,  row[7] = (i+3)|TILE_FLIPH;
	return row;
}

inline std::vector<Uint32> harvestersand_row(Uint32 i){
	std::vector<Uint32> row(8);
	row[0] = (i+6)|TILE_NORMAL, row[1] = (i+3)|TILE_NORMAL, row[2] = i|TILE_NORMAL,      row[3] = (i+3)|TILE_FLIPV,
	row[4] = (i+6)|TILE_FLIPV,  row[5] = (i+9)|TILE_FLIPV,  row[6] = (i+12)|TILE_NORMAL, row[7] = (i+9)|TILE_NORMAL;
	return row;
}

inline std::vector<Uint32> rocket_row(Uint32 i){
	std::vector<Uint32> row(16);
	row[0] = (i+4)|TILE_NORMAL, row[1] = (i+3)|TILE_NORMAL, row[2] = (i+2)|TILE_NORMAL,  row[3] =(i+1)|TILE_NORMAL,
	row[4] = i|TILE_NORMAL,     row[5] = (i+1)|TILE_FLIPV,  row[6] = (i+2)|TILE_FLIPV,   row[7] =(i+3)|TILE_FLIPV,
	row[8] = (i+4)|TILE_FLIPV,  row[9] = (i+3)|TILE_ROTATE, row[10]= (i+2)|TILE_ROTATE,  row[11]= (i+1)|TILE_ROTATE,
	row[12]= i|TILE_FLIPH,      row[13]= (i+1)|TILE_FLIPH,  row[14]= (i+2)|TILE_FLIPH,   row[15]= (i+3)|TILE_FLIPH;
	return row;
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
				ShpFile *shpfile(new ShpFile(data, len, palette));
				if(node.lookupValue("index", value))
				{
					m_surface.reset(new Image(shpfile->getSurface(value)));
				}
				else if(node.exists("tiles"))
				{
					Setting& s = dataConfig->lookup(fullpath + ".tiles");
					Uint32 tilesX = 0, tilesY = 0;
					std::vector<Uint32> tiles;
					for(int i = 0; i < s.getLength(); i++)
					{
						std::vector<Uint32> tmp;
						if(s[i].isGroup())
						{
							if(s[i].exists("groundunit_row"))
									tmp = groundunit_row((int)s[i]["groundunit_row"]);
							else if(s[i].exists("airunit_row"))
									tmp = airunit_row((int)s[i]["airunit_row"]);
							else if(s[i].exists("infantry_row"))
									tmp = infantry_row((int)s[i]["infantry_row"]);
							else if(s[i].exists("ornithopter_row"))
									tmp = ornithopter_row((int)s[i]["ornithopter_row"]);
							else if(s[i].exists("harvestersand_row"))
									tmp = harvestersand_row((int)s[i]["harvestersand_row"]);
							else if(s[i].exists("rocket_row"))
									tmp = rocket_row((int)s[i]["rocket_row"]);
						}
						else if(s[i].isArray())
						{
							for(int j = 0; j < s[i].getLength(); j++)
								tmp.push_back((int)s[i][j]|TILE_NORMAL);
						}
						if(!tilesX)
							tilesX = tmp.size();
						else if(tilesX != tmp.size())
							LOG_FATAL("GCObject:", "Tile row size %d is of different size than %d for %s!",
									tmp.size(), tilesX, variable.c_str());
						tilesY++;
						for(Uint32 j = 0; j < tmp.size(); j++)
							tiles.push_back(tmp[j]);
					}
					Uint32 *tilesArray = new Uint32[tiles.size()];
					for(Uint32 i = 0; i < tiles.size(); i++)
						tilesArray[i] = tiles[i];
					m_surface.reset(new Image(shpfile->getSurfaceArray(tilesX, tilesY, tilesArray)));
				}
				else
				{
					LOG_FATAL("GCObject", "No index or tiles specified for %s!", variable.c_str());
					exit(EXIT_FAILURE);
				}
				delete shpfile;
			}
			if (type.compare("ICN") == 0)
			{
				std::string mapName;
				if(node.lookupValue("map", mapName))
				{
					int mapLen;
					uint8_t *mapData = ResMan::Instance()->readFile(mapName, &mapLen);
					
					IcnFile *icnfile(new IcnFile(data, len, mapData, mapLen, palette));
					if(node.lookupValue("index", value))
					{
						m_surface.reset(new Image(icnfile->getSurface(value)));
					}
					else if(node.exists("row"))
					{
						Setting& s = dataConfig->lookup(fullpath + ".row");
						Uint32 start = s[0], end = s[1];
						m_surface.reset(new Image(icnfile->getSurfaceRow(start, end)));
					}
					else if(node.lookupValue("mapindex", value))
					{
						int tilesX = 0, tilesY = 0, tilesN = 0;
						node.lookupValue("x", tilesX);
						node.lookupValue("y", tilesY);
						node.lookupValue("num", tilesN);
						m_surface.reset(new Image(icnfile->getSurfaceArray(value, tilesX, tilesY, tilesN)));
					}
					else
					{
						LOG_FATAL("GCObject", "index, mapindex or row specified for %s!", variable.c_str());
						exit(EXIT_FAILURE);
					}
					delete icnfile;
				}
				else
				{
					LOG_FATAL("GCObject", "No map specified for %s!", variable.c_str());
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
					m_surface->drawHLine(UPoint((int)s[i]["point"][0], (int)s[i]["point"][1]),
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
