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
class GameData;
class GameData;
class StringFile;
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

		GameData *getGameData(std::string name);

		void freeGameData();

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

	void loadPyObject(std::string objectName);
        
	inline python::object getPyObject(std::string objectName) {
	    if(m_pyObjects.find(objectName) == m_pyObjects.end()) {
		loadPyObject(objectName);
	    }

	    return m_pyObjects[objectName];
	}

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
		std::map<std::string, GameData*> m_gcObjs;
		std::map<std::string, GameData*> m_gameData;
		std::map<std::string, python::object> m_pyObjects;
};

#endif // DUNE_DATACACHE_H
