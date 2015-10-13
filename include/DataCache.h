#ifndef DUNE_DATACACHE_H
#define DUNE_DATACACHE_H


#include "Definitions.h"
#include <eastwood/StringFile.h>
#include <eastwood/Palette.h>

#include "houses.h"
#include "singleton.h"

#include <map>
#include <vector>

#define NUM_MAPCHOICEPIECES	28
#define NUM_MAPCHOICEARROWS	9

class AnimationLabel;
class GameData;
class GameData;

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

	eastwood::Palette getPalette(std::string paletteFile);

	python::object loadPyObject(std::string moduleName, std::string objectName);
        
	inline python::object getPyObject(std::string moduleName, std::string objectName) {
	    if(m_pyObjects.find(moduleName) == m_pyObjects.end()) {
		m_pyObjects[moduleName] = std::map<std::string, python::object>();
	    }

	    if(m_pyObjects[moduleName].find(objectName) == m_pyObjects[moduleName].end()) {
		m_pyObjects[moduleName][objectName] = loadPyObject(moduleName, objectName);
	    }

	    return m_pyObjects[moduleName][objectName];
	}


        song * getMusic(MUSICTYPE musicType, uint16_t ID);
        std::string	getBriefingText(uint16_t mission, eastwood::MissionType textType, HOUSETYPE house);
        std::string	getIntroString(uint16_t i);
        std::string	getCreditsString(uint16_t i);
        //@}

    private:
		std::map<std::string, eastwood::Palette> m_palette;

		eastwood::StringFile* BriefingStrings[3];
		eastwood::StringFile* IntroStrings;
		eastwood::StringFile* CreditsStrings;
		std::map<std::string, GameData*> m_gameData;
		std::map<std::string, std::map<std::string, python::object> > m_pyObjects;
};

#endif // DUNE_DATACACHE_H
