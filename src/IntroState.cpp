#include "IntroState.h"
#include "ResMan.h"
#include "Application.h"
#include "DataCache.h"
#include "Gfx.h"
#include "Settings.h"
#include "pakfile/Palette.h"
#include "boost/bind.hpp"
#include "gui2/Label.h"
#include "gui2/Container.h"
#include <iostream>

// ------------------------------------------------------------------
// IntroState::Frame

IntroState::Frame::Frame(std::string filename, Transition in, Transition out,
                            bool continuation, uint16_t endWait)
{
    m_filename = filename;
    m_transition_in = in;
    m_transition_out = out;
    m_continuation = continuation;
    
    m_state = TRANSITION_IN;
    m_hold = 0.0f;
    m_transitionPalette = NULL;
	m_container = new Container();
    m_container->setSize(UPoint(Settings::Instance()->GetWidth(),
								Settings::Instance()->GetHeight()));
	m_subText = NULL;
	m_palette = DataCache::Instance()->getPalette(INTRO_PAL);

	m_song = -1;
	m_endWait = endWait;
    
    Application::Instance()->RootWidget()->addChild(m_container);

}

void IntroState::Frame::addText(uint16_t playAt, std::string text)
{
	m_introStrings.push_back(introText(playAt, text));
}

void IntroState::Frame::addSound(uint16_t playAt, Sound_enum sound)
{
	m_introSounds.push_back(introSound(playAt, sound));
}

void IntroState::Frame::setPalette(Palette_enum palette)
{
	m_palette = DataCache::Instance()->getPalette(palette);
}

void IntroState::Frame::setSong(uint8_t song)
{
	m_song = song;
}

void IntroState::Frame::Load(Frame* lastframe)
{
//    SDL_Palette* palette = Application::Instance()->Screen()->getSurface()->format->palette;

    printf("intro loading %s\n", m_filename.c_str());
    
    int len;
    uint8_t * data = ResMan::Instance()->readFile(m_filename, &len);

    assert(data != NULL);

    if (m_continuation)
    {
        m_wsa.reset(new Wsafile(data, len, lastframe->m_animSurface->getSurface()));
    }
    else
    {
        m_wsa.reset(new Wsafile(data, len));
    }
    
    m_frametime = 0;
    m_currentFrame = 0;
    mb_finished = false;

    m_animSurface.reset(m_wsa->getPicture(m_currentFrame, m_palette));
    m_scaledSurface = m_animSurface->getResized(2.0);
}

bool IntroState::Frame::Execute(float dt)
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

    m_scaledSurface->blitToScreenCentered();

    return mb_finished;
}


