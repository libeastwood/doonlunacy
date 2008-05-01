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

// ------------------------------------------------------------------
// IntroState::Frame

IntroState::Frame::Frame(std::string filename, 
                            Transition in, Transition out,
						 	std::vector<introText> introStrings,
                            bool continuation, int8_t song, Palette_enum pal)
{
    m_filename = filename;
    m_transition_in = in;
    m_transition_out = out;
    m_continuation = continuation;
    
    m_state = TRANSITION_IN;
    m_hold = 0.0f;
    m_transitionPalette = NULL;
	m_introStrings = introStrings;
	m_container = new Container();
//    m_container->setPosition(UPoint(250, 400));
    m_container->setSize(UPoint(Settings::Instance()->GetWidth(),
								Settings::Instance()->GetHeight()));
	m_palette = DataCache::Instance()->getPalette(pal);

	m_song = song;
    
    Application::Instance()->RootWidget()->addChild(m_container);

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
	if(m_introStrings.size() > 0){
		m_subText = new Label(m_introStrings[0].second, 49, 0);
		m_container->setPosition(UPoint(50, 420));
		m_container->addChild(m_subText);
	}    
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
    m_state = TRANSITION_OUT;
}

IntroState::Frame::~Frame()
{
	m_container->deleteChild(m_subText);
}
// ------------------------------------------------------------------
// IntroState

IntroState::IntroState()
{
	m_introStrings.push_back(introText(0, "")); // credits.eng isn't properly decoded yet..
												// DataCache::Instance()->getCreditsString(20)));
    enque( new Frame("INTRO:WESTWOOD.WSA",  
                     Frame::NO_TRANSITION, 
                     Frame::NO_TRANSITION,
					 m_introStrings,
                     false, 0, WESTWOOD_PAL) );
	m_introStrings.clear();

	m_introStrings.push_back(introText(0, DataCache::Instance()->getIntroString(2)));
    enque( new Frame("INTRO:INTRO1.WSA",  
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
					 m_introStrings,
                     false, 1) );
	m_introStrings.clear();
	m_introStrings.push_back(introText(0, DataCache::Instance()->getIntroString(3)));

    enque( new Frame("INTRO:INTRO2.WSA",  
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
					 m_introStrings,
                     false) );
	m_introStrings.clear();
	m_introStrings.push_back(introText(0, DataCache::Instance()->getIntroString(4)));

    enque( new Frame("INTRO:INTRO3.WSA",  
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
					 m_introStrings,
                     false) );
	m_introStrings.clear();
	m_introStrings.push_back(introText(0, DataCache::Instance()->getIntroString(5)));

    enque( new Frame("INTRO:INTRO9.WSA",  
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
					 m_introStrings,
                     false) );
	m_introStrings.clear();
	m_introStrings.push_back(introText(0, DataCache::Instance()->getIntroString(6)));

    enque( new Frame("INTRO:INTRO10.WSA",  
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
					 m_introStrings,
                     false) );
	m_introStrings.clear();
	m_introStrings.push_back(introText(0, DataCache::Instance()->getIntroString(7)));

    enque( new Frame("INTRO:INTRO11.WSA",  
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
					 m_introStrings,
                     false) ); 
	m_introStrings.clear();
	m_introStrings.push_back(introText(0, DataCache::Instance()->getIntroString(8)));

    enque( new Frame("INTRO:INTRO4.WSA", 
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
					 m_introStrings,
                     false) );
	m_introStrings.clear();
	m_introStrings.push_back(introText(0, DataCache::Instance()->getIntroString(9)));

    enque( new Frame("INTRO:INTRO6.WSA", 
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
					 m_introStrings,
                     false) );
	m_introStrings.clear();
	m_introStrings.push_back(introText(0, DataCache::Instance()->getIntroString(10)));

    enque( new Frame("INTRO:INTRO7A.WSA", 
                     Frame::NO_TRANSITION, 
                     Frame::NO_TRANSITION,
					 m_introStrings,
                     false) );
	m_introStrings.clear();
	m_introStrings.push_back(introText(0, DataCache::Instance()->getIntroString(11)));

    enque( new Frame("INTRO:INTRO7B.WSA", 
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
					 m_introStrings,
                     true) );
	m_introStrings.clear();
	m_introStrings.push_back(introText(0, DataCache::Instance()->getIntroString(12)));

    enque( new Frame("INTRO:INTRO8A.WSA", 
                     Frame::NO_TRANSITION, 
                     Frame::NO_TRANSITION,
					 m_introStrings,
                     false) );
	m_introStrings.clear();
	m_introStrings.push_back(introText(0, DataCache::Instance()->getIntroString(13)));

    enque( new Frame("INTRO:INTRO8B.WSA",  
                     Frame::NO_TRANSITION, 
                     Frame::NO_TRANSITION,
					 m_introStrings,
                     true) );
	m_introStrings.clear();
	m_introStrings.push_back(introText(0, DataCache::Instance()->getIntroString(14)));

    enque( new Frame("INTRO:INTRO8C.WSA", 
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
					 m_introStrings,
                     true) );
	m_introStrings.clear();
	m_introStrings.push_back(introText(0, DataCache::Instance()->getIntroString(15)));

    enque( new Frame("INTRO:INTRO5.WSA", 
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
					 m_introStrings,
                     false) );
    // seems nice to play this again ;)
    enque( new Frame("INTRO:INTRO1.WSA",  
                     Frame::NO_TRANSITION, 
                     Frame::FADE_OUT,
					 m_introStrings,
                     false) );

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

//    if (m_currentFrame != NULL) delete m_currentFrame;
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



