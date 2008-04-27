#include <stdlib.h>
#include <math.h>

#include "pakfile/Vocfile.h"
#include "SoundPlayerClass.h"
#include "SDL_mixer.h"
#include "DuneConstants.h"
#include "mmath.h"
#include "gui/Graphics.h"
#include "DataCache.h"

#include <algorithm>


Mix_Chunk* curVoiceChunk = NULL;
int voiceChannel = 0;
bool PlayingVoiceATM = false;

void VoiceChunkFinishedCallback(int channel) {
	if(channel == voiceChannel) {
		PlayingVoiceATM = false;
	}
}

SoundPlayerClass::SoundPlayerClass() {

/*	AttackMusic = getMusicFileNames("./data/" + settings.Audio.MusicDirectory + "/attack/");
	IntroMusic = getMusicFileNames("./data/" + settings.Audio.MusicDirectory + "/intro/");
	LoseMusic = getMusicFileNames("./data/" + settings.Audio.MusicDirectory + "/lose/");
	PeaceMusic = getMusicFileNames("./data/" + settings.Audio.MusicDirectory + "/peace/");
	WinMusic = getMusicFileNames("./data/" + settings.Audio.MusicDirectory + "/win/");
*/
	sfxVolume = MIX_MAX_VOLUME/2;

	musicVolume = MIX_MAX_VOLUME/2;
	responseVolume = 100;
	voiceVolume = 128;

	Mix_Volume(-1, MIX_MAX_VOLUME);
	Mix_VolumeMusic(musicVolume);

	music = NULL;
	thisMusicID = NONE;
	currentMusicType = MUSIC_RANDOM;
	currentMusicNum = 0;

	// init global variables
	curVoiceChunk = NULL;
	PlayingVoiceATM = false;

	voiceChannel = Mix_ReserveChannels(1);	//Reserve a channel for voice over
	Mix_ChannelFinished(VoiceChunkFinishedCallback);

	soundOn = true;
	//soundOn = false;
//	musicOn = true;
	musicOn = false;
//	changeMusic(MUSIC_INTRO);
}
SoundPlayerClass::~SoundPlayerClass() {
	if(music != NULL) {
		Mix_FreeMusic(music);
		music = NULL;
	}
}

#if 0
void SoundPlayerClass::changeMusic(MUSICTYPE musicType)
{
	int musicNum = -1;
	string filename = "";
	
	if(currentMusicType == musicType) {
		return;
	}

	switch(musicType)
	{
		case MUSIC_ATTACK:
			if (currentMusicType != MUSIC_ATTACK)
			{
				if(AttackMusic.size() > 0) {
					musicNum = getRandomInt(0, AttackMusic.size()-1);
					filename = AttackMusic[musicNum];
				}
			}
			break;
		case MUSIC_INTRO:
			if (currentMusicType != MUSIC_INTRO)
			{
				if(IntroMusic.size() > 0) {
					musicNum = getRandomInt(0, IntroMusic.size()-1);
					filename = IntroMusic[musicNum];
				}
			}
			break;
		case MUSIC_LOSE:
			if (currentMusicType != MUSIC_LOSE)
			{
				if(LoseMusic.size() > 0) {
					musicNum = getRandomInt(0, LoseMusic.size()-1);
					filename = LoseMusic[musicNum];
				}
			}
			break;
		case MUSIC_PEACE:
			if (currentMusicType != MUSIC_PEACE)
			{
				if(PeaceMusic.size() > 0) {
					musicNum = getRandomInt(0, PeaceMusic.size()-1);
					filename = PeaceMusic[musicNum];
				}
			}
			break;
		case MUSIC_WIN:
			if (currentMusicType != MUSIC_WIN)
			{
				if(WinMusic.size() > 0) {
					musicNum = getRandomInt(0, WinMusic.size()-1);
					filename = WinMusic[musicNum];
				}
			}
			break;
		case MUSIC_RANDOM:
		default:
			int maxnum = AttackMusic.size() + IntroMusic.size() + LoseMusic.size() + PeaceMusic.size() + WinMusic.size();
		
			if(maxnum > 0) {
				unsigned int randnum = getRandomInt(0, maxnum-1);
			
				if(randnum < AttackMusic.size()) {
					musicNum = randnum;
					filename = AttackMusic[musicNum];
				} else if(randnum < IntroMusic.size()) {
					musicNum = randnum - AttackMusic.size();
					filename = IntroMusic[musicNum];
				} else if(randnum < LoseMusic.size()) {
					musicNum = randnum - AttackMusic.size() - IntroMusic.size();
					filename = LoseMusic[musicNum];
				} else if(randnum < PeaceMusic.size()) {
					musicNum = randnum - AttackMusic.size() - IntroMusic.size() - LoseMusic.size();
					filename = PeaceMusic[musicNum];
				} else {
					musicNum = randnum - AttackMusic.size() - IntroMusic.size() - LoseMusic.size() - PeaceMusic.size();
					filename = WinMusic[musicNum];					
				}
			}
			break;
	}
	currentMusicType = musicType;
	
	if((musicOn == true) && (filename != "")) {
		
		Mix_HaltMusic();
		
		if(music != NULL) {
			Mix_FreeMusic(music);
			music = NULL;
		}
		
		music = Mix_LoadMUS(filename.c_str());
		if(music != NULL) {
			printf("Now playing %s!\n",filename.c_str());
			Mix_PlayMusic(music, -1);
		} else {
			printf("Unable to play %s!\n",filename.c_str());			
		}
		
		/*
		Mix_Chunk *sample;
		sample=Mix_LoadWAV(filename.c_str());
		Mix_PlayChannel(-1, sample, 0);*/
	}
}
/*void SoundPlayerClass::musicCheck()
{
	if (musicOn)
	{
		if ( ! Mix_PlayingMusic() )
			changeMusic(MUSIC_PEACE);
	}
}*/
#endif
#if 0
void SoundPlayerClass::playSoundAt(int soundID, COORDTYPE* location)
{
	if (soundOn)
	{
		int	volume = sfxVolume,
			panning = 128,
			xOffset = 0,
			yOffset = 0;

		if (location->x > dborder->maxX/BLOCKSIZE)
		{
			xOffset = location->x - dborder->maxX/BLOCKSIZE;
			panning += xOffset;
		}
		else if (location->x < dborder->minX/BLOCKSIZE)
		{	
			xOffset = dborder->minX/BLOCKSIZE - location->x;
			panning -= xOffset;
		}

		if (location->y > dborder->maxY/BLOCKSIZE)
			yOffset = location->y - dborder->maxY/BLOCKSIZE;
		else if (location->y < dborder->minY/BLOCKSIZE)
			yOffset = dborder->minY/BLOCKSIZE - location->y;
		
		volume -= (int)(10*(sqrt((double)(xOffset*xOffset + yOffset*yOffset))));
		if (volume < 15) volume = 15;

		playSound(soundID, volume);
	}
}
#endif
#if 0
void SoundPlayerClass::setMusic(bool value)
{
	musicOn = value;

	if (musicOn)
		changeMusic(MUSIC_RANDOM);
	else if (music != NULL)
		Mix_HaltMusic();
}
#endif

