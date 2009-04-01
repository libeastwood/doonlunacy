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

        //! @name Fetch methods	
        //@{
        
        /*!
         *  Used to fetch Palette from cache
         *  @param palette name of Palette to be fetched
         *  @return pointer to SDL_Palette
         *  @note Currently we have several palettes:
         *        <ul>
         *        <li>DUNE:BENE.PAL	- Used for Bene Gesserit mentat graphics</li>
         *        <li>DUNE:IBM.PAL	- Used for sprites</li>
         *        <li>DUNE:INTRO_PAL	- Used in Intro sequence</li>
         *        <li>DUNE:WESTWOOD_PAL	- Used for Westwood studios sequence before intro</li>
         *         </ul>
         */

        SDL_Palette* getPalette(std::string paletteFile);

        void loadPyObjects();
        
        template<typename T>
        inline const T getPyObjectAttribute(std::string objectName, std::string parameter, int index = -1)
        {
	    python::object obj = m_pyObjects[objectName].attr(parameter.c_str());
	    if(index < 0)
		return python::extract<T>(obj);
	    else
		return python::extract<T>(obj[index]);
        }

        template<typename T>
        inline std::vector<T> getPyObjectVector(std::string objectName, std::string parameter)
        {
	    python::object obj = m_pyObjects[objectName].attr(parameter.c_str());

	    ssize_t size = python::len(obj);
	    std::vector<T> ret(size);
	    for(ssize_t i = 0; i < size; i++)
		ret[i] = python::extract<T>(obj[i]);
	    return ret;
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
		
		std::map<std::string, PalfilePtr> m_palette;

		StringFile* BriefingStrings[3];
		StringFile* IntroStrings;
		StringFile* CreditsStrings;
		std::vector<Mix_Chunk*> soundChunk;
		std::map<std::string, GCObject*> m_gcObjs;
		std::map<std::string, python::object> m_pyObjects;
};

#endif // DUNE_DATACACHE_H