void IntroState::Frame::doPlaying(float dt)
{
	if(m_introStrings.size() > 0){
		if(m_currentFrame ==  m_introStrings[0].first){
			if(m_subText != NULL) m_container->deleteChild(m_subText);
			m_subText = new Label(m_introStrings[0].second, 49, 0);
			m_introStrings.erase(m_introStrings.begin());
			m_container->setPosition(UPoint(50, 420));
			m_container->addChild(m_subText);
		}
	}

	if(m_introSounds.size() > 0){
		if(m_currentFrame ==  m_introSounds[0].first){
			Mix_Chunk* sound = DataCache::Instance()->getSoundChunk(m_introSounds[0].second);
			Application::Instance()->playSound(sound);
			m_introSounds.erase(m_introSounds.begin());
		}
	}

    m_frametime += dt;

    if (m_frametime > m_wsa->getFPS())
    {
        m_currentFrame ++;
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

void IntroState::Frame::setupTransitionIn()
{
}

void IntroState::Frame::cleanupTransitionIn()
{
}

void IntroState::Frame::doTransitionIn(float dt) 
{
	if(m_song != -1){
		Application::Instance()->playSound(DataCache::Instance()->getMusic(MUSIC_INTRO, m_song));
	}

    if (m_transition_in == NO_TRANSITION) m_state = PLAYING;
}

void IntroState::Frame::setupTransitionOut()
{
    m_transitionPalette = new SDL_Color[256];
    memcpy((unsigned char*)m_transitionPalette,
			m_palette->colors,
            sizeof(SDL_Color) * 256);
}

void IntroState::Frame::cleanupTransitionOut()
{
    delete m_transitionPalette;
}

void IntroState::Frame::doTransitionOut(float dt) 
{

    if (m_transition_out == NO_TRANSITION) 
    {
        mb_finished = true;
        return;
    }

    if (m_transitionPalette == NULL) setupTransitionOut();
    
    bool done = true;
    SDL_Surface* screen = m_scaledSurface->getSurface();
    SDL_Color* col = m_transitionPalette;

    const int fadeAmt = 3;

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

void IntroState::Frame::doHolding(float dt)
{
	uint16_t startTicks = SDL_GetTicks();

	SDL_Event event;
	bool wait = true;
	while(wait && (SDL_GetTicks() - startTicks)  <  m_endWait){
		while(SDL_PollEvent(&event)){
			switch (event.type){
				case (SDL_KEYDOWN):
					switch( event.key.keysym.sym ){
						case SDLK_RETURN:
						case SDLK_SPACE:
							wait = false;
							break;
						case SDLK_ESCAPE:
							if(m_subText != NULL) m_container->deleteChild(m_subText);
							Application::Instance()->RootState()->PopState();
							return;
						default:
							break;
					}}}
	}

    m_state = TRANSITION_OUT;
}

IntroState::Frame::~Frame()
{
	if(m_subText != NULL) m_container->deleteChild(m_subText);
}
// ------------------------------------------------------------------
// IntroState

IntroState::IntroState()
{
	m_currentFrame = NULL;

//	m_introStrings.push_back(introText(0, "")); // credits.eng isn't properly decoded yet..
												// DataCache::Instance()->getCreditsString(20)));
	frame = new Frame("INTRO:WESTWOOD.WSA",  
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
                     false, 5000);
	frame->setSong(0);
	frame->setPalette(WESTWOOD_PAL);
	enque(frame);

	frame =  new Frame("INTRO:INTRO1.WSA",
                     Frame::NO_TRANSITION,
                     Frame::FADE_OUT,
                     false, 2000);
	frame->setSong(1);
	frame->addSound(5, Intro_Dune);
	frame->addSound(25, Intro_TheBuilding);
	frame->addSound(49, Intro_OfADynasty);
	frame->addText(48, "The Building of a Dynasty");
	enque(frame);

    frame = new Frame("INTRO:INTRO2.WSA",
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
                     false, 2000);
	frame->addSound(0, Intro_ThePlanetArrakis);
	frame->addSound(11, Intro_KnownAsDune);
	frame->addText(0, DataCache::Instance()->getIntroString(3));
	enque(frame);

    frame = new Frame("INTRO:INTRO3.WSA",  
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
                     false, 2000);
	frame->addSound(0, Intro_LandOfSand);
	frame->addText(0, DataCache::Instance()->getIntroString(4));
	frame->addSound(20, Intro_Home);
	frame->addSound(33, Intro_OfTheSpice);
	frame->addSound(39, Intro_Melange);
	frame->addText(33, DataCache::Instance()->getIntroString(5));
	enque(frame);

    frame = new Frame("INTRO:INTRO9.WSA",  
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
                     false, 3000);
	frame->addSound(0, Intro_TheSpice);
	frame->addSound(5, Intro_Controls);
	frame->addSound(9, Intro_TheEmpire);
	frame->addSound(10, Intro_Clank);
	frame->addSound(25, Intro_WhoEver);
	frame->addSound(29, Intro_ControlsDune);
	frame->addSound(35, Intro_ControlsTheSpice);
	frame->addSound(50, Intro_Brakes_2p);
	frame->addSound(58, Intro_Clank);
	frame->addText(0, DataCache::Instance()->getIntroString(6));
	frame->addText(50, DataCache::Instance()->getIntroString(7));
	enque(frame);

    frame = new Frame("INTRO:INTRO10.WSA",  
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
                     false, 2000);
	frame->addText(0, DataCache::Instance()->getIntroString(8));
	frame->addSong(0, Intro_TheEmperor);
	enque(frame);

    frame = new Frame("INTRO:INTRO11.WSA",  
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
                     false, 2000);
	frame->addText(0, DataCache::Instance()->getIntroString(9));
	frame->addText(25, DataCache::Instance()->getIntroString(10));
	frame->addText(41, DataCache::Instance()->getIntroString(11));
	enque(frame);

    frame = new Frame("INTRO:INTRO4.WSA", 
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
                     false, 3000);
	frame->addText(0, DataCache::Instance()->getIntroString(12));
	frame->addText(25, DataCache::Instance()->getIntroString(13));
	enque(frame);

    frame = new Frame("INTRO:INTRO6.WSA", 
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
                     false);
	frame->addText(0, DataCache::Instance()->getIntroString(14));
	enque(frame);

    frame =  new Frame("INTRO:INTRO7A.WSA", 
                     Frame::NO_TRANSITION, 
                     Frame::NO_TRANSITION,
                     false);
	frame->addText(0, DataCache::Instance()->getIntroString(15));
	enque(frame);

    frame = new Frame("INTRO:INTRO7B.WSA", 
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
                     true, 2000);
	frame->addText(0, DataCache::Instance()->getIntroString(16));
	enque(frame);

    frame = new Frame("INTRO:INTRO8A.WSA", 
                     Frame::NO_TRANSITION, 
                     Frame::NO_TRANSITION,
                     false);
	frame->addText(0, DataCache::Instance()->getIntroString(17));
	enque(frame);

    frame = new Frame("INTRO:INTRO8B.WSA",  
                     Frame::NO_TRANSITION, 
                     Frame::NO_TRANSITION,
                     true);
	frame->addText(0, DataCache::Instance()->getIntroString(18));
	enque(frame);

    frame = new Frame("INTRO:INTRO8C.WSA", 
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
                     true, 2000);
	frame->addText(0, DataCache::Instance()->getIntroString(19));
	enque(frame);

    frame = new Frame("INTRO:INTRO5.WSA", 
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
                     false);
	frame->addText(0, DataCache::Instance()->getIntroString(20));
	enque(frame);

    // seems nice to play this again ;)
    frame = new Frame("INTRO:INTRO1.WSA",  
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
                     false);
	enque(frame);

    next();
    m_butIntro = new TranspButton(Settings::Instance()->GetWidth(),
                                  Settings::Instance()->GetHeight());
                                  
    m_butIntro->onClick.connect(
            boost::bind(&IntroState::SkipIntro, this) );
}

IntroState::~IntroState()
{
}

void IntroState::SkipIntro()
{
	delete m_currentFrame;
    mp_parent->PopState();
    
}

void IntroState::JustMadeActive()
{
    Application::Instance()->SetClearColor(0);
    Application::Instance()->RootWidget()->addChild(m_butIntro);
    State::JustMadeActive();
}

void IntroState::JustMadeInactive()
{
    Application::Instance()->RootWidget()->deleteChild(m_butIntro);
    State::JustMadeInactive();
}

bool IntroState::next()
{

    fprintf(stderr, "loading next..\n");
    IntroList::iterator it = m_wsaNames.begin();
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

int IntroState::Execute(float dt)
{
    if (m_currentFrame->Execute(dt))
    {
        if (!next()) return -1;
    };

    return 0;
}



