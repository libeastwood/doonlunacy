#include "IntroState.h"
#include "ResMan.h"
#include "Application.h"
#include "pakfile/Palette.h"

IntroState::IntroState()
{
    enque("INTRO:INTRO1.WSA");
    /*
    enque("INTRO:INTRO2.WSA");
    enque("INTRO:INTRO3.WSA");
    enque("INTRO:INTRO4.WSA");
    enque("INTRO:INTRO5.WSA");
    enque("INTRO:INTRO6.WSA");
    enque("INTRO:INTRO7A.WSA");
    enque("INTRO:INTRO7B.WSA");
    enque("INTRO:INTRO8A.WSA");
    enque("INTRO:INTRO8B.WSA");
    enque("INTRO:INTRO8C.WSA");
    enque("INTRO:INTRO9.WSA");
    enque("INTRO:INTRO10.WSA");
    enque("INTRO:INTRO11.WSA");
    */

    m_currentFrame = 0;
    m_frametime = 0.0f;

    
    m_wsa = NULL;

    next();
};

IntroState::~IntroState()
{
    if (m_wsa != NULL) delete m_wsa;
};

void IntroState::JustMadeActive()
{
    
    State::JustMadeActive();
};

void IntroState::JustMadeInactive()
{
    State::JustMadeInactive();
};

void IntroState::load(std::string name)
{
    printf("intro loading %s\n", name.c_str());
    if (m_wsa != NULL) delete m_wsa;
    
    int len;
    unsigned char* data = ResMan::Instance()->readFile(name, &len);

    assert(data != NULL);

    m_wsa = new Wsafile(data, len);
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

    if (m_frametime > 0.1f)
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



