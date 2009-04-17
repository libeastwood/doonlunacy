#include "states/MentatInfoMenu.h"

#include "DataCache.h"
#include "GameData.h"

#include "gui2/Button.h"
#include "gui2/Frame.h"

#include <eastwood/StringFile.h>

#include <boost/bind.hpp>


MentatInfoMenuState::MentatInfoMenuState(HOUSETYPE newHouse) : MentatMenuState(newHouse)
{
    m_butExit = new GraphicButton(DataCache::Instance()->getGameData("UI_MentatExit")->getImage()->getResized(), DataCache::Instance()->getGameData("UI_MentatExit_Pressed")->getImage()->getResized());
    m_butExit->setPosition(UPoint(200, 168).getScaled());
    m_butExit->onClick.connect(
        boost::bind(&MentatInfoMenuState::doExit, this) );    
    m_backgroundFrame->addChild(m_butExit);
    drawSpecificStuff();
}

void MentatInfoMenuState::doExit()
{
    assert(mp_parent != NULL);
    PopState();
}
