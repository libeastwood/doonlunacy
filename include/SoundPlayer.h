#ifndef SOUNDPLAYER_H_INCLUDED
#define SOUNDPLAYER_H_INCLUDED

#include "SDL_mixer.h"
#include <string>
#include <vector>

#include "dMath.h"
#include "DataCache.h"
#include "houses.h"
#include "ResMan.h"
#include "singleton.h"
#include "Settings.h"

#include "pakfile/sound/adl/adl.h"
#include "pakfile/sound/adl/emuopl.h"

/*!
	Class that handles sounds and music.
*/


class SoundPlayer : public Singleton<SoundPlayer>
{
    friend class Singleton<SoundPlayer>;
  
  public:  
    //! @name Constructor & Destructor
	//@{    
    SoundPlayer();
    ~SoundPlayer();
    //@}
  
	void playVoice(Sound_enum soundID, HOUSETYPE house);

	void playSound(Sound_enum soundID);

	void playSound(Mix_Chunk* sound, int channel = 0);
    
    /*! change type of current music
     *
     * @param value if true music will start playing
     * @param musicType type of music to be played
     * 
     * if value is set to true and musicType is not given
     * random music will be played
     * 
     * if value is set to false music will stop
     */
    void playMusic(std::string filename, uint16_t trackNum);
    
    void stopMusic();
    
    void VoiceChunkFinishedCallback(int channel);
    
  private:
	/*! used to play/pause playing of music
	 *  so in order to listen to music both m_musicOn && m_musicPlay have to 
	 *  be true
	 */
	bool m_musicPlay;
	
    //! pointer to music chunk to be played
	Mix_Chunk * m_music;
	
	/*!
		the function plays a sound with a given volume
		@param soundID id of a sound to be played
		@param volume sound will be played with this volume
	*/
	void playSound(Sound_enum soundID, int volume);
	
	
	int m_musicChannel;

	//! type of currently played music
	MUSICTYPE m_currentMusicType;

	Copl *m_opl;
	CadlPlayer *m_player;
};
#endif //SOUNDPLAYER_H_INCLUDED
