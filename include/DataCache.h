#ifndef DUNE_DATACACHE_H
#define DUNE_DATACACHE_H

#include "Definitions.h"
#include "Gfx.h"
#include "ResMan.h"
#include "singleton.h"

#include "houses.h"


#include <map>
#include <vector>
#include <libconfig.h++>

#define NUM_MAPCHOICEPIECES	28
#define NUM_MAPCHOICEARROWS	9

class Animation;
class StringFile;
class Mix_Chunk;
class PalFile;
typedef boost::shared_ptr<PalFile> PalfilePtr;

#define GROUNDUNIT_ROW(i) (i+2)|TILE_NORMAL,(i+1)|TILE_NORMAL,i|TILE_NORMAL,(i+1)|TILE_FLIPV,(i+2)|TILE_FLIPV,(i+3)|TILE_FLIPV, (i+4)|TILE_NORMAL,(i+3)|TILE_NORMAL
#define AIRUNIT_ROW(i) (i+2)|TILE_NORMAL,(i+1)|TILE_NORMAL,i|TILE_NORMAL,(i+1)|TILE_FLIPV,(i+2)|TILE_FLIPV,(i+1)|TILE_ROTATE, i|TILE_FLIPH,(i+1)|TILE_FLIPH
#define ORNITHOPTER_ROW(i) (i+6)|TILE_NORMAL,(i+3)|TILE_NORMAL,i|TILE_NORMAL,(i+3)|TILE_FLIPV,(i+6)|TILE_FLIPV,(i+3)|TILE_ROTATE, i|TILE_FLIPH,(i+3)|TILE_FLIPH
#define INFANTRY_ROW(i) (i+3)|TILE_NORMAL,i|TILE_NORMAL,(i+3)|TILE_FLIPV,(i+6)|TILE_NORMAL
#define HARVESTERSAND_ROW(i) (i+6)|TILE_NORMAL,(i+3)|TILE_NORMAL,i|TILE_NORMAL,(i+3)|TILE_FLIPV,(i+6)|TILE_FLIPV,(i+9)|TILE_FLIPV,(i+12)|TILE_NORMAL,(i+9)|TILE_NORMAL
#define ROCKET_ROW(i)	(i+4)|TILE_NORMAL,(i+3)|TILE_NORMAL,(i+2)|TILE_NORMAL,(i+1)|TILE_NORMAL,i|TILE_NORMAL,(i+1)|TILE_FLIPV,(i+2)|TILE_FLIPV,(i+3)|TILE_FLIPV, \
						(i+4)|TILE_FLIPV,(i+3)|TILE_ROTATE,(i+2)|TILE_ROTATE, (i+1)|TILE_ROTATE,i|TILE_FLIPH,(i+1)|TILE_FLIPH,(i+2)|TILE_FLIPH,(i+3)|TILE_FLIPH

typedef std::map <unsigned, ImagePtr> images;
typedef std::vector <images*> remapped_images; //One for each house
typedef	std::map<std::string, PalfilePtr> palStrings;


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
	//}@

    public:
		void Init();
        
        //! @name Add methods
        //@{

        /*! Adds sprite pictures to cache
         *  @param ID identification number a given picture is assigned
         *  @param tmp Image to be cached
         *  @param house colour to which image is remapped. By default: HOUSE_HARKONNEN
         */
        void addObjPic(ObjPic_enum ID, Image * tmp, HOUSETYPE house = HOUSE_HARKONNEN);
        /*! Adds GUI pictures to cache
         *  @param ID identification number a given picture is assigned
         *  @param tmp Image to be cached
         *  @param house colour to which image is remapped. By default: HOUSE_HARKONNEN
         */
        void addGuiPic(GuiPic_enum ID, Image * tmp, HOUSETYPE house = HOUSE_HARKONNEN);
        /*! 
         *  Parses animation parameters from a file and loads animation
         *  @return pointer to animation
         */
        Animation*		getAnimation(std::string path);
		void addSoundChunk(Sound_enum ID, Mix_Chunk* tmp); 
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
         *        <li>BENE_PAL - No idea what it is used for</li>
         *        <li>IBM_PAL - Used for sprites</li>
         *        <li>INTRO_PAL - Used in Intro sequence</li>
         *        <li>WESTWOOD_PAL - Not properly decoded yet..</li>
         *         </ul>
         */
        SDL_Palette* getPalette(Palette_enum palette);

        SDL_Palette* getPalette(std::string paletteFile);

        /*! Used to fetch shared_ptr to sprite picture
         *  @param ID identification number of a picture to be fetched
         *  @param house colour to which image is remapped. By default: HOUSE_HARKONNEN
         *  @return ImagePtr to sprite picture
         *  @note If image for a given HOUSETYPE is not available, base image (HOUSE_HARKONNEN)
         *        will be remapped, cached and a pointer to it returned
         */
        ImagePtr	getObjPic(ObjPic_enum ID, HOUSETYPE house = HOUSE_HARKONNEN);
        /*! Used to fetch shared_ptr to GUI picture
         *  @param ID identification number of a picture to be fetched
         *  @param house colour to which image is remapped. By default: HOUSE_HARKONNEN
         *  @return ImagePtr to GUI picture
         *  @note If image for a given HOUSETYPE is not available, base image (HOUSE_HARKONNEN)
         *        will be remapped, cached and a pointer to it returned
         */
        ImagePtr	getGuiPic(GuiPic_enum ID, HOUSETYPE house = HOUSE_HARKONNEN);
        /*! Used to fetch pointer to Animation
         *  @param id identification number of an animation to be fetched
         *  @return pointer to Animation
         *  @note Currently animations are resized on the fly, which is quite CPU consuming,
         *        but on the other hand it cuts down RAM consumption. TODO: Already seen 
         *        and thus already resized animations should be cached. This would allow
         *        to keep only necessary resized animations in RAM.
         */
        Mix_Chunk* getSoundChunk(Sound_enum ID);
        song * getMusic(MUSICTYPE musicType, uint16_t ID);
        Mix_Chunk* concat2Chunks(Sound_enum ID1, Sound_enum ID2);   
        std::string	getBriefingText(uint16_t mission, uint16_t textType, HOUSETYPE house);
        std::string	getIntroString(uint16_t i);
        std::string	getCreditsString(uint16_t i);
        //@}

    private:
		libconfig::Config * m_dataConfig;
		
        remapped_images m_objImg;
        remapped_images m_guiImg;
        
		PalfilePtr m_palette[NUM_PALETTES];
		palStrings m_palStrings;

		Mix_Chunk* getChunkFromFile(std::string fileName);
		Mix_Chunk* concat2Chunks(Mix_Chunk* sound1, Mix_Chunk* sound2);
		Mix_Chunk* concat3Chunks(Mix_Chunk* sound1, Mix_Chunk* sound2, Mix_Chunk* sound3);
		Mix_Chunk* createEmptyChunk();

		StringFile* BriefingStrings[3];
		StringFile* IntroStrings;
		StringFile* CreditsStrings;
		std::vector<Mix_Chunk*> soundChunk;
};

#endif // DUNE_DATACACHE_H
