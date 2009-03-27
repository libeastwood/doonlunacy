#include "states/GameMenu.h"

#include "DataCache.h"
#include "GCObject.h"

#include "gui2/Button.h"
#include "gui2/Frame.h"
#include "gui2/Label.h"

#include "states/MentatInfoMenu.h"

#include <boost/bind.hpp>

GameMenuState::GameMenuState(HOUSETYPE house)
{
    m_house = house;
}


GameMenuState::~GameMenuState()
{
    GameMan::Instance()->Clear();
}

void GameMenuState::drawMenu()
{
    ImagePtr top(new Image(UPoint(set->GetWidth(), m_mapWidget->getPosition().y)));
    top->drawTiles(DataCache::Instance()->getGCObject("UI_MenuBackground")->getImage());
    Frame *topFrame = new Frame(top);
    m_container->addChild(topFrame);

    ImagePtr messageBox(new Image(UPoint(set->GetWidth(), 28)));
    messageBox->recolor(0, 116);
    messageBox->drawBorders2();
    GraphicsLabel *messageLabel = new GraphicsLabel(messageBox);
    messageLabel->setPosition(UPoint(0, top->getSize().y - 28));
    topFrame->addChild(messageLabel);

    ImagePtr screen = DataCache::Instance()->getGCObject("Screen")->getImage(m_house);


    m_mentatButton = new GraphicButton(DataCache::Instance()->getGCObject("UI_Mentat")->getImage(m_house),
	    DataCache::Instance()->getGCObject("UI_Mentat_Pressed")->getImage(m_house));
    m_mentatButton->onClick.connect(
	    boost::bind(&GameMenuState::doMentat, this) );    

    m_mentatButton->setPosition(UPoint(16,6));
    topFrame->addChild(m_mentatButton);

    m_optionsButton = new GraphicButton(DataCache::Instance()->getGCObject("UI_Options")->getImage(m_house),
	    DataCache::Instance()->getGCObject("UI_Options_Pressed")->getImage(m_house));

    m_optionsButton->setPosition(UPoint(104,6));
    topFrame->addChild(m_optionsButton);

    ImagePtr credits(screen->getPictureCrop(Rect(201, 2, 54, 12)));
    GraphicsLabel *creditsLabel = new GraphicsLabel(credits);
    creditsLabel->setPosition(UPoint(set->GetWidth() - 135, 7));
    topFrame->addChild(creditsLabel);

    ImagePtr creditsCounter(new Image(UPoint(65, 15)));
    creditsCounter->blitFrom(screen->getPictureCrop(Rect(256, 1, 64, 15)));
    Uint32 color = creditsCounter->getPixel(UPoint(0,0));
    creditsCounter->drawVLine(UPoint(64, 0), 14, color);
    GraphicsLabel *creditsCounterLabel = new GraphicsLabel(creditsCounter);
    creditsCounterLabel->setPosition(creditsLabel->getPosition() + SPoint(creditsLabel->getSize().x + 2, -1));
    topFrame->addChild(creditsCounterLabel);

    ImagePtr sideBar(new Image(UPoint(set->GetWidth() - m_mapWidget->getSize().x, set->GetHeight() - m_mapWidget->getPosition().y)));
    ImagePtr roundThingie(screen->getPictureCrop(Rect(240, 39, 14, 13)));
    sideBar->blitFrom(roundThingie.get(), UPoint(0, 0));
    sideBar->drawVBar(UPoint(1, roundThingie->getSize().y), 117);
    sideBar->blitFrom(roundThingie.get(), UPoint(0, 118));
    sideBar->drawHBarSmall(UPoint(roundThingie->getSize().x, 122), sideBar->getSize().x - 1);
    sideBar->drawVBar(UPoint(1, 118 + roundThingie->getSize().y), sideBar->getSize().y - 1 - roundThingie->getSize().y);
    sideBar->blitFrom(roundThingie.get(), UPoint(0, sideBar->getSize().y - 1 - roundThingie->getSize().y));

    Frame *sideBarFrame = new Frame(sideBar);
    sideBarFrame->setPosition(UPoint(m_mapWidget->getSize().x, m_mapWidget->getPosition().y));
    m_container->addChild(sideBarFrame);
    m_container->recolorChildrenByHouse(m_house);
}

void GameMenuState::doMentat()
{
    mp_parent->PushState( new MentatInfoMenuState(HOUSE_ATREIDES) );
}

int GameMenuState::Execute(float dt)
{
    if(m_drawMenu)
    {
	drawMenu();
	m_drawMenu = false;
    }

    GameMan::Instance()->Update(dt);

    return 0;
}

