#include <boost/bind.hpp>

#include "states/SkirmishMenu.h"

#include "DataCache.h"
#include "GameData.h"
#include "Settings.h"

#include "gui2/Button.h"
#include "gui2/Counter.h"
#include "gui2/Frame.h"
#include "gui2/HScale.h"
#include "gui2/SelectionBox.h"
#include "gui2/VBox.h"


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

    m_selectionBox = new SelectionBox(false);

    m_missionLabel = new TransparentLabel("Mission");

    m_missionCounter = new Counter(1, 22, 1);
    m_butPlus = new BoringButton("+");
    m_butPlus->setSize(SPoint(10, 10));
    m_butPlus->onClick.connect(
            boost::bind(&Counter::increase, m_missionCounter) );

    m_butMinus = new BoringButton("-");
    m_butMinus->setSize(SPoint(10,10));
    m_butMinus->onClick.connect(
            boost::bind(&Counter::decrease, m_missionCounter) );

    m_missionFrame = new Frame();

    m_missionFrame->addChild(m_missionLabel);
    m_missionFrame->addChild(m_butMinus);
    m_missionFrame->addChild(m_butPlus);
    m_missionFrame->addChild(m_missionCounter);
    m_backgroundFrame->addChild(m_missionFrame);

}

void SkirmishMenuState::drawSpecifics()
{
    DataCache::Instance()->getGameData("BigPlanet")->getImage();

    ImagePtr houseChoice = DataCache::Instance()->getGameData("UI_HouseChoiceBackground")->getImage()->getCopy();
    houseChoice->drawBorders1();
    Rect area(20, 53, 300, 53+95);
    houseChoice->greyOut(area);
    m_middleFrame->changeBackground(houseChoice);
    m_selectionBox->setPosition(area.getPosition() - 1);
    m_selectionBox->setSize(area.getSize());
    m_middleFrame->addChild(m_selectionBox);

    ImagePtr atreides = DataCache::Instance()->getGameData("UI_HeraldColoredAtreides")->getImage();
    ImagePtr transparent(new Image(atreides->getSize()));

    transparent->setColorKey();
    GraphicButton *atreidesButton = new GraphicButton(transparent, atreides);
    atreidesButton->setPosition(UPoint(0, 0));
    m_selectionBox->addChild(atreidesButton);

    ImagePtr ordos = DataCache::Instance()->getGameData("UI_HeraldColoredOrdos")->getImage();
    transparent->setColorKey();
    GraphicButton *ordosButton = new GraphicButton(transparent, ordos);
    ordosButton->setPosition(UPoint(96, 0));
    m_selectionBox->addChild(ordosButton);

    ImagePtr harkonnen = DataCache::Instance()->getGameData("UI_HeraldColoredHarkonnen")->getImage();
    transparent->setColorKey();
    GraphicButton *harkonnenButton = new GraphicButton(transparent, harkonnen);
    harkonnenButton->setPosition(UPoint(194, 0));
    m_selectionBox->addChild(harkonnenButton);

    m_missionLabel->redraw();
}

void SkirmishMenuState::resize()
{
    MainMenuBaseState::resize();
    UPoint resolution = set->GetResolution();

    m_missionLabel->setPosition(SPoint(8,8));
    m_missionCounter->setPosition(m_missionLabel->getPosition() + SPoint(m_missionLabel->getSize().x + 5, 0));
    m_butPlus->setPosition(m_missionCounter->getPosition() + SPoint(m_missionCounter->getSize().x + 5, -2));
    m_butMinus->setPosition(m_missionCounter->getPosition() + SPoint(m_missionCounter->getSize().x + 5, 8));

    ImagePtr tmp(new Image(m_missionLabel->getSize() + m_missionLabel->getPosition() + SPoint(m_missionCounter->getSize().x + m_butPlus->getSize().x + 20, 5)));
    tmp->setColorKey();
    tmp->drawBorders1();

    m_missionFrame->changeBackground(tmp);

    SPoint midPos = m_menuFrame->getPosition() + SPoint(m_menuFrame->getSize().x, m_menuBottomFrame->getPosition().y);
    SPoint pos = midPos + (UPoint(resolution.x - midPos.x,m_menuBottomFrame->getSize().y)/2) - (m_missionFrame->getSize()/2);

    m_missionFrame->setPosition(pos);
}

void SkirmishMenuState::doStart()
{
}

void SkirmishMenuState::doCancel()
{
    assert(mp_parent != NULL);
    PopState();
}
