#include "boost/bind.hpp"

#include "Application.h"
#include "Gfx.h"
#include "DrawImage.h"
#include "MainMenuBase.h"
#include "ResMan.h"
#include "SoundPlayer.h"
#include "FontManager.h"
#include "gui2/Frame.h"
#include "gui2/Label.h"
#include "gui2/VBox.h"

#include "pakfile/Cpsfile.h"

MainMenuBaseState::MainMenuBaseState()
{
    m_vbox = new VBox();
    m_song = new song;
    m_song->filename = "SOUND:DUNE7.ADL";
    m_song->track = 6;
    m_backgroundFrame = new Frame();
    m_container->addChild(m_backgroundFrame);
}

void MainMenuBaseState::draw()
{
    m_vbox->fit(2);
    m_vbox->reshape();    
    int len;
    uint8_t *data = ResMan::Instance()->readFile("FINALE:BIGPLAN.CPS", &len);
    CpsfilePtr m_cps (new Cpsfile(data, len));

    DrawImage *planet = new DrawImage(m_cps->getPicture());
    m_cps.reset();
    planet->drawBorders1();
    m_middleFrame = new Frame1(planet);
    
    m_backgroundFrame->addChild(m_middleFrame);


    Image *menu = new Image(UPoint(bw + 20, (m_vbox->getSize() * 2) + (m_vbox->getSize() * bh) + 55));
    menu->setColorKey();

    m_menuFrame = new Frame(menu);
    DrawImage *menuTop = new DrawImage(UPoint(bw + 20, 30));
    menuTop->recolor(0, 236);
    menuTop->drawBorders1();

    GraphicsLabel *menuTopLabel = new GraphicsLabel(menuTop, "Doon Lunacy", 32);
    m_menuFrame->addChild(menuTopLabel);

    DrawImage *menuBottom = new DrawImage(UPoint(bw + 20, (m_vbox->getSize() * 2) + (m_vbox->getSize() * bh) + 25));
    menuBottom->drawBorders2();
    menuBottom->setColorKey();
    Frame *menuBottomFrame = new Frame(menuBottom);
    m_vbox->setPosition(UPoint(9,14));

    menuBottomFrame->addChild(m_vbox);
    //FIXME: not able to click on anything added as child directly to m_container..?
    m_container->addChild(m_vbox);    
    menuBottomFrame->setPosition(UPoint(0,31));
    m_menuFrame->addChild(menuBottomFrame);

    m_backgroundFrame->addChild(m_menuFrame);


    m_harkonnenHerald = new GraphicsLabel(DataCache::Instance()->getGuiPic(UI_Mentat_HeraldHarkonnen).get());
    m_backgroundFrame->addChild(m_harkonnenHerald);

    m_atreidesHerald = new GraphicsLabel(DataCache::Instance()->getGuiPic(UI_Mentat_HeraldAtreides).get());
    m_backgroundFrame->addChild(m_atreidesHerald);

    m_ordosHerald = new GraphicsLabel(DataCache::Instance()->getGuiPic(UI_Mentat_HeraldOrdos).get());
    m_backgroundFrame->addChild(m_ordosHerald);

    DrawImage *version = new DrawImage(UPoint(100, 30));
    version->recolor(0, 116);
    version->drawBorders2();
    m_versionLabel = new GraphicsLabel(version, VERSION, 57);
    m_backgroundFrame->addChild(m_versionLabel);

}

void MainMenuBaseState::resize()
{
    Uint16 x = (set->GetWidth() / 2) - (m_vbox->w / 2);

    DrawImage *background = new DrawImage(UPoint(set->GetWidth(), set->GetHeight()));
    background->drawTiles(DataCache::Instance()->getGuiPic(UI_MenuBackground).get());
    background->drawBorders3(3);
    m_backgroundFrame->changeBackground(background);

    m_middleFrame->setPosition(UPoint(x - 70, set->GetHeight()/2 - 172));

    m_menuFrame->setPosition(UPoint(x - 15, set->GetHeight()/2 + 36));

    m_harkonnenHerald->setPosition(UPoint(11, 11));
    m_atreidesHerald->setPosition(UPoint(set->GetWidth() - 66, 11));
    m_ordosHerald->setPosition(UPoint(11, set->GetHeight() - 67));

    m_versionLabel->setPosition(UPoint(set->GetWidth() - 120, set->GetHeight() - 48));

}

MainMenuBaseState::~MainMenuBaseState()
{
    delete m_vbox;
}

int MainMenuBaseState::Execute(float dt)
{
    if(m_drawMenu)
    {
        draw();
	m_drawMenu = false;
    }
    if(m_backgroundFrame->getPictureSize() != UPoint(set->GetWidth(), set->GetHeight())){
        resize();
    }

    return 0;
}
