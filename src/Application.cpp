#include <boost/python.hpp>
#include <assert.h>

#include <SDL.h>
#include <SDL_mixer.h>

#include "Application.h"
#include "Colours.h"
#include "DataCache.h"
#include "FontManager.h"
#include "GCObject.h"
#include "DuneConstants.h"
#include "Gfx.h"
#include "Log.h"
#include "ResMan.h"
#include "Settings.h"
#include "SoundPlayer.h"
#include "states/TopLevelState.h"
#include "gui2/Container.h"

#include "houses.h"

Uint8 gpaloff;

Application::Application()
{
    m_running = false;
    m_rootState = NULL;

    m_clearColor = 0;
    m_cursorX = 0;
    m_cursorY = 0;
    m_cursorFrame = CURSOR_NORMAL;
    m_currentPalette = NULL;
}

Application::~Application()
{
    // TODO: not sure where this belongs... (otpetrik)
    Settings::Instance()->save();

    delete m_rootState;
    delete m_rootWidget;
    
    //FontManager::Destroy();
    //destroyDataFile();
    //MentatClass::Destroy();

    Mix_CloseAudio();
    //SDLNet_Quit();
    SDL_Quit();
}

void Application::Init()
{

    InitSettings();

    srand(time(NULL));

    Uint32 flags = SDL_INIT_AUDIO | \
                   SDL_INIT_TIMER | \
                   SDL_INIT_VIDEO | \
                   SDL_INIT_NOPARACHUTE ;
    
    if (SDL_Init(flags) < 0)
    {
        LOG_ERROR("Application", "ERROR: Couldn't initialise SDL: %s", SDL_GetError());
        //Die();
    }

    SDL_EnableUNICODE(1);

    // replace NULL with a path to a 32x32 icon
    SDL_WM_SetCaption("Doon Lunacy", "paks/DUNE2.ICO");

    InitAudio();
    //InitNet();

    // force font manager to be loaded 
    FontManager::Instance();

    //mutex_button = SDL_CreateMutex();
    //mutex_currentWidget = SDL_CreateMutex();
    //mutex_playersJoined = SDL_CreateMutex();


    m_rootWidget = new Container();
    

    houseColour[HOUSE_ATREIDES]     = COLOUR_ATREIDES;
    houseColour[HOUSE_ORDOS]        = COLOUR_ORDOS;
    houseColour[HOUSE_HARKONNEN]    = COLOUR_HARKONNEN;
    houseColour[HOUSE_SARDAUKAR]    = COLOUR_SARDAUKAR;
    houseColour[HOUSE_FREMEN]       = COLOUR_FREMEN;
    houseColour[HOUSE_MERCENARY]    = COLOUR_MERCENARY;

    //printf("loading mentat.....\n");
    //MentatClass* m = new MentatClass();
    //setMentat( m );
    //MentatClass::Instance();
    InitPython();

    Settings::Instance()->load();

    InitVideo();

    LoadData();

    //char versionString[100];
    //sprintf(versionString, "%s", VERSION);
    //menuText = TTF_RenderText_Solid(font[16], versionString, palette->colors[COLOUR_BLACK]);
    
    //init_loadWindow();

    //realign_buttons();

    m_rootState = new TopLevelState();
    //m_rootState->PushState( new MainMenuState() );
    //m_rootState->PushState( new IntroState() );
    //m_rootState->PushState( new MainMenuState() );
}

void Application::InitSettings()
{
    settings.concreteRequired = true;
    settings.gameType = ORIGINAL;
    settings.playerType = SINGLE;

    settings.gamespeed = 10;

    settings.playerNum = HOUSE_ATREIDES;

    settings.playerHouse[settings.playerNum] = settings.playerNum;

    strcpy(settings.localPlayerName, "Player");
}

