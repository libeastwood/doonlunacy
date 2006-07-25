#include "IntroState.h"
#include "ResMan.h"
#include "Application.h"
#include "pakfile/Palette.h"

IntroState::IntroState()
{
    unsigned char* data;
    int len = ResMan::Instance()->readFile("INTRO:INTRO2.WSA", data);

    printf("intro1 len %d\n", len);

    m_wsa = new Wsafile(data, len);

    m_currentFrame = 0;
    m_frametime = 0.0f;
};

IntroState::~IntroState()
{
    delete m_wsa;
};

void IntroState::JustMadeActive()
{
    unsigned char* data;
    int len = ResMan::Instance()->readFile("INTRO:INTRO.PAL", data);
    
    Palettefile pal (data, len);
    Application::Instance()->SetPalette(pal.getPalette());
    
    State::JustMadeActive();
};

void IntroState::JustMadeInactive()
{
    State::JustMadeInactive();
};

int IntroState::Execute(float dt)
{
    m_frametime += dt;

    if (m_frametime > 1.0f)
    {
        m_frametime = 0.0f;
        m_currentFrame ++;
        if (m_currentFrame >= m_wsa->getNumFrames())
        {
            return -1;
        };
    };

    m_animSurface = m_wsa->getPicture(m_currentFrame);
    Application::Instance()->BlitCentered(m_animSurface);

    return 0;
};



