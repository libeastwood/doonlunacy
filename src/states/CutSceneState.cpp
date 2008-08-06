#include <iostream>
#include <libconfig.h++>

#include "boost/bind.hpp"

#include "Application.h"
#include "DataCache.h"
#include "Font.h"
#include "Gfx.h"
#include "states/CutSceneState.h"
#include "Log.h"
#include "ResMan.h"
#include "Settings.h"
#include "SoundPlayer.h"

#include "pakfile/Palette.h"


using namespace libconfig;

// ------------------------------------------------------------------
// CutSceneState::Frame

CutSceneState::Frame::Frame(std::string filename, Transition in, Transition out,
                            bool continuation, uint8_t endWait)
{
    m_filename = filename;
    m_transition_in = in;
    m_transition_out = out;
    m_continuation = continuation;
    
    m_state = TRANSITION_IN;
    m_hold = 0.0f;
    m_transitionPalette = NULL;
	m_palette = DataCache::Instance()->getPalette(INTRO_PAL);

	m_song = -1;
	m_fps = 0;
	m_endWait = endWait;
	m_textColor = 49;
	m_textSurface.reset(new Image(UPoint(320,40)));
	m_textSize = 1.7;
	m_textLocation = SPoint(20,20);
	m_textTransition = 0;
	m_textFade = true;
	font = FontManager::Instance()->getFont("INTRO:INTRO.FNT");

}

void CutSceneState::Frame::addText(uint16_t playAt, std::string text)
{
	m_cutSceneStrings.push_back(cutSceneText(playAt, text));
}

void CutSceneState::Frame::addSound(uint16_t playAt, std::string sound)
{
	Mix_Chunk* sound1;
	Mix_Chunk* sound2 = DataCache::Instance()->getSoundChunk(sound);
	bool exists = false;
	uint8_t i;
	for(i = 0; i < m_soundChunks.size(); i++)
	{
		if(m_soundChunks[i].first == playAt)
		{
			exists = true;
			sound1 = m_soundChunks[i].second;
			break;
		}
	}
    //Sound is not loaded, so let's just add it.
	if(!exists){
		m_soundChunks.push_back(soundChunk(playAt, sound2));
		return;
	}
	
    //Sound is loaded, so let's concat 2 sounds.
	Mix_Chunk* newChunk;
	if((newChunk = (Mix_Chunk*) malloc(sizeof(Mix_Chunk))) == NULL) {
		return;
	}
	
	newChunk->allocated = 1;
	newChunk->volume = sound1->volume;
	newChunk->alen = sound1->alen + sound2->alen;
	
	if((newChunk->abuf = (Uint8 *)malloc(newChunk->alen)) == NULL) {
		free(newChunk);
		return;
	}
	
	memcpy(newChunk->abuf, sound1->abuf, sound1->alen);
	memcpy(newChunk->abuf + sound1->alen, sound2->abuf, sound2->alen);
	m_soundChunks[i].second = newChunk;
	Mix_FreeChunk(sound1);
	Mix_FreeChunk(sound2);
}

void CutSceneState::Frame::setPalette(Palette_enum palette)
{
	m_palette = DataCache::Instance()->getPalette(palette);
}

void CutSceneState::Frame::setSong(uint16_t song)
{
	m_song = song;
}

void CutSceneState::Frame::setFps(float fps)
{
	m_fps = fps;
}

void CutSceneState::Frame::addLoop(uint8_t loopAt, uint8_t rewindTo, uint8_t numLoops, uint8_t wait)
{
	m_loops.push_back(videoLoop(loopAt, rewindTo));
	m_loops.push_back(videoLoop(numLoops, wait));
}

void CutSceneState::Frame::setTextColor(uint8_t textColor)
{
	m_textColor = textColor;
}

void CutSceneState::Frame::setTextLocation(SPoint textLocation)
{
	m_textLocation = textLocation;
}

void CutSceneState::Frame::setTextSize(float textSize)
{
	m_textSize = textSize;
}

void CutSceneState::Frame::setTextFade(bool textFade)
{
	m_textFade = textFade;
}

