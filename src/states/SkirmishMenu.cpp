#include "states/SkirmishMenu.h"

#include "DataCache.h"
#include "GameData.h"

#include "gui2/Button.h"
#include "gui2/Frame.h"
#include "gui2/VBox.h"

#include <boost/bind.hpp>


SkirmishMenuState::SkirmishMenuState() : MainMenuBaseState()
{
    m_butStart = new BoringButton("Start");
    m_butStart->setSize(SPoint(bw, bh));
    m_butStart->onClick.connect(
            boost::bind(&SkirmishMenuState::doStart, this) );
    m_vbox->addChild(m_butStart);

    m_butCancel = new BoringButton("Cancel");
    m_butCancel->setSize(SPoint(bw, bh));
    m_butCancel->onClick.connect(
            boost::bind(&SkirmishMenuState::doCancel, this) );
    m_vbox->addChild(m_butCancel);

}

void SkirmishMenuState::drawSpecifics()
{
    DataCache::Instance()->getGameData("BigPlanet")->getImage();

    ImagePtr houseChoice = DataCache::Instance()->getGameData("UI_HouseChoiceBackground")->getImage()->getCopy();
    houseChoice->drawBorders1();
    houseChoice->greyOut(Rect(20, 53, 300, 53+95));
    m_middleFrame->changeBackground(houseChoice);

    ImagePtr atreides = DataCache::Instance()->getGameData("UI_HeraldColoredAtreides")->getImage();
    ImagePtr transparent(new Image(atreides->getSize()));

    transparent->setColorKey();
    GraphicButton *atreidesButton = new GraphicButton(transparent, atreides);
    atreidesButton->setPosition(UPoint(19, 52));
    m_middleFrame->addChild(atreidesButton);

    ImagePtr ordos = DataCache::Instance()->getGameData("UI_HeraldColoredOrdos")->getImage();
    transparent->setColorKey();
    GraphicButton *ordosButton = new GraphicButton(transparent, ordos);
    ordosButton->setPosition(UPoint(115, 52));
    m_middleFrame->addChild(ordosButton);

    ImagePtr harkonnen = DataCache::Instance()->getGameData("UI_HeraldColoredHarkonnen")->getImage();
    transparent->setColorKey();
    GraphicButton *harkonnenButton = new GraphicButton(transparent, harkonnen);
    harkonnenButton->setPosition(UPoint(213, 52));
    m_middleFrame->addChild(harkonnenButton);

}

void SkirmishMenuState::doStart()
{
}

void SkirmishMenuState::doCancel()
{
    assert(mp_parent != NULL);
    PopState();
}