void Application::InitAudio()
{

    printf("initialising sound.....\n");

    if ( Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) < 0 )
    {
        LOG_ERROR("Application","Warning: Couldn't set 22050 Hz 16-bit audio\n"
                       "- Reason: %s",SDL_GetError());
        Die();
    }
    else
    {
        LOG_INFO("Application", "Allocated %d channels.\n", Mix_AllocateChannels(16)); 
    };
}

void Application::InitNet()
{
    /*
    // TODO move to net code 
    if (SDLNet_Init() < 0)
    {
        fprintf(stderr, "ERROR: Couldn't initialise networking: %s\n", SDLNet_GetError());
        Die();
    }

    for(int i=0;i<MAX_PLAYERS;i++)
        sockets[i] = NULL;
    */
}

void Application::SetPalette()
{
    SDL_Palette * pal = DataCache::Instance()->getPalette(IBM_PAL);
    
    //This fixes white wheels. Is palette broken or sth??

    pal->colors[205].r = 109;
    pal->colors[205].g = 109;
    pal->colors[205].b = 153;
    
    assert(pal != NULL);
    LOG_INFO("Application", "Setting palette %d colors", pal->ncolors);
    assert( SDL_SetColors(m_screen->getSurface(), pal->colors, 0, pal->ncolors) == 1 );
    m_currentPalette = pal;

    SDL_Palette* palette = m_screen->getSurface()->format->palette;
    
    palette = m_currentPalette;
}

void Application::InitPython()
{
    Py_Initialize();

/*    py_main = python::import("__main__");
    py_global = new python::dict(py_main.attr("__dict__"));*/

}

void Application::InitVideo()
{
    Settings* set = Settings::Instance();
    
    int videoFlags = SDL_HWPALETTE;
    if (set->m_doubleBuffered)
        videoFlags |= SDL_HWSURFACE | SDL_DOUBLEBUF;
    if (set->m_fullscreen)
        videoFlags |= SDL_FULLSCREEN;

    SDL_Surface * surf = SDL_SetVideoMode(set->m_width, set->m_height, 8, videoFlags);
    
    if(!surf)
    {
        LOG_ERROR("Application", "Couldn't set video mode: %s", SDL_GetError());
        Die();
    };

    m_screen = new Image(surf);
    
    // reset the palette if we've got one 
    if (m_currentPalette != NULL)
        SetPalette();

    SDL_ShowCursor(SDL_DISABLE);

    m_rootWidget->setSize(UPoint(set->m_width, set->m_height));
    m_rootWidget->setPosition(UPoint(0, 0));
}

void Application::UpdateVideoMode(bool fs)
{
    Settings::Instance()->m_fullscreen = fs;
    InitVideo();
}
    
void Application::UpdateVideoMode(Uint16 w, Uint16 h)
{
    Settings::Instance()->m_width = w;
    Settings::Instance()->m_height = h;
    InitVideo();
}

void Application::UpdateVideoMode(Uint16 w, Uint16 h, bool fs)
{
    Settings::Instance()->m_width = w;
    Settings::Instance()->m_height = h;
    Settings::Instance()->m_fullscreen = fs;
    InitVideo();
}

void Application::LoadData()
{
    LOG_INFO("Application", "Loading resources");
    ResMan::Instance()->addRes("INTRO");

    DataCache::Instance()->Init();
    
    SetPalette();
    m_cursor = DataCache::Instance()->getGCObject("UI_MouseCursor")->getImage();

    LOG_INFO("Application", "Starting sound...");
    SoundPlayer::Instance();

/*	Mix_Chunk* myChunk = DataCache::Instance()->getMusic(MUSIC_INTRO, 0);
	soundPlayer->playSound(myChunk);*/
}

void Application::Die()
{
    FontManager::Destroy();
    //TTF_Quit();
    Mix_CloseAudio();
    //SDLNet_Quit();
    SDL_Quit();
    exit(1);
}

