#include "SoundPlayer.h"

#include "DataCache.h"
#include "DuneConstants.h"
#include "Log.h"
#include "mmath.h"

#include <eastwood/VocFile.h>
#include <eastwood/AdlFile.h>

#include <SDL_mixer.h>
#include <stdlib.h>
#include <math.h>

SoundPlayer::SoundPlayer()
{

    Settings * set = Settings::Instance();
    Mix_Volume(-1, set->m_sfxVolume);

    m_musicChannel = Mix_ReserveChannels(1);
    Mix_Volume(m_musicChannel, set->m_musicVolume);

    m_player = NULL;
    m_currentSong = new song;
}

SoundPlayer::~SoundPlayer()
{
    stopMusic();
    free(m_currentSong);

    if (m_player) delete m_player;
}

// Functionality disabled for now, might add other opl emulators to library later..
void SoundPlayer::changeEmuOpl(EMUOPL oplType)
{
/*    int freq, channels;
    Uint16 format;
*/
    if (Mix_GetMusicHookData())
        stopMusic();

/*    Mix_QuerySpec(&freq, &format, &channels);

    if (m_opl)
        delete m_opl;
*/
    if (m_player)
        delete m_player;

/*    switch (oplType)
    {

        case C_EMUOPL:
            m_opl = new CEmuopl(freq, true, true);
            break;

        case CT_EMUOPL:
            m_opl = new CTemuopl(freq, true, true);
            break;

        case CK_EMUOPL:
            m_opl = new CKemuopl(freq, true, true);
            break;

        case CREAL_EMUOPL:
            m_opl = new CRealopl();
            break;

        case CANAL_EMUOPL:
            m_opl = new CAnalopl();
            break;

    }

    m_player = new AdlibPlayer(m_opl);
    */
    m_player = new AdlFile();
}

void SoundPlayer::VoiceChunkFinishedCallback(int channel)
{
// if(channel == m_voiceChannel) {
//  PlayingVoiceATM = false;
// }
}

void SoundPlayer::playSound(std::string soundID, int volume)
{
    if (Settings::Instance()->m_soundOn)
    {
        Mix_Chunk* tmp;

        if ((tmp = DataCache::Instance()->getSoundChunk(soundID)) == NULL)
        {
            return;
        }

        int channel = Mix_PlayChannel(-1, tmp, 0);

        if (channel != -1)
            Mix_Volume(channel, (volume*Settings::Instance()->m_sfxVolume) / MIX_MAX_VOLUME);
    }
}

void SoundPlayer::playMusic(MUSICTYPE musicType, uint16_t cacheTrackNum)
{
    song * songfile = DataCache::Instance()->getMusic(musicType, cacheTrackNum);
    playMusic(songfile->filename, songfile->track);
}

void SoundPlayer::playMusic(std::string filename, uint16_t trackNum)
{
    if (Settings::Instance()->m_musicOn)
    {
	m_currentSong->filename = filename;
	m_currentSong->track = trackNum;
        changeEmuOpl(Settings::Instance()->GetEmuOpl());
	int bufsize;
	uint8_t *data;
	data = ResMan::Instance()->readFile(filename, &bufsize);
        m_player->load(data, bufsize);
        m_player->rewind(trackNum);

        Mix_HookMusic(m_player->callback, m_player);
    }
}

song *SoundPlayer::getCurrentSong()
{
    if(!Mix_GetMusicHookData())
	    return NULL;
    return m_currentSong;
}

void SoundPlayer::stopMusic()
{
    Mix_HookMusic(NULL, NULL);
}

void SoundPlayer::playSound(std::string soundID)
{
    if (Settings::Instance()->m_soundOn)
    {
        Mix_Chunk* tmp;

        if ((tmp = DataCache::Instance()->getSoundChunk(soundID)) == NULL)
        {
            LOG_ERROR("SoundPlayer", "There is no sound with id %s!", soundID.c_str());
            exit(EXIT_FAILURE);
        }

        Mix_PlayChannel(-1, tmp, 0);
    }
}

void SoundPlayer::playSound(Mix_Chunk* sound, int channel)
{
    if (Settings::Instance()->m_soundOn)
    {
        Mix_PlayChannel(channel, sound, 0);
    }
}

//FIXME:Remove this eventually and reuse all useful functions

#if 0
void SoundPlayer::playVoice(int id, int house)
{
    if (soundOn)
    {
        Mix_Chunk* tmp;

        if ((tmp = pDataManager->GetVoice(id, house)) == NULL)
        {
            fprintf(stderr, "There is no voice with id %d!\n", id);
            exit(EXIT_FAILURE);
        }

        int channel = Mix_PlayChannel(-1, tmp, 0);

        Mix_Volume(channel, sfxVolume);
    }
}