void SoundPlayerClass::toggleSound()
{
	if (!soundOn && !musicOn)
	{
		soundOn = true;
//		currentGame->AddToNewsTicker("sound on, music off");
	}
	else if (soundOn && !musicOn)
	{
		musicOn = true;
		soundOn = false;
//		currentGame->AddToNewsTicker("sound off, music on");
		currentMusicType = MUSIC_RANDOM;
//		changeMusic(MUSIC_PEACE);
	}
	else if (!soundOn && musicOn)
	{
		soundOn = true;
//		currentGame->AddToNewsTicker("sound on, music on");
	}
	else if (soundOn && musicOn)
	{
		soundOn = false;
		musicOn = false;
//		currentGame->AddToNewsTicker("sound off, music off");

		if (music != NULL)
			Mix_HaltMusic();
	}
}

void SoundPlayerClass::playSound(Sound_enum soundID, int volume)
{
	if (soundOn)
	{
		Mix_Chunk* tmp;
		
		if((tmp = DataCache::Instance()->getSoundChunk(soundID)) == NULL) {
			return;
		}
		
		int channel = Mix_PlayChannel(-1,tmp, 0);
		if (channel != -1)
			Mix_Volume(channel, (volume*sfxVolume)/MIX_MAX_VOLUME);
	}
}

#if 0
void SoundPlayerClass::playVoice(int id, int house) {
	if (soundOn)
	{
		Mix_Chunk* tmp;
	
		if((tmp = pDataManager->GetVoice(id,house)) == NULL) {
			fprintf(stderr,"There is no voice with id %d!\n",id);
			exit(EXIT_FAILURE);
		}
	
		int channel = Mix_PlayChannel(-1, tmp, 0);
		Mix_Volume(channel,sfxVolume);
	}
}
#endif

void SoundPlayerClass::playSound(Sound_enum soundID) {
	if (soundOn)
	{
		Mix_Chunk* tmp;

		if((tmp = DataCache::Instance()->getSoundChunk(soundID)) == NULL) {
			fprintf(stderr,"There is no sound with id %d!\n",soundID);
			exit(EXIT_FAILURE);
		}
	
		Mix_PlayChannel(-1, tmp, 0);
	}
}

void SoundPlayerClass::playSound(Mix_Chunk* sound) {
	if (soundOn)
	{
		Mix_PlayChannel(-1, sound, 0);
	}
}

#if 0
vector<string> SoundPlayerClass::getMusicFileNames(string dir) {
	vector<string> Files;
	std::list<std::string> tmp;
	std::list<std::string>::const_iterator iter;
	
	tmp = GetFileNames(dir,"mp3",true);
	for(iter = tmp.begin(); iter != tmp.end(); iter++) {
		Files.push_back(dir + *iter);
	}
	
	tmp = GetFileNames(dir,"ogg",true);
	for(iter = tmp.begin(); iter != tmp.end(); iter++) {
		Files.push_back(dir + *iter);
	}
	
	tmp = GetFileNames(dir,"wav",true);
	for(iter = tmp.begin(); iter != tmp.end(); iter++) {
		Files.push_back(dir + *iter);
	}
	
	tmp = GetFileNames(dir,"mid",true);
	for(iter = tmp.begin(); iter != tmp.end(); iter++) {
		Files.push_back(dir + *iter);
	}
	
	return Files;
}
#endif
