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
#include "MainMenu.h"
#include "DataFile.h"

#define SCREEN_BPP 8
#define VERSION "0.94.1"

Application::Application()
{
    m_running = false;
    m_rootState = NULL;
    m_screen = NULL;

    m_cursorX = 0;
    m_cursorY = 0;
};

Application::~Application()
{
    delete m_rootState;
    
    FontManager::Destroy();
    destroyDataFile();
    //MentatClass::Destroy();

    //Mix_CloseAudio();
    //SDLNet_Quit();
    SDL_Quit();
};

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
        Die();
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

    //LoadData();
    loadDataFile();

    m_rootWidget = new Container();

    //get the house palettes
    /*
    houseColour[HOUSE_ATREIDES]     = COLOUR_ATREIDES;
    houseColour[HOUSE_ORDOS]        = COLOUR_ORDOS;
    houseColour[HOUSE_HARKONNEN]    = COLOUR_HARKONNEN;
    houseColour[HOUSE_SARDAUKAR]    = COLOUR_SARDAUKAR;
    houseColour[HOUSE_FREMEN]       = COLOUR_FREMEN;
    houseColour[HOUSE_MERCENARY]    = COLOUR_MERCENARY;
    */

    //printf("loading mentat.....\n");
    //MentatClass* m = new MentatClass();
    //setMentat( m );
    //MentatClass::Instance();

    InitVideo();

    char versionString[100];
    sprintf(versionString, "%s", VERSION);
    //menuText = TTF_RenderText_Solid(font[16], versionString, palette->colors[COLOUR_BLACK]);
    
    //init_loadWindow();

    //realign_buttons();

    m_rootState = new TopLevelState();
    m_rootState->PushState( new MainMenuState() );
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
};

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
};

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
};

void Application::InitVideo()
{
    Settings* set = Settings::Instance();
    
    int videoFlags = 0;
    if (set->m_doubleBuffered)
        videoFlags |= SDL_HWSURFACE | SDL_DOUBLEBUF;
    if (set->m_fullscreen)
        videoFlags |= SDL_FULLSCREEN;

    m_screen = SDL_SetVideoMode(set->m_width, set->m_height, 
                              SCREEN_BPP, videoFlags);
    if(!m_screen)
    {
        fprintf(stderr, "ERROR: Couldn't set video mode: %s\n", SDL_GetError());
        Die();
    };
    
    // set the video palette 
    // grab the palette from any image, we use menu
    SDL_Surface* menu = (SDL_Surface*)(dataFile[UI_Menu].dat);
    SDL_Palette* palette = new SDL_Palette;
    palette->ncolors = menu->format->palette->ncolors;
    palette->colors = new SDL_Color[palette->ncolors];
    memcpy(palette->colors, menu->format->palette->colors, 
            sizeof(SDL_Color) * palette->ncolors);
    SDL_SetColors(m_screen, palette->colors, 0, palette->ncolors);

    SDL_ShowCursor(SDL_DISABLE);

    m_rootWidget->setSize(set->m_width, set->m_height);
    m_rootWidget->setPos(0, 0);

    /*
    menu = (SDL_Surface*)(dataFile[UI_Menu].dat);
    backgroundOffsetX = (screen->w - menu->w)/2;
    backgroundOffsetY = (screen->h - menu->h)/2;

    //resize all buttons
    main_vbox.setX(screen->w/3);
    main_vbox.setWidth(screen->w/3);
    main_vbox.setY(screen->h/2 + 32);
    main_vbox.setHeight(screen->h - main_vbox.getY() - 10);
    main_vbox.resizeChildren();

    single_hbox.setX(10);

    single_hbox.setWidth(screen->w - 20);

    single_hbox.setY(10);

    single_hbox.setHeight(screen->h - 20);
    single_hbox.resizeChildren();

    custom_hbox.setX(10);

    custom_hbox.setWidth(screen->w - 20);

    custom_hbox.setY(10);

    custom_hbox.setHeight(screen->h - 20);
    custom_hbox.resizeChildren();

    multi_hbox.setX(10);

    multi_hbox.setWidth(screen->w - 20);

    multi_hbox.setY(10);

    multi_hbox.setHeight(screen->h - 20);
    multi_hbox.resizeChildren();

    multiServer_hbox.setX(10);

    multiServer_hbox.setWidth(screen->w - 20);

    multiServer_hbox.setY(10);

    multiServer_hbox.setHeight(screen->h - 20);
    multiServer_hbox.resizeChildren();

    multiClient_hbox.setX(10);

    multiClient_hbox.setWidth(screen->w - 20);

    multiClient_hbox.setY(10);

    multiClient_hbox.setHeight(screen->h - 20);
    multiClient_hbox.resizeChildren();

    options_vbox.setX(10);

    options_vbox.setWidth(screen->w - 20);

    options_vbox.setY(10);

    options_vbox.setHeight(screen->h - 20);
    options_vbox.resizeChildren();

    about_window.setX(10);

    about_window.setWidth(screen->w - 20);

    about_window.setY(10);

    about_window.setHeight(screen->h - 20);

    //fixDisplayFormat();
    //oscaleBackgroundScreens();
    //scaleUnits();
    */
};