void CutSceneState::Frame::Load(Frame* lastframe)
{
    LOG_INFO("CutSceneState", "Loading %s", m_filename.c_str());

    std::string::size_type wsaSuffix = m_filename.rfind(".WSA", m_filename.size() - 1);
    std::string::size_type cpsSuffix = m_filename.rfind(".CPS", m_filename.size() - 1);
	bool isWsa = m_filename.size() > 3 && wsaSuffix == m_filename.size() - 4;
	bool isCps = m_filename.size() > 3 && cpsSuffix == m_filename.size() - 4;

	CpsfilePtr tmp;
	if(isWsa || isCps){
		int len;
		uint8_t * data = ResMan::Instance()->readFile(m_filename, &len);
		
		assert(data != NULL);
		
		if(isWsa){
			if (m_continuation)
			{
				m_wsa.reset(new Wsafile(data, len, lastframe->m_animSurface->getSurface(), m_fps));
			}
			else
			{
				m_wsa.reset(new Wsafile(data, len, NULL, m_fps));
			}
		}else{
			if(isCps){
				tmp.reset(new Cpsfile(data, len));
				m_wsa.reset(new Wsafile());
			}
		}
	}else
		m_wsa.reset(new Wsafile());

    
    m_frametime = 0;
    m_currentFrame = 0;
	m_framesPlayed = 0;
    mb_finished = false;
	uint8_t loopFrames = 0;
	for(uint8_t i = 0; i < m_loops.size(); i = i+2)
	{
		 loopFrames += (m_loops[i].first - m_loops[i].second) * m_loops[i+1].first + m_loops[i+1].second;
	}
	m_totalFrames = m_wsa->getNumFrames() + m_endWait + loopFrames;
	if(isCps){
		// A bit retarded, needs to be cleaned up later..
		m_animSurface.reset(tmp->getPicture());
		m_palette = m_animSurface->getSurface()->format->palette;
		m_textSurface = m_animSurface->getResized(2);
		setTextLocation(SPoint(0,-200));
	}
	m_animSurface.reset(m_wsa->getPicture(m_currentFrame, m_palette));
    m_scaledSurface = m_animSurface->getResized(2.0);
	if(m_endWait)
		addLoop(m_wsa->getNumFrames(), m_wsa->getNumFrames(), 1, m_endWait);
}

bool CutSceneState::Frame::Execute(float dt)
{

    switch (m_state)
    {
        case TRANSITION_IN:
            doTransitionIn(dt);
            break;

        case TRANSITION_OUT:
            doTransitionOut(dt);
            break;

        case PLAYING:
            doPlaying(dt);
            break;

        case HOLDING:
            doHolding(dt);
            break;
    };

//    m_scaledSurface->blitToScreen(SPoint(0, Application::Instance()->Screen()->getSurface()->h/2 - m_scaledSurface->getSurface()->h/2 - 55));
	m_scaledSurface->blitToScreenCentered();
	m_textSurface->blitToScreen(SPoint(0 + m_textLocation.x,Application::Instance()->Screen()->getSurface()->h/2 + m_scaledSurface->getSurface()->h/2 + m_textLocation.y));

    return mb_finished;
}


void CutSceneState::Frame::doPlaying(float dt)
{
	if(m_textFade && !m_textTransition && !Mix_Playing(10)){
		m_textTransition = m_framesPlayed + 2;
	}
	if(m_cutSceneStrings.size() > 0){
		if(m_framesPlayed ==  m_cutSceneStrings[0].first){
			ImagePtr tmp(new Image(UPoint(360,50)));
			std::string text = m_cutSceneStrings[0].second;
			uint8_t numLines = 0;
			int linebreak = text.find("\n",0)+ 1;
			std::string thisLine;
			// This is a bit hairy and needs to be cleaned up a bit..
			Uint16 textw, texth;

			while(text.substr(0, linebreak-1).length() > 0){
				thisLine = text.substr(0, linebreak-1);
				if(linebreak != -1)
					thisLine += " ";
				
				font->extents(thisLine, textw, texth);
				
				font->render(thisLine, tmp->getSurface(), tmp->getSurface()->w/2 - textw/2, 10+(numLines++*20) - texth/2, m_textColor);
				if(linebreak == -1 || text == text.substr(linebreak, text.length()-linebreak))
					break;
				text = text.substr(linebreak, text.length()-linebreak);
				linebreak = text.find("\n",0);
			}
			m_textSurface = tmp->getResized(m_textSize);
			m_cutSceneStrings.erase(m_cutSceneStrings.begin());
			m_textTransition = 0;
		}
	}

	if(m_textTransition && m_framesPlayed >= m_textTransition){
		doTransitionOut(m_textSurface, false, true, 6);
	}

	if(m_cutSceneSounds.size() > 0){
		if(m_framesPlayed ==  m_cutSceneSounds[0].first){
			Mix_Chunk* sound = DataCache::Instance()->getSoundChunk(m_cutSceneSounds[0].second);
			SoundPlayer::Instance()->playSound(sound);
			m_cutSceneSounds.erase(m_cutSceneSounds.begin());
		}
	}

	if(m_soundChunks.size() > 0){
		if(m_framesPlayed ==  m_soundChunks[0].first){
			Mix_Chunk* sound = m_soundChunks[0].second;
			SoundPlayer::Instance()->playSound(sound, 10);
//			delete(m_soundChunks[0].second);
			m_soundChunks.erase(m_soundChunks.begin());
		}
	}

    m_frametime += dt;

    if (m_frametime > m_wsa->getFPS())
    {
		if(m_loops.size() > 0)
		{
			if(!(m_currentFrame == m_loops[0].first - 1 && m_framesPlayed < m_loops[0].first + m_loops[1].second))
				m_currentFrame++;
			if(m_loops[1].first != 0 && m_currentFrame == m_loops[0].first)
			{
				m_currentFrame = m_loops[0].second;
				m_loops[1].first--;
			}
			if(m_loops[1].first == 0){
				m_loops.erase(m_loops.begin());
				m_loops.erase(m_loops.begin());
			}
		}else
			m_currentFrame++;


		m_framesPlayed++;		

        m_frametime = 0.0f;
        if (m_currentFrame >= m_wsa->getNumFrames())
        {
            m_state = HOLDING;
        }
        else
        {
            m_animSurface.reset(m_wsa->getPicture(m_currentFrame, m_palette));
            m_scaledSurface = m_animSurface->getResized(2.0);
        };

    };
}

