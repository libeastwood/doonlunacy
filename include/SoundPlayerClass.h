#ifndef SOUNDPLAYERCLASS_H_INCLUDED
#define SOUNDPLAYERCLASS_H_INCLUDED

#include <string>
#include <vector>
#include "ResMan.h"
#include "DataCache.h"
#include "dMath.h"
#include "SDL_mixer.h"
#include "singleton.h"

#include "pakfile/sound/adl/adl.h"
#include "pakfile/sound/adl/emuopl.h"

/*!
	Class that handles sounds and music.
*/
class SoundPlayerClass : public Singleton<SoundPlayerClass>
{
    friend class Singleton<SoundPlayerClass>;
public:
	//! @name Constructor & Destructor
	//@{
	SoundPlayerClass(/*SETTINGSTYPE* settings*/);
	~SoundPlayerClass();
	//@}
	
	/*!
		change type of current music
		@param musicType type of music to be played
	*/
	void changeMusic(MUSICTYPE musicType);
	
	/*!
		sets current music to MUSIC_PEACE if there's no
		other song being played
	*/
	void musicCheck();

	/*!
		plays a certain sound at certain coordinates.
		the volume of sound depends on the difference between
		location of the sound and location of the view window
		(the thing you see units in)
		@param soundID id of the sound to be played
		@param location coordinates where the sound is to be played
	*/
	void playSoundAt(Sound_enum soundID, COORDTYPE* location);
	
	/*!
		turns music playing on or off
		@param value when true the function turns music on
	*/	
	void setMusic(bool value);
	void toggleSound();

	void playVoice(Sound_enum id, HOUSETYPE house);
	void playSound(Sound_enum soundID);
	void playSound(Mix_Chunk* sound, int channel = -1);
    void playMusic(std::string filename, uint16_t trackNum);
    void stopMusic();
	inline int GetSfxVolume() { return sfxVolume; };
	void SetSfxVolume(int newVolume) {
		if(newVolume >= 0 && newVolume <= MIX_MAX_VOLUME) {
			sfxVolume = newVolume;
		}
	}
	
	inline int GetMusicVolume() { return Mix_VolumeMusic(-1); };
	void SetMusicVolume(int newVolume) {
		if(newVolume >= 0 && newVolume <= MIX_MAX_VOLUME) {
			musicVolume = newVolume;
			Mix_VolumeMusic(newVolume);
		}
	}

private:
	/*!
		the function plays a sound with a given volume
		@param soundID id of a sound to be played
		@param volume sound will be played with this volume
	*/
	void playSound(Sound_enum soundID, int volume);
	std::vector<std::string> getMusicFileNames(std::string dir);
    void MusicHook (void *userdata, Uint8 *audiobuf, int len);
	std::vector<std::string> AttackMusic;
	std::vector<std::string> IntroMusic;
	std::vector<std::string> LoseMusic;
	std::vector<std::string> PeaceMusic;
	std::vector<std::string> WinMusic;

	//! whether sound should be played
	bool	soundOn;
	//! whether music should be played
	bool	musicOn;

	//! volume of unit responses played when a unit is selected
	int	responseVolume;
	//! volume of sound effects
	int	sfxVolume;
	//! music volume
	int	musicVolume;
	//! id of currently played music
	int	thisMusicID;
	//! volume of voice over sounds
	int	voiceVolume;

	MUSICTYPE currentMusicType;
	int currentMusicNum;

	Mix_Music*      music;
	
	Copl *m_opl;
	CadlPlayer *m_player;
};
#endif //SOUNDPLAYERCLASS_H_INCLUDED
