#include "states/MainMenuBase.h"

#include "Application.h"
#include "Gfx.h"
#include "GameData.h"
#include "GameData.h"
#include "SoundPlayer.h"

#include "gui2/Frame.h"
#include "gui2/Label.h"
#include "gui2/VBox.h"

#include <boost/bind.hpp>

MainMenuBaseState::MainMenuBaseState()
{
    m_vbox = new VBox();
    m_song = new song;
    m_song->filename = "SOUND:DUNE7.ADL";
    m_song->track = 6;
    m_middleFrame = new Frame();
    m_backgroundFrame->addChild(m_middleFrame);
    m_menuFrame = new Frame();
    m_backgroundFrame->addChild(m_menuFrame);
}

void MainMenuBaseState::draw()
{
    m_vbox->fit(2);
    m_vbox->reshape();

    ImagePtr menu(new Image(UPoint(bw + 20, (m_vbox->getChildrenSize() * 2) + (m_vbox->getChildrenSize() * bh) + 55)));
    menu->setColorKey();

    m_menuFrame->changeBackground(menu);
    ImagePtr menuTop(new Image(UPoint(bw + 20, 30)));
    menuTop->recolor(0, 236);
    menuTop->drawBorders1();

    GraphicsLabel *menuTopLabel = new GraphicsLabel(menuTop, "Doon Lunacy", 32);
    m_menuFrame->addChild(menuTopLabel);


	ImagePtr menuBottom(new Image(UPoint(bw + 20, (m_vbox->getChildrenSize() * 2) + (m_vbox->getChildrenSize() * bh) + 25)));
    menuBottom->drawBorders2();
    menuBottom->setColorKey();
    Frame *menuBottomFrame = new Frame(menuBottom);
    m_vbox->setPosition(UPoint(9,14));
    menuBottomFrame->addChild(m_vbox);

    menuBottomFrame->setPosition(UPoint(0,31));
    m_menuFrame->addChild(menuBottomFrame);

    m_harkonnenHerald = new GraphicsLabel(DataCache::Instance()->getGameData("UI_Mentat_HeraldHarkonnen")->getImage());
    m_backgroundFrame->addChild(m_harkonnenHerald);

    m_atreidesHerald = new GraphicsLabel(DataCache::Instance()->getGameData("UI_Mentat_HeraldAtreides")->getImage());
    m_backgroundFrame->addChild(m_atreidesHerald);

    m_ordosHerald = new GraphicsLabel(DataCache::Instance()->getGameData("UI_Mentat_HeraldOrdos")->getImage());
    m_backgroundFrame->addChild(m_ordosHerald);

    ImagePtr version(new Image(UPoint(100, 30)));
    version->recolor(0, 116);
    version->drawBorders2();
    m_versionLabel = new GraphicsLabel(version, VERSION, 57);
    m_backgroundFrame->addChild(m_versionLabel);

}

void MainMenuBaseState::drawSpecifics()
{
    ImagePtr planet = DataCache::Instance()->getGameData("BigPlanet")->getImage();
    planet->drawBorders1();
    m_middleFrame->changeBackground(planet);
}

void MainMenuBaseState::resize()
{
    Uint16 x = (set->GetWidth() / 2) - (m_vbox->w / 2);

    m_container->setSize(UPoint(set->GetWidth(),
		set->GetHeight()));

    ImagePtr background(new Image(UPoint(set->GetWidth(), set->GetHeight())));
    background->drawTiles(DataCache::Instance()->getGameData("UI_MenuBackground")->getImage());
    background->drawBorders3(3);
    m_backgroundFrame->changeBackground(background);

    m_middleFrame->setPosition(UPoint(x - 70, set->GetHeight()/2 - 172));

    m_menuFrame->setPosition(UPoint(x - 15, set->GetHeight()/2 + 36));


    m_harkonnenHerald->setPosition(UPoint(11, 11));
    m_atreidesHerald->setPosition(UPoint(set->GetWidth() - 66, 11));
    m_ordosHerald->setPosition(UPoint(11, set->GetHeight() - 67));

    m_versionLabel->setPosition(UPoint(set->GetWidth() - 120, set->GetHeight() - 48));

}

int MainMenuBaseState::Execute(float dt)
{
    if(m_drawMenu)
    {
        draw();
		drawSpecifics();
		m_drawMenu = false;
    }
    if(m_backgroundFrame->getSize() != UPoint(set->GetWidth(), set->GetHeight())){
        resize();
    }

    return 0;
}
