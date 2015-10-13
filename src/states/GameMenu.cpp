#include <boost/bind.hpp>

#include "states/GameMenu.h"

#include "DataCache.h"
#include "GameData.h"
#include "GameData.h"
#include "Gfx.h"
#include "Settings.h"

#include "gui2/Button.h"
#include "gui2/Frame.h"
#include "gui2/Label.h"

#include "states/MentatInfoMenu.h"

GameMenuState::GameMenuState(HOUSETYPE house)
{
    m_house = house;

    m_topFrame = new Frame();
    m_messageLabel = new GraphicsLabel(ImagePtr());
    m_sideBarFrame = new Frame();
    m_radarFrame = new Frame();



}


GameMenuState::~GameMenuState()
{
    GameMan::Instance()->Clear();
}

void GameMenuState::draw()
{
    m_backgroundFrame->addChild(m_topFrame);

    m_topFrame->addChild(m_messageLabel);

    ImagePtr screen = DataCache::Instance()->getGameData("Screen")->getImage(m_house);


    m_mentatButton = new GraphicButton(DataCache::Instance()->getGameData("UI_Mentat")->getImage(m_house),
	    DataCache::Instance()->getGameData("UI_Mentat_Pressed")->getImage(m_house));
    m_mentatButton->onClick.connect(
	    boost::bind(&GameMenuState::doMentat, this) );    

    m_topFrame->addChild(m_mentatButton);

    m_optionsButton = new GraphicButton(DataCache::Instance()->getGameData("UI_Options")->getImage(m_house),
	    DataCache::Instance()->getGameData("UI_Options_Pressed")->getImage(m_house));

    m_topFrame->addChild(m_optionsButton);

    m_creditsLabel = new GraphicsLabel(DataCache::Instance()->getGameData("UI_Credits")->getImage(m_house));
    m_topFrame->addChild(m_creditsLabel);

    ImagePtr creditsCounter(new Image(UPoint(65, 15)));
    creditsCounter->blitFrom(*DataCache::Instance()->getGameData("UI_CreditsCounter")->getImage());
    uint32_t color = creditsCounter->getPixel(UPoint(0,0));
    creditsCounter->drawVLine(UPoint(64, 0), 14, color);
    m_creditsCounterLabel = new GraphicsLabel(creditsCounter);
    m_topFrame->addChild(m_creditsCounterLabel);

    ImagePtr roundThingie = DataCache::Instance()->getGameData("UI_RoundBall")->getImage(m_house);

    ImagePtr radar(new Image(UPoint(144, 118 + roundThingie->getSize().y)));
    radar->blitFrom(*roundThingie, UPoint(0, 0));
    radar->drawVBar(UPoint(1, roundThingie->getSize().y), 117);
    radar->blitFrom(*roundThingie, UPoint(0, 118));
    radar->drawHBarSmall(UPoint(roundThingie->getSize().x, 122), radar->getSize().x - 1);
    m_radarFrame->changeBackground(radar);

    m_backgroundFrame->addChild(m_radarFrame);
    m_backgroundFrame->addChild(m_sideBarFrame);
}

void GameMenuState::doMentat()
{
    mp_parent->PushState( new MentatInfoMenuState(m_house) );
}

void GameMenuState::resize()
{
    MenuBaseState::resize();

    UPoint resolution = set->GetResolution();
    ImagePtr top(new Image(UPoint(resolution.x, 56)));
    top->drawTiles(*DataCache::Instance()->getGameData("UI_MenuBackground")->getImage());
    m_topFrame->changeBackground(top);

    ImagePtr messageBox(new Image(UPoint(resolution.x, 28)));
    messageBox->recolor(0, 116);
    messageBox->drawBorders2();

    m_messageLabel->changeBackground(messageBox);
    m_messageLabel->setPosition(UPoint(0, top->getSize().y - 28));

    m_mentatButton->setPosition(UPoint(16,6));

    m_optionsButton->setPosition(UPoint(104,6));

    m_creditsLabel->setPosition(UPoint(resolution.x - 135, 7));
    
    m_creditsCounterLabel->setPosition(m_creditsLabel->getPosition() + SPoint(m_creditsLabel->getSize().x + 2, -1));


    ImagePtr screen = DataCache::Instance()->getGameData("Screen")->getImage(m_house);
    ImagePtr roundThingie(screen->getPictureCrop(Rect(240, 39, 14, 13)));

    ImagePtr sideBar(new Image(UPoint(m_radarFrame->getSize().x, resolution.y - m_radarFrame->getSize().y - m_topFrame->getSize().y)));
    sideBar->drawVBar(UPoint(1, 0), sideBar->getSize().y - 1 - roundThingie->getSize().y);
    sideBar->blitFrom(*roundThingie, UPoint(0, sideBar->getSize().y - 1 - roundThingie->getSize().y));
    
    m_sideBarFrame->changeBackground(sideBar);
    m_radarFrame->setPosition(UPoint(resolution.x - m_radarFrame->getSize().x, m_topFrame->getSize().y));
    m_sideBarFrame->setPosition(UPoint(resolution.x - m_sideBarFrame->getSize().x, m_radarFrame->getPosition().y + m_radarFrame->getSize().y));


    m_backgroundFrame->recolorChildrenByHouse(m_house);

}

int GameMenuState::Execute(float dt)
{
    if(m_drawMenu)
    {
	draw();
	m_drawMenu = false;
    }

    MenuBaseState::Execute(dt);

    GameMan::Instance()->Update(dt);

    return 0;
}