void Application::Run()
{
    Uint32 now = SDL_GetTicks();
    Uint32 then;
    // keep the frame rate down to around 30 fps
    const Uint32 min_frame_duration = 30; 

    Uint32 fps_start = now;
    int fps_frames = 0;
    const int fps_interval = 10 * 1000; // 10 seconds
    float fps;

//    Font* fnt = FontManager::Instance()->getFont("INTRO:INTRO.FNT");
    
    m_running = true;

    assert(m_rootWidget != NULL);

    gpaloff = 0 ;


    while (m_running)
    {
        SDL_FillRect(m_screen->getSurface(), NULL, m_clearColor);

        HandleEvents();
        
        then = now;
        now = SDL_GetTicks();

        #if 1
        // dont steal all the processing time 
        // FIXME: If user clicks when SDL_Delay is run, input will be ignored.
        if (now - then < min_frame_duration)
        {
            SDL_Delay(min_frame_duration -(now - then));
            now = SDL_GetTicks();
        };
        #endif
        float dt = float(now - then) / 1000.0f;

        if (m_rootState->Execute(dt) == -1) m_running = false;

        m_rootWidget->draw(m_screen, SPoint(0, 0));

        BlitCursor();
#if 0 
        fnt->render((const char*)"ABCDEFGHIJKLMOPQRSTUVWXYZ", m_screen->getSurface(), 10, 10, gpaloff);
        fnt->render((const char*)"abcdefghijklmnopqrstuvwxz", m_screen->getSurface(), 10, 30, gpaloff);

        SDL_Rect pdest = {10, 10, 5, 10};

        for (Uint32 i=0; i!=256; i++)
        {
            pdest.x = 7 * i;
            SDL_FillRect(m_screen->getSurface(), &pdest, i);
        }    
#endif
        SDL_Flip(m_screen->getSurface());

        fps_frames ++;

        if (now > fps_start + fps_interval)
        {
            fps = fps_frames / (float(now - fps_start) / 1000.0);

            printf("fps: %f\n", fps);

            fps_frames = 0;
            fps_start = now;
        };
    };
    
    LOG_INFO("Application", "done");
}

void Application::HandleEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                fprintf(stderr,"QUIT!\n");
                m_running = false;
                break;
            case SDL_MOUSEMOTION:
                m_cursorX = event.motion.x;
                m_cursorY = event.motion.y;

                m_rootWidget->handleMotion(SPoint(event.motion.x, event.motion.y)); 
                break;
            case SDL_MOUSEBUTTONDOWN:
                gpaloff++;
                m_rootWidget->handleButtonDown( event.button.button,
                                                SPoint(event.button.x, event.button.y));
                break;
            case SDL_MOUSEBUTTONUP:
                m_rootWidget->handleButtonUp(   event.button.button,
                                                SPoint(event.button.x, event.button.y));
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_F11)
                    UpdateVideoMode(!Settings::Instance()->GetFullScreen());
            
                m_rootWidget->handleKeyDown(&(event.key.keysym));
                break;
            case SDL_KEYUP:
                m_rootWidget->handleKeyUp(&(event.key.keysym));
                break;
        };
    };
}

void Application::BlitCursor()
{
    Rect src(0,0,16,16);
    UPoint dest(m_cursorX, m_cursorY);
    //src.w = surface->w / NUM_CURSORS;
    //src.h = surface->h;
    //src.x = src.w * m_cursorFrame;
    //src.y = 0;

    //reposition image so pointing on right spot

    switch (m_cursorFrame)
    {
    case CURSOR_NORMAL: src.setPosition(UPoint(0,0)); break;
    case CURSOR_UP: src.setPosition(UPoint(16,0)); break;
    case CURSOR_RIGHT: src.setPosition(UPoint(32,0)); break;
    case CURSOR_DOWN: src.setPosition(UPoint(48,0)); break;
    case CURSOR_LEFT: src.setPosition(UPoint(64,0)); break;
    case CURSOR_TARGET: src.setPosition(UPoint(80,0)); break;
    case CURSOR_SELECTION: src.setPosition(UPoint(96,0)); break;
    case NUM_CURSORS: break;
    }
    
    m_screen->blitFrom(m_cursor.get(), src, dest);
}

