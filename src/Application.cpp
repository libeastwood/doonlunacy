#include "Application.h"

#include "SDL.h"
//#include "SDL_ttf.h"
//#include "SDL_net.h"
//#include "SDL_mixer.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//#include "globals.h"
//#include "houses.h"
//#include "MentatClass.h"
//#include "Game.h"
//#include "Menu.h"
//#include "Net.h"
//#include "data.h"
//#include "ui.h"
//#include "SoundPlayerClass.h"
#include "Colours.h"
#include "Settings.h"
#include "Font.h"
#include "TopLevelState.h"
//#include "DataFile.h"
#include "Gfx.h"
#include "pakfile/Palette.h"
#include "pakfile/Shpfile.h"
#include "pakfile/Cpsfile.h"

#include "ResMan.h"

#include "houses.h"

#define VERSION "0.94.1"

Uint8 gpaloff;

Application::Application()
{
    m_running = false;
    m_rootState = NULL;

    m_cursorX = 0;
    m_cursorY = 0;
    m_cursorFrame = CURSOR_NORMAL;
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

    //Mix_CloseAudio();
    //SDLNet_Quit();
    //SDL_Quit();
}

void Application::Init()
{
    /*
    lookDist[0] = 10; 
    lookDist[1] = 10;
    lookDist[2] = 9;
    lookDist[3] = 9;
    lookDist[4] = 9;
    lookDist[5] = 8;
    lookDist[6] = 8;
    lookDist[7] = 7;
    lookDist[8] = 6;
    lookDist[9] = 4;
    lookDist[10] = 1;
    

    InitSettings();

    srand(time(NULL));
    */

    Uint32 flags = SDL_INIT_AUDIO | \
                   SDL_INIT_TIMER | \
                   SDL_INIT_VIDEO | \
                   SDL_INIT_NOPARACHUTE ;
    
    if (SDL_Init(flags) < 0)
    {
        fprintf(stderr, "ERROR: Couldn't initialise SDL: %s\n", SDL_GetError());
        //Die();
    }

    SDL_EnableUNICODE(1);

    // replace NULL with a path to a 32x32 icon
    SDL_WM_SetCaption("Dune Legacy", NULL);

    //InitAudio();
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
    /*
    settings.concreteRequired = true;
    settings.gameType = ORIGINAL;
    settings.playerType = SINGLE;

    settings.gamespeed = 10;

    settings.playerNum = HOUSE_ATREIDES;

    settings.playerHouse[settings.playerNum] = settings.playerNum;

    strcpy(settings.localPlayerName, "Player");
    */
}

void Application::InitAudio()
{
    /*
    printf("initialising sound.....\n");

    if ( Mix_OpenAudio(11025, MIX_DEFAULT_FORMAT, 2, 512) < 0 )
    {
        fprintf(stderr,"Warning: Couldn't set 11025 Hz 16-bit audio\n"
                       "- Reason: %s\n",SDL_GetError());
        Die();
    }
    else
    {
        printf("allocated %d channels.\n", Mix_AllocateChannels(16)); 
    };
    */
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
    int len;
    unsigned char* data = ResMan::Instance()->readFile("INTRO:INTRO.PAL", &len);
    
    Palettefile tmp (data, len);

    SDL_Palette * pal = tmp.getPalette();
    
    //This fixes white wheels. Is palette broken or sth??

    pal->colors[205].r = 109;
    pal->colors[205].g = 109;
    pal->colors[205].b = 153;
    
    assert(pal != NULL);
    printf("setting palette %d colors\n", pal->ncolors);
    assert( SDL_SetColors(m_screen->getSurface(), pal->colors, 0, pal->ncolors) == 1 );
    m_currentPalette = pal;

    SDL_Palette* palette = m_screen->getSurface()->format->palette;
    
    palette = m_currentPalette;
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
        fprintf(stderr, "ERROR: Couldn't set video mode: %s\n", SDL_GetError());
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
    printf("loading resources\n");
    ResMan::Instance()->addRes("ATRE");
    ResMan::Instance()->addRes("DUNE");
    ResMan::Instance()->addRes("ENGLISH");
    ResMan::Instance()->addRes("FINALE");
    ResMan::Instance()->addRes("HARK");
    ResMan::Instance()->addRes("HERC");
    ResMan::Instance()->addRes("INTRO");
    ResMan::Instance()->addRes("INTROVOC");
    ResMan::Instance()->addRes("MENTAT");
    ResMan::Instance()->addRes("MERC");
    ResMan::Instance()->addRes("ORDOS");
    ResMan::Instance()->addRes("SCENARIO");
    ResMan::Instance()->addRes("SOUND");
    ResMan::Instance()->addRes("VOC");
    ResMan::Instance()->addRes("XTRE");
    printf("done loading resources\n");

    SetPalette();
    int len;
    
    unsigned char * data;

    data = ResMan::Instance()->readFile("DUNE:MOUSE.SHP", &len);

    Shpfile mouse (data, len);

    m_cursor.reset(new Image(mouse.getPicture(0)));


    /*
    fprintf(stdout, "starting sound...\n");
    soundPlayer = new SoundPlayerClass();
    */
}

void Application::Die()
{
    FontManager::Destroy();
    //TTF_Quit();
    //Mix_CloseAudio();
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

    //Font* fnt = FontManager::Instance()->getFont("INTRO:INTRO.FNT");
    
    m_running = true;

    assert(m_rootWidget != NULL);

    gpaloff = 0 ;
    
    while (m_running)
    {
        SDL_FillRect(m_screen->getSurface(), NULL, m_clearColor);

        HandleEvents();

        then = now;
        now = SDL_GetTicks();

        // dont steal all the processing time 
        if (now - then < min_frame_duration)
        {
            SDL_Delay(min_frame_duration -(now - then));

            now = SDL_GetTicks();
        };
        
        float dt = float(now - then) / 1000.0f;

        if (m_rootState->Execute(dt) == -1) m_running = false;

        m_rootWidget->draw(m_screen, SPoint(0, 0));

        BlitCursor();
        
#if 0
        fnt->render((const char*)"ABCDEFGHIJKLMOPQRSTUVWXYZ", m_screen, 10, 10, gpaloff);
        fnt->render((const char*)"abcdefghijklmnopqrstuvwxz", m_screen, 10, 30, gpaloff);
#endif 

#if 0
        SDL_Rect pdest = {10, 10, 5, 10};

        for (Uint32 i=0; i!=256; i++)
        {
            pdest.x = 7 * i;
            SDL_FillRect(m_screen, &pdest, i);
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
    
    printf("done\n");
	Die();
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
    UPoint dest;
    SDL_Surface * surface  = m_cursor->getSurface(); // being lazy, rename me 

    dest.x = m_cursorX;
    dest.y = m_cursorY;
    //src.w = surface->w / NUM_CURSORS;
    //src.h = surface->h;
    //src.x = src.w * m_cursorFrame;
    //src.y = 0;

    //reposition image so pointing on right spot

    if (m_cursorFrame == CURSOR_RIGHT)
    {
        dest.x -= surface->w/2;
    }
    else if (m_cursorFrame == CURSOR_DOWN)
    {
        dest.y -= surface->w/2;
    }
    else if ((m_cursorFrame == CURSOR_TARGET) || (m_cursorFrame == CURSOR_SELECTION))
    {
        dest.x -= surface->w/2;
        dest.y -= surface->h/2;
    }

    m_screen->blitFrom(m_cursor.get(), dest);

    //SDL_BlitSurface(surface, NULL, m_screen, &dest);
}