void SoundPlayer::changeMusic(MUSICTYPE musicType)
{
    int musicNum = -1;
    string filename = "";

    if (currentMusicType == musicType)
    {
        return;
    }

    switch (musicType)
    {

        case MUSIC_ATTACK:

            if (currentMusicType != MUSIC_ATTACK)
            {
                if (AttackMusic.size() > 0)
                {
                    musicNum = getRandomInt(0, AttackMusic.size() - 1);
                    filename = AttackMusic[musicNum];
                }
            }

            break;

        case MUSIC_INTRO:

            if (currentMusicType != MUSIC_INTRO)
            {
                if (IntroMusic.size() > 0)
                {
                    musicNum = getRandomInt(0, IntroMusic.size() - 1);
                    filename = IntroMusic[musicNum];
                }
            }

            break;

        case MUSIC_LOSE:

            if (currentMusicType != MUSIC_LOSE)
            {
                if (LoseMusic.size() > 0)
                {
                    musicNum = getRandomInt(0, LoseMusic.size() - 1);
                    filename = LoseMusic[musicNum];
                }
            }

            break;

        case MUSIC_PEACE:

            if (currentMusicType != MUSIC_PEACE)
            {
                if (PeaceMusic.size() > 0)
                {
                    musicNum = getRandomInt(0, PeaceMusic.size() - 1);
                    filename = PeaceMusic[musicNum];
                }
            }

            break;

        case MUSIC_WIN:

            if (currentMusicType != MUSIC_WIN)
            {
                if (WinMusic.size() > 0)
                {
                    musicNum = getRandomInt(0, WinMusic.size() - 1);
                    filename = WinMusic[musicNum];
                }
            }

            break;

        case MUSIC_RANDOM:

        default:
            int maxnum = AttackMusic.size() + IntroMusic.size() + LoseMusic.size() + PeaceMusic.size() + WinMusic.size();

            if (maxnum > 0)
            {
                unsigned int randnum = getRandomInt(0, maxnum - 1);

                if (randnum < AttackMusic.size())
                {
                    musicNum = randnum;
                    filename = AttackMusic[musicNum];
                }

                else if (randnum < IntroMusic.size())
                {
                    musicNum = randnum - AttackMusic.size();
                    filename = IntroMusic[musicNum];
                }

                else if (randnum < LoseMusic.size())
                {
                    musicNum = randnum - AttackMusic.size() - IntroMusic.size();
                    filename = LoseMusic[musicNum];
                }

                else if (randnum < PeaceMusic.size())
                {
                    musicNum = randnum - AttackMusic.size() - IntroMusic.size() - LoseMusic.size();
                    filename = PeaceMusic[musicNum];
                }

                else
                {
                    musicNum = randnum - AttackMusic.size() - IntroMusic.size() - LoseMusic.size() - PeaceMusic.size();
                    filename = WinMusic[musicNum];
                }
            }

            break;
    }

    currentMusicType = musicType;

    if ((musicOn == true) && (filename != ""))
    {

        Mix_HaltMusic();

        if (music != NULL)
        {
            Mix_FreeMusic(music);
            music = NULL;
        }

        music = Mix_LoadMUS(filename.c_str());

        if (music != NULL)
        {
            printf("Now playing %s!\n", filename.c_str());
            Mix_PlayMusic(music, -1);
        }

        else
        {
            printf("Unable to play %s!\n", filename.c_str());
        }

        /*
        Mix_Chunk *sample;
        sample=Mix_LoadWAV(filename.c_str());
        Mix_PlayChannel(-1, sample, 0);*/
    }
}

void SoundPlayer::musicCheck()
{
    if (musicOn)
    {
        if ( ! Mix_PlayingMusic() )
            changeMusic(MUSIC_PEACE);
    }
}

void SoundPlayer::playSoundAt(int soundID, COORDTYPE* location)
{
    if (soundOn)
    {
        int volume = sfxVolume,
                     panning = 128,
                               xOffset = 0,
                                         yOffset = 0;

        if (location->x > dborder->maxX / BLOCKSIZE)
        {
            xOffset = location->x - dborder->maxX / BLOCKSIZE;
            panning += xOffset;
        }

        else if (location->x < dborder->minX / BLOCKSIZE)
        {
            xOffset = dborder->minX / BLOCKSIZE - location->x;
            panning -= xOffset;
        }

        if (location->y > dborder->maxY / BLOCKSIZE)
            yOffset = location->y - dborder->maxY / BLOCKSIZE;
        else if (location->y < dborder->minY / BLOCKSIZE)
            yOffset = dborder->minY / BLOCKSIZE - location->y;

        volume -= (int)(10 * (sqrt((float)(xOffset * xOffset + yOffset * yOffset))));

        if (volume < 15) volume = 15;

        playSound(soundID, volume);
    }
}

void SoundPlayer::setMusic(bool value)
{
    musicOn = value;

    if (musicOn)
        changeMusic(MUSIC_RANDOM);
    else if (music != NULL)
        Mix_HaltMusic();
}

vector<string> SoundPlayer::getMusicFileNames(string dir)
{
    vector<string> Files;
    std::list<std::string> tmp;
    std::list<std::string>::const_iterator iter;

    tmp = GetFileNames(dir, "mp3", true);

    for (iter = tmp.begin(); iter != tmp.end(); iter++)
    {
        Files.push_back(dir + *iter);
    }

    tmp = GetFileNames(dir, "ogg", true);

    for (iter = tmp.begin(); iter != tmp.end(); iter++)
    {
        Files.push_back(dir + *iter);
    }

    tmp = GetFileNames(dir, "wav", true);

    for (iter = tmp.begin(); iter != tmp.end(); iter++)
    {
        Files.push_back(dir + *iter);
    }

    tmp = GetFileNames(dir, "mid", true);

    for (iter = tmp.begin(); iter != tmp.end(); iter++)
    {
        Files.push_back(dir + *iter);
    }

    return Files;
}

#endif