void Application::LoadData()
{
    /*
    fprintf(stdout, "loading data.....\n");
    loadDataFile();

    fprintf(stdout, "starting sound...\n");
    soundPlayer = new SoundPlayerClass();

    fprintf(stdout, "starting main menu.......\n");

    menu = (SDL_Surface*)(dataFile[UI_Menu].dat);
    palette = new SDL_Palette;
    palette->ncolors = menu->format->palette->ncolors;
    palette->colors = new SDL_Color[palette->ncolors];

    //i suspect the palette on one of the images is corrupt...so we have to redefine white
    palette->colors[COLOUR_WHITE].r = 255;
    palette->colors[COLOUR_WHITE].g = 255;
    palette->colors[COLOUR_WHITE].b = 255;

    // looks like this should be a function
    memcpy(palette->colors, menu->format->palette->colors, sizeof(SDL_Color)*palette->ncolors);
    */
};

void Application::Die()
{
    FontManager::Destroy();
    //TTF_Quit();
    //Mix_CloseAudio();
    //SDLNet_Quit();
    SDL_Quit();
    exit(1);
};

void Application::Run()
{
    Uint32 now = SDL_GetTicks();
    Uint32 then;

    Uint32 fps_start = now;
    int fps_frames = 0;
    const int fps_interval = 10 * 1000; // 10 seconds
    float fps;
    
    m_running = true;

    assert(m_rootWidget != NULL);
    
    while (m_running)
    {
        SDL_FillRect(m_screen, NULL, COLOUR_BLACK);

        HandleEvents();

        then = now;
        now = SDL_GetTicks();
        float dt = float(then - now) / 1000.0f;

        if (m_rootState->Execute(dt) == -1) m_running = false;

        m_rootWidget->draw(m_screen);

        BlitCursor();
            
        SDL_Flip(m_screen);

        fps_frames ++;

        if (now > fps_start + fps_interval)
        {
            fps = fps_frames / (float(now - fps_start) / 1000.0);

            printf("fps: %f\n", fps);

            SDL_Delay(100);

            fps_frames = 0;
            fps_start = now;
        };
    };
    
    printf("done\n");
};

void Application::HandleEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                m_running = false;
                break;
            case SDL_MOUSEMOTION:
                m_cursorX = event.motion.x;
                m_cursorY = event.motion.y;

                m_rootWidget->handleMotion(event.motion.x, event.motion.y); 
                break;
            case SDL_MOUSEBUTTONDOWN:
                m_rootWidget->handleButtonDown( event.button.button,
                                                event.button.x,
                                                event.button.y);
                break;
            case SDL_MOUSEBUTTONUP:
                m_rootWidget->handleButtonUp(   event.button.button,
                                                event.button.x,
                                                event.button.y);
                break;
            case SDL_KEYDOWN:
                m_rootWidget->handleKeyDown(&(event.key.keysym));
                break;
            case SDL_KEYUP:
                m_rootWidget->handleKeyUp(&(event.key.keysym));
                break;
        };
    };
};

void Application::BlitCursor()
{
    SDL_Rect dest, src;
    SDL_Surface* surface = (SDL_Surface*)dataFile[UI_Cursor].dat;

    dest.x = m_cursorX;
    dest.y = m_cursorY;
    src.w = surface->w / NUM_CURSORS;
    src.h = surface->h;
    src.x = src.w * m_cursorFrame;
    src.y = 0;

    //reposition image so pointing on right spot

    if (m_cursorFrame == CURSOR_RIGHT)
    {
        dest.x -= dest.w/2;
    }
    else if (m_cursorFrame == CURSOR_DOWN)
    {
        dest.y -= dest.h/2;
    }
    else if ((m_cursorFrame == CURSOR_TARGET) || (m_cursorFrame == CURSOR_SELECTION))
    {
        dest.x -= dest.w/2;
        dest.y -= dest.h/2;
    }

    SDL_BlitSurface(surface, &src, m_screen, &dest);
};


void Application::Blit(SDL_Surface* surface, SDL_Rect* src, SDL_Rect* dest)
{
    SDL_BlitSurface(surface, src, m_screen, dest);
};

void Application::BlitCentered(SDL_Surface* surface, SDL_Rect* src)
{
    SDL_Rect dest;
    if (src == NULL)
    {
        dest.x = (Settings::Instance()->m_width / 2) - (surface->w / 2);
        dest.y = (Settings::Instance()->m_height / 2) - (surface->h / 2);
    }
    else
    {
        dest.x = (Settings::Instance()->m_width / 2) - (src->w / 2);
        dest.y = (Settings::Instance()->m_height / 2) - (src->h / 2);
    };
    
    Blit(surface, src, &dest);
};


