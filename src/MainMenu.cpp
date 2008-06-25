#include "boost/bind.hpp"

#include "Application.h"
#include "Editor.h"
#include "Gfx.h"
#include "MainMenu.h"
#include "OptionsMenu.h"
#include "ResMan.h"
#include "SingleMenu.h"
#include "Settings.h"
#include "SoundPlayer.h"
#include "Font.h"

#include "pakfile/Wsafile.h"
#include "gui2/Frame.h"

MainMenuState::MainMenuState()
{
    //m_menuBackground = (SDL_Surface*)(dataFile[UI_Menu].dat); 
    //m_menu = new Window();
    //m_menu->setHeight(m_menuBackground->h);
    //m_menu->setWidth(m_menuBackground->w);

    m_vbox = new VBox();

    const int bw = 180;
    const int bh = 20;

    m_butSingle = new BoringButton("Single Player");
    m_butSingle->setSize(SPoint(bw, bh));
    //m_butSingle = new GraphicButton((SDL_Surface*)(dataFile[UI_Single].dat),
    //                   (SDL_Surface*)(dataFile[UI_Single_Pressed].dat));
    m_butSingle->onClick.connect(
            boost::bind(&MainMenuState::doSingle, this) );
   
    m_vbox->addChild(m_butSingle);

    m_butMulti = new BoringButton("Multi Player");
    m_butMulti->setSize(SPoint(bw, bh));
    //m_butMulti = new GraphicButton((SDL_Surface*)(dataFile[UI_Multi].dat),
    //                   (SDL_Surface*)(dataFile[UI_Multi_Pressed].dat));
    m_butMulti->onClick.connect(
            boost::bind(&MainMenuState::doSkirmish, this) );
   
    m_vbox->addChild(m_butMulti);

    m_butMapEd = new BoringButton("Map Editor");
    m_butMapEd->setSize(SPoint(bw, bh));
    //m_butMapEd = new GraphicButton((SDL_Surface*)(dataFile[UI_MapEdit].dat),
    //                   (SDL_Surface*)(dataFile[UI_MapEdit_Pressed].dat));
    m_butMapEd->onClick.connect(
            boost::bind(&MainMenuState::doEditor, this) );
   
    m_vbox->addChild(m_butMapEd);

    m_butOptions = new BoringButton("Options");
    m_butOptions->setSize(SPoint(bw, bh));
    //m_butOptions = new GraphicButton((SDL_Surface*)(dataFile[UI_OptionsMM].dat),
    //                   (SDL_Surface*)(dataFile[UI_OptionsMM_Pressed].dat));
    m_butOptions->onClick.connect(
            boost::bind(&MainMenuState::doOptions, this) );
   
    m_vbox->addChild(m_butOptions);

    m_butAbout = new BoringButton("About");
    m_butAbout->setSize(SPoint(bw, bh));
    //m_butAbout = new GraphicButton((SDL_Surface*)(dataFile[UI_About].dat),
    //                   (SDL_Surface*)(dataFile[UI_About_Pressed].dat));
    m_butAbout->onClick.connect(
            boost::bind(&MainMenuState::doSkirmish, this) );
   
    m_vbox->addChild(m_butAbout);

    m_butQuit = new BoringButton("Quit");
    m_butQuit->setSize(SPoint(bw, bh));
    //m_butQuit = new GraphicButton((SDL_Surface*)(dataFile[UI_Quit].dat),
    //                   (SDL_Surface*)(dataFile[UI_Quit_Pressed].dat));
    m_butQuit->onClick.connect(
            boost::bind(&MainMenuState::doQuit, this) );
   
    m_vbox->addChild(m_butQuit);

    m_vbox->fit(2);
    Uint16 x = (Settings::Instance()->GetWidth() / 2) - 
                (m_vbox->w / 2);
	m_vbox->setPosition(UPoint(x - 5, 312));
    m_vbox->reshape();
    
    m_container->addChild(m_vbox);


    Frame3 backgroundFrame(0, UPoint(Settings::Instance()->GetWidth(), Settings::Instance()->GetHeight()),
            DataCache::Instance()->getGuiPic(UI_MenuBackground).get(), 3);
    m_surf = backgroundFrame.getPicture();

    m_surf->blitFrom(backgroundFrame.getPicture().get());
    int len;
    uint8_t *data = ResMan::Instance()->readFile("FINALE:MAPPLAN.CPS", &len);

    CpsfilePtr m_cps (new Cpsfile(data, len));

    ImagePtr tmp;
    Frame1 planet(m_cps->getPicture(), UPoint(320, 200));
    tmp = planet.getPicture();
    // Not satisfied with this recoloring, what goes wrong here..?
    tmp->recolor(COLOUR_HARKONNEN, 87);
    tmp->recolor(92, 95);
    m_surf->blitFrom(tmp.get(), UPoint(150, 68));


    tmp.reset(new Image(UPoint(192, 153)));
    Frame1 menuTopFrame(236, UPoint(192,19));
    tmp->blitFrom(menuTopFrame.getPicture().get(), UPoint(0, 0));
    Frame2 menuBottomFrame((Uint32)0, UPoint(192, 134));
    
    tmp->setColorKey();
    tmp->blitFrom(menuBottomFrame.getPicture().get(), UPoint(0, 20));
    m_surf->blitFrom(tmp.get(), UPoint(219, 276));

    m_surf->blitFrom(DataCache::Instance()->getGuiPic(UI_Mentat_HeraldHarkonnen).get(), UPoint(11, 11));
    m_surf->blitFrom(DataCache::Instance()->getGuiPic(UI_Mentat_HeraldAtreides).get(), UPoint(534, 11));
    m_surf->blitFrom(DataCache::Instance()->getGuiPic(UI_Mentat_HeraldOrdos).get(), UPoint(11, 413));

    // I'd really prefer 77, 30, but then text gets fscked up..?
    Frame2 versionBox(116, UPoint(100, 30));
    tmp = versionBox.getPicture();
    Font* font = FontManager::Instance()->getFont("INTRO:INTRO.FNT");
    Uint16 textw, texth;
    font->extents(VERSION, textw, texth);
    // And here I'd prefer actually black, but I seem to lack understanding of the palette stuff.. :/
    font->render(VERSION, tmp, tmp->getSize().x/2 - textw/2, tmp->getSize().y/2 - texth/2, 57);

    m_surf->blitFrom(tmp.get(), UPoint(500, 432));
}