void CutSceneState::Frame::setupTransitionIn()
{
}

void CutSceneState::Frame::cleanupTransitionIn()
{
}

void CutSceneState::Frame::doTransitionIn(float dt) 
{
	if(m_song != -1){
		SoundPlayer::Instance()->playMusic(MUSIC_INTRO, m_song);
	}

    if (m_transition_in == NO_TRANSITION) m_state = PLAYING;
}

void CutSceneState::Frame::setupTransitionOut(ImagePtr img)
{
    m_transitionPalette = new SDL_Color[256];
    memcpy((unsigned char*)m_transitionPalette,
			img->getSurface()->format->palette->colors,
            sizeof(SDL_Color) * 256);
}

void CutSceneState::Frame::cleanupTransitionOut()
{
    delete m_transitionPalette;
}

void CutSceneState::Frame::doTransitionOut(float dt)
{
	doTransitionOut(m_scaledSurface, true);
	doTransitionOut(m_textSurface, true, true);
}

void CutSceneState::Frame::doTransitionOut(ImagePtr img, bool done, bool forceTransition, const int fadeAmt) 
{
    if (m_transition_out == NO_TRANSITION && !forceTransition) 
    {
        mb_finished = true;
        return;
    }

    //if (m_transitionPalette == NULL) // We want new one created to handle several transitions per cutscene
	setupTransitionOut(img);
    
    SDL_Surface* screen = img->getSurface();
    SDL_Color* col = m_transitionPalette;


    for (int i=0; i!=256; i++, col++)
    {
        if (col->r > fadeAmt || col->g > fadeAmt || col->b > fadeAmt)
        {
           if (col->r > fadeAmt) col->r -=fadeAmt;
           if (col->g > fadeAmt) col->g -=fadeAmt;
           if (col->b > fadeAmt) col->b -=fadeAmt;
            
           done = false;
        }
    };

    SDL_SetPalette(screen, SDL_LOGPAL, m_transitionPalette, 0, 256);

    if (done)
    {
        cleanupTransitionOut();
        mb_finished = true;
    };
}

void CutSceneState::Frame::doHolding(float dt)
{
    m_state = TRANSITION_OUT;
}

CutSceneState::Frame::~Frame()
{			
    m_loops.clear();
	m_cutSceneStrings.clear();
	m_cutSceneSounds.clear();
	m_soundChunks.clear();

}
// ------------------------------------------------------------------
// CutSceneState

