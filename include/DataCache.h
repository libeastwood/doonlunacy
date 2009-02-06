#ifndef DUNE_DATACACHE_H
#define DUNE_DATACACHE_H

#include "Definitions.h"
#include "Gfx.h"
#include "houses.h"
#include "singleton.h"

#include <boost/shared_ptr.hpp>
#include <libconfig.h++>

#include <map>
#include <vector>

#define NUM_MAPCHOICEPIECES	28
#define NUM_MAPCHOICEARROWS	9

class SDL_Palette;
class AnimationLabel;
class GCObject;
class StringFile;
class Mix_Chunk;
class PalFile;
typedef boost::shared_ptr<PalFile> PalfilePtr;

typedef	std::map<std::string, PalfilePtr> palStrings;

#include <boost/python.hpp>

namespace python = boost::python;

typedef struct {
    //! ResMan filename to song file e.g. SOUND:DUNE0.ADL
    std::string filename;
    //! It's track number in adl file. they usually have several tracks.
    //! @note Do not confuse it with track number in cache. 
    uint16_t track;
} song;

class DataCache : public Singleton<DataCache> 
{
	friend class Singleton<DataCache>;

	protected:
    //! @name Constructors & Destructor
    //@{
	DataCache();
	~DataCache();
	//@}

    public:
		void Init();
        
        //! @name Add methods
        //@{

		GCObject *getGCObject(std::string path);

		void freeGCObjects();

        /*! 
         *  Parses animation parameters from a file and loads animation
         *  @return pointer to animation
         */
        AnimationLabel *getAnimationLabel(std::string path);

        /*! Adds palette from a file 
         *  @param palette identification number Palette is assigned
         *  @param paletteFile file from which Palette is to be loaded
         */
		void addPalette(Palette_enum palette, std::string paletteFile);
		//@}

        //! @name Fetch methods	
        //@{
        
        /*!
         *  Used to fetch Palette from cache
         *  @param palette name of Palette to be fetched
         *  @return pointer to SDL_Palette
         *  @note Currently we have several palettes:
         *        <ul>
         *        <li>BENE_PAL - Used for Bene Gesserit mentat graphics</li>
         *        <li>IBM_PAL - Used for sprites</li>
         *        <li>INTRO_PAL - Used in Intro sequence</li>
         *        <li>WESTWOOD_PAL - Not properly decoded yet..</li>
         *         </ul>
         */
        SDL_Palette* getPalette(Palette_enum palette);

        SDL_Palette* getPalette(std::string paletteFile);

        void loadPyObjects();
        
        template<typename T>
        T getPyObjectAttribute(std::string objectName, std::string parameter, int index = -1)
        {
	    python::object obj = m_pyObjects[objectName].attr(parameter.c_str());
	    if(index < 0)
		return python::extract<T>(obj);
	    else
		return python::extract<T>(obj[index]);
        }

	inline std::string getPyObjectType(std::string objectName)
	{
    	    python::object obj = m_pyObjects[objectName];
    	    return python::extract<std::string>(((python::object)((python::object)((python::object)obj.attr("__class__")).attr("__mro__")[1]).attr("__name__")));
	}

        Mix_Chunk* getSoundChunk(std::string ID);
        song * getMusic(MUSICTYPE musicType, uint16_t ID);
        std::string	getBriefingText(uint16_t mission, uint16_t textType, HOUSETYPE house);
        std::string	getIntroString(uint16_t i);
        std::string	getCreditsString(uint16_t i);
		libconfig::Config *getConfig();
        //@}

    private:
		libconfig::Config *m_dataConfig;
		
		PalfilePtr m_palette[NUM_PALETTES];
		palStrings m_palStrings;

		StringFile* BriefingStrings[3];
		StringFile* IntroStrings;
		StringFile* CreditsStrings;
		std::vector<Mix_Chunk*> soundChunk;
		std::vector<GCObject*> m_gcObjs;
		std::map<std::string, python::object> m_pyObjects;
};

#endif // DUNE_DATACACHE_H