MainMenuState::~MainMenuState()
{
    delete m_butSingle;
    delete m_butMulti;
    delete m_butMapEd;
    delete m_butOptions;
    delete m_butAbout;
    delete m_butQuit;

    delete m_vbox;
}

void MainMenuState::doEditor()
{
    mp_parent->PushState( new EditorState() );
}

void MainMenuState::doOptions()
{
    mp_parent->PushState( new OptionsMenuState() );
}

void MainMenuState::doSkirmish()
{
    printf("skirmish!\n");
    SoundPlayer::Instance()->playMusic(MUSIC_PEACE, 3);
    
#if 0
    Rect rect(0,0,30, 30);
    
    Image * img = new Image(UPoint(600, 300));
    int colour = 0;
    
    for (int j=0; j< 10; j++)
    {
        for (int i=0; i< 20; i++)
        {
            rect.x = 30 * i;
            rect.y = 30 * j;
            img->fillRect(colour, rect);
            colour++;
        }
    }
    SDL_SaveBMP(img->getSurface(), "palette.bmp");

#endif 
}

void MainMenuState::doSingle()
{
    mp_parent->PushState( new SingleMenuState() );
}

void MainMenuState::doQuit()
{
    PopState();
}

/*
void MainMenuState::JustMadeActive()
{
    Application::Instance()->RootWidget()->addChild(m_vbox);
    State::JustMadeActive();
}

void MainMenuState::JustMadeInactive()
{
    Application::Instance()->RootWidget()->deleteChild(m_vbox);
    State::JustMadeInactive();
}*/


int MainMenuState::Execute(float dt)
{
	if(!Mix_GetMusicHookData()){
		SoundPlayer::Instance()->playMusic(MUSIC_PEACE, 10);
	}
	m_surf->blitToScreen(SPoint(0, 0));//Settings::Instance()->GetWidth() / 2 - m_surf->getSurface()->w/2,
//				Settings::Instance()->GetHeight() / 4));
	return 0;
}