CutSceneState::CutSceneState(std::string scene)
{	
    Config * dataConfig = new Config();

    try
    {
        dataConfig->readFile("data.dunetxt");
    }
    catch(ParseException& ex)
    {
        LOG_FATAL("CutSceneState", "Fatal error loading configuration file on line %d: %s", 
            ex.getLine(), ex.getError());

        exit(EXIT_FAILURE);
    }

	m_currentFrame = NULL;

//	m_cutSceneStrings.push_back(cutSceneText(0, "")); // credits.eng isn't properly decoded yet..
												// DataCache::Instance()->getCreditsString(20)));
    std::string filename;
    bool continuation;
    int hold, song;
    int textColour;
    float fps;
    std::string path = ".cutscenes.";
    path += scene;
    
    Setting &node = dataConfig->lookup(path);

    try
    {
        for (int i = 0; i < node.getLength(); i++)
        {
            filename = "";
            song = -1;
            hold = 0;
            fps = 1;
            continuation = false;

            
            bool fadeOut = false;
            Frame::Transition aa = Frame::NO_TRANSITION;
            node[i].lookupValue("fade_out", fadeOut);
            if (fadeOut) aa = Frame::FADE_OUT;
            aa = Frame::FADE_OUT;
            
            node[i].lookupValue("filename", filename);
            node[i].lookupValue("hold", hold);
            node[i].lookupValue("continuation", continuation);
        	frame = new Frame(filename,
                             Frame::NO_TRANSITION, 
                             aa,
                             continuation, hold);

            if (node[i].lookupValue("text_colour", textColour))
            {
            	frame->setTextColor(textColour);
            }

            
            if (node[i].exists("loop"))
            {
                frame->addLoop((int)node[i]["loop"][0],
                               (int)node[i]["loop"][1], 
                               (int)node[i]["loop"][2], 
                               (int)node[i]["loop"][3]);
            }
            
            int textSize;
            
            if (node[i].lookupValue("text_size", textSize))
            {
                frame->setTextSize(textSize);
            }

            if (node[i].exists("text_location"))
            {
                frame->setTextLocation(UPoint((int)node[i]["text_location"][0], (int)node[i]["text_location"][1]));
            }
            
            if (node[i].lookupValue("fps", fps))
                frame->setFps(fps);
            if (node[i].lookupValue("song", song))                
                frame->setSong(song);

           //TODO: It does look interesting. Need to make it more readable, I think
             if (node[i].exists("text"))
            {
                for (int j = 0; j < node[i]["text"].getLength(); j++)
                {
                    int time = node[i]["text"][j][0];
                    std::string text = node[i]["text"][j][1];

                    frame->addText(time, text);
                }
            }

            if (node[i].exists("text_string"))
            {
                for (int j = 0; j < node[i]["text_string"].getLength(); j++)
                {
                    int time = node[i]["text_string"][j][0];
                    int string_id = node[i]["text_string"][j][1];

                    frame->addText(time, DataCache::Instance()->getIntroString(string_id));
                }
            }
            
            if (node[i].exists("sounds"))
            {
                for (int j = 0; j < node[i]["sounds"].getLength(); j++)
                {
                    int time = node[i]["sounds"][j][0];
                    std::string sound_id = node[i]["sounds"][j][1];
                    
                    frame->addSound(time, sound_id);
                }
            }            
            enque(frame);
        }
    }  
    catch(ParseException& ex)
    {
        LOG_FATAL("CutSceneState", "Setting not found %d: %s", 
            ex.getLine(), ex.getError());
    }
   
    delete (dataConfig);

#if 0
	frame = new Frame("INTRO:WESTWOOD.WSA",
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
                     false, 20);
	frame->setSong(0);
	frame->setFps(0.15);
	frame->setPalette(WESTWOOD_PAL);
	enque(frame);

	frame = new Frame("",
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
                     false, 10);
	frame->addText(0, "and");
	frame->setTextSize(2.0);
	frame->setTextLocation(SPoint(-25,0));
	enque(frame);

	frame = new Frame("INTRO:VIRGIN.CPS",
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
                     false, 12);
	enque(frame);

	frame = new Frame("",
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
                     false, 10);
	frame->addText(0,DataCache::Instance()->getIntroString(1));
	frame->setTextLocation(SPoint(-25,0));
	frame->setTextSize(2.0);
	enque(frame);

	frame =  new Frame("INTRO:INTRO1.WSA",
                     Frame::NO_TRANSITION,
                     Frame::FADE_OUT,
                     false, 65);
	frame->setFps(0.07);
	frame->addLoop(1,1,0,20);
	frame->setSong(1);
	frame->addSound(30, "Intro_Dune");
	frame->concatSound(70, "Intro_TheBuilding");
	frame->concatSound(70, "Intro_OfADynasty");
	frame->addText(70, DataCache::Instance()->getIntroString(2));
	frame->setTextLocation(SPoint(-13,-35));
	frame->setTextSize(2.0);
	frame->setTextFade(false);
	enque(frame);

    frame = new Frame("INTRO:INTRO2.WSA",
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
                     false, 5);
	frame->concatSound(10, "Intro_ThePlanetArrakis");
	frame->concatSound(10, "Intro_KnownAsDune");
	frame->addText(10, DataCache::Instance()->getIntroString(3));
	enque(frame);

    frame = new Frame("INTRO:INTRO3.WSA",  
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
                     false, 25);
	frame->concatSound(0, "Intro_LandOfSand");
	frame->addText(0, DataCache::Instance()->getIntroString(4));
	frame->concatSound(33, "Intro_Home");
	frame->concatSound(33, "Intro_OfTheSpice");
	frame->concatSound(33, "Intro_Melange");
	frame->addText(33, DataCache::Instance()->getIntroString(5));
	enque(frame);

    frame = new Frame("INTRO:INTRO9.WSA",  
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
                     false, 2);
	frame->setFps(0.16);
	frame->addText(0, DataCache::Instance()->getIntroString(6));
	frame->concatSound(0, "Intro_TheSpice");
	frame->concatSound(0, "Intro_Controls");
	frame->concatSound(0, "Intro_TheEmpire");
	frame->addSound(18, "Intro_Clank");
	frame->addText(32, DataCache::Instance()->getIntroString(7));
	frame->concatSound(32, "Intro_WhoEver");
	frame->concatSound(32, "Intro_ControlsDune");
	frame->concatSound(32, "Intro_ControlsTheSpice");
	frame->addSound(53, "Intro_Brakes_2p");
	frame->addSound(61, "Intro_Clank");
	enque(frame);

    frame = new Frame("INTRO:INTRO10.WSA",  
                     Frame::NO_TRANSITION, 
                     Frame::NO_TRANSITION,
                     false, 25);
	frame->addText(0, DataCache::Instance()->getIntroString(8));
	frame->concatSound(0, "Intro_TheEmperor");
	frame->concatSound(0, "Intro_HasProposedAChallenge");
	frame->concatSound(0, "Intro_ToEachOfTheHouses");
	enque(frame);

    frame = new Frame("INTRO:INTRO11.WSA",  
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
                     false);
	frame->setFps(0.095);
	frame->addText(0, DataCache::Instance()->getIntroString(9));
	frame->setTextColor(208);
	frame->concatSound(0, "Intro_TheHouse");
	frame->concatSound(0, "Intro_ThatProduces");
	frame->concatSound(0, "Intro_TheMostSpice");
	frame->concatSound(0, "Intro_WillControlDune");
	frame->addLoop(45, 0, 1, 14);
	frame->addText(61, DataCache::Instance()->getIntroString(10));
	frame->concatSound(61, "Intro_ThereAreNoSet");
	frame->concatSound(61, "Intro_Territories");
	frame->addText(83, DataCache::Instance()->getIntroString(11));
	frame->concatSound(83, "Intro_AndNo");
	frame->concatSound(83, "Intro_RulesOfEngagement");

	enque(frame);

    frame = new Frame("INTRO:INTRO4.WSA", 
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
                     false);
	frame->addText(11, DataCache::Instance()->getIntroString(12));
	frame->concatSound(11, "Intro_VastArmies");
	frame->concatSound(11, "Intro_HasArrived");
	enque(frame);

    frame = new Frame("", 
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
                     false, 38);
	frame->addText(0,DataCache::Instance()->getIntroString(13));
	frame->setTextLocation(SPoint(-25,0));
	frame->setTextSize(2.0);
	frame->setTextFade(false);	
	frame->concatSound(0, "Intro_AndNow");
	frame->concatSound(0, "Intro_3Houses");
	frame->concatSound(0, "Intro_ForControl");
	frame->concatSound(0, "Intro_OfDune");
	enque(frame);


    frame = new Frame("INTRO:INTRO6.WSA", 
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
                     false);
	frame->setFps(0.08);
	frame->addText(0, DataCache::Instance()->getIntroString(14));
	frame->addSound(0, "Intro_TheNobleAtreides");
	frame->addSound(31, "Intro_Glass");
	frame->addSound(32, "Intro_Glass");
	frame->addSound(33, "Intro_Glass");
	frame->addSound(63, "Intro_Glass");
	frame->addSound(64, "Intro_Glass");
	frame->addSound(65, "Intro_Glass");
	enque(frame);

    frame =  new Frame("INTRO:INTRO7A.WSA", 
                     Frame::NO_TRANSITION, 
                     Frame::NO_TRANSITION,
                     false);
	frame->addText(0, DataCache::Instance()->getIntroString(15));
	frame->concatSound(0, "Intro_TheInsideous");
	frame->concatSound(0, "Intro_Ordos");
	frame->addSound(2, "Intro_Missile_8");
	frame->addSound(7, "Intro_Missile_8");
	frame->addSound(26, "Intro_Missile_8");
	enque(frame);

    frame = new Frame("INTRO:INTRO7B.WSA", 
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
                     true, 5);
	frame->addSound(10, "Intro_Missile_8");
	enque(frame);

    frame = new Frame("INTRO:INTRO8A.WSA", 
                     Frame::NO_TRANSITION, 
                     Frame::NO_TRANSITION,
                     false);
	frame->addText(0, DataCache::Instance()->getIntroString(16));
	frame->concatSound(0, "Intro_AndThe");
	frame->concatSound(0, "Intro_EvilHarkonnen");
	frame->addSound(5, "Sound_Gun");
	frame->addSound(8, "Sound_Gun");
	enque(frame);

    frame = new Frame("INTRO:INTRO8B.WSA",  
                     Frame::NO_TRANSITION, 
                     Frame::NO_TRANSITION,
                     true);
	enque(frame);
	frame->addSound(1, "Sound_Gun");
	

    frame = new Frame("INTRO:INTRO8C.WSA", 
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
                     true);
	frame->addSound(16, "Sound_ExplosionSmall");
	frame->addSound(25, "Sound_ExplosionMedium");
	enque(frame);

    frame = new Frame("INTRO:INTRO5.WSA", 
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
                     false);
	frame->setFps(0.2);
	frame->addLoop(3, 0, 10, 0);
	frame->addText(0, DataCache::Instance()->getIntroString(17));
	frame->concatSound(0, "Intro_OnlyOneHouse");
	frame->concatSound(0, "Intro_WillPrevail");
	enque(frame);


    frame = new Frame("",
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
                     false, 22);
	frame->addText(0, DataCache::Instance()->getIntroString(18));
	frame->setTextLocation(SPoint(-20,0));
	frame->setTextSize(2.0);	
	frame->setTextFade(false);	
	frame->concatSound(0, "Intro_Your");
	frame->concatSound(0, "Intro_BattleForDune");
	frame->concatSound(0, "Intro_Begins");
	enque(frame);


    frame = new Frame("", 
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
                     false, 20);
	frame->addText(0, DataCache::Instance()->getIntroString(19));
	frame->setTextLocation(SPoint(-20,0));
	frame->setTextSize(2.0);
	frame->setTextFade(false);	
	frame->addSound(0, "Intro_Now");
	enque(frame);

    // seems nice to play this again ;)
  /*  frame = new Frame("INTRO:INTRO1.WSA",  
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
                     false);
	enque(frame);*/

#endif

    next();
    m_butCutScene = new TranspButton(Settings::Instance()->GetWidth(),
                                  Settings::Instance()->GetHeight());
                                  
    m_butCutScene->onClick.connect(
            boost::bind(&CutSceneState::SkipCutScene, this) );
}

CutSceneState::~CutSceneState()
{
}

void CutSceneState::SkipCutScene()
{
	delete m_currentFrame;
    mp_parent->PopState();
    
}

void CutSceneState::JustMadeActive()
{
    Application::Instance()->SetClearColor(0);
    Application::Instance()->RootWidget()->addChild(m_butCutScene);
    State::JustMadeActive();
}

void CutSceneState::JustMadeInactive()
{
    Application::Instance()->RootWidget()->deleteChild(m_butCutScene);
    SoundPlayer::Instance()->stopMusic();
    State::JustMadeInactive();
}

bool CutSceneState::next()
{

    LOG_INFO("CutSceneState", "Loading next..");
    CutSceneList::iterator it = m_wsaNames.begin();
    if (it == m_wsaNames.end() )
    {
        return false;
    }

    Frame* nextFrame = *it;
    nextFrame->Load(m_currentFrame);
    m_wsaNames.pop_front();


    if (m_currentFrame != NULL) delete m_currentFrame;
    m_currentFrame = nextFrame;

    return true;
}

int CutSceneState::Execute(float dt)
{
    if (m_currentFrame->Execute(dt))
    {
        if (!next()) return -1;
    };

    return 0;
}



