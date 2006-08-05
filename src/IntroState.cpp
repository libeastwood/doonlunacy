#include "IntroState.h"
#include "ResMan.h"
#include "Application.h"
#include "Settings.h"
#include "pakfile/Palette.h"
#include "boost/bind.hpp"



IntroState::IntroState()
{
    enque(Frame("INTRO:INTRO1.WSA", false, 1.0f));
    enque(Frame("INTRO:INTRO2.WSA", false, 0.0f));
    enque(Frame("INTRO:INTRO3.WSA", false, 0.0f));
    enque(Frame("INTRO:INTRO4.WSA", false, 0.0f));
    enque(Frame("INTRO:INTRO5.WSA", false, 0.0f));
    enque(Frame("INTRO:INTRO6.WSA", false, 0.0f));
    enque(Frame("INTRO:INTRO7A.WSA", false, 0.0f));
    enque(Frame("INTRO:INTRO7B.WSA", true, 0.0f));
    enque(Frame("INTRO:INTRO8A.WSA", false, 0.0f));
    enque(Frame("INTRO:INTRO8B.WSA", true, 0.0f));
    enque(Frame("INTRO:INTRO8C.WSA", true, 0.0f));
    enque(Frame("INTRO:INTRO9.WSA", false, 0.0f));
    enque(Frame("INTRO:INTRO10.WSA", false, 1.0f));
    enque(Frame("INTRO:INTRO11.WSA", false, 0.0f));
    // seems nice to play this again ;)
    enque(Frame("INTRO:INTRO1.WSA", false, 1.0f));

    m_currentFrame = 0;
    m_frametime = 0.0f;

    
    m_wsa = NULL;

    next();
    m_butIntro = new TranspButton(Settings::Instance()->GetWidth(),
                                  Settings::Instance()->GetHeight());
                                  
    m_butIntro->onClick.connect(
            boost::bind(&IntroState::SkipIntro, this) );
};

IntroState::~IntroState()
{
    if (m_wsa != NULL) delete m_wsa;
};

void IntroState::SkipIntro()
{
    mp_parent->PopState();
}

void IntroState::JustMadeActive()
{
 
    Application::Instance()->RootWidget()->addChild(m_butIntro);
    State::JustMadeActive();
};

void IntroState::JustMadeInactive()
{
    Application::Instance()->RootWidget()->deleteChild(m_butIntro);
    State::JustMadeInactive();
};

void IntroState::load(Frame frame)
{
    printf("intro loading %s\n", frame.filename.c_str());
    Wsafile* old_wsa = m_wsa;
    
    int len;
    unsigned char* data = ResMan::Instance()->readFile(frame.filename, &len);

    assert(data != NULL);

    if (frame.continuation)
    {
        m_wsa = new Wsafile(data, len, m_animSurface);
    }
    else
    {
        m_wsa = new Wsafile(data, len);
    }

    if (old_wsa != NULL) delete old_wsa;
};

bool IntroState::next()
{
    printf("loading next..\n");
    IntroList::iterator it = m_wsaNames.begin();
    if (it == m_wsaNames.end() )
    {
        return false;
    }

    load(*it);
    m_wsaNames.pop_front();
    m_currentFrame = 0 ;

    return true;
};

int IntroState::Execute(float dt)
{
    m_frametime += dt;

    if (m_frametime > m_wsa->getFPS())
    {
        m_frametime = 0.0f;
        m_currentFrame ++;
        if (m_currentFrame >= m_wsa->getNumFrames())
        {
            if (!next()) return -1;
        };
    };

    SDL_Palette* palette = Application::Instance()->Screen()->format->palette;
    
    /*
    printf("ISE %u %u %u\n", palette->colors[12].r,
                         palette->colors[12].g,
                         palette->colors[12].b);
    */
    
    m_animSurface = m_wsa->getPicture(m_currentFrame, palette);

    assert(m_animSurface != NULL);
    
    Application::Instance()->BlitCentered(m_animSurface);

    return 0;
};



