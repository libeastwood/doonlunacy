#include <boost/bind.hpp>
#include "MentatInfoMenu.h"

#include "Application.h"
#include "Settings.h"
#include "DataCache.h"
#include "Animation.h"
#include "gui2/Label.h"
#include "gui2/Button.h"
#include <StringFile.h>
#include <string>

MentatInfoMenuState::MentatInfoMenuState(HOUSETYPE newHouse) : MentatMenuState(newHouse)
{
	m_butExit = new GraphicButton(DataCache::Instance()->getGuiPic(UI_MentatExit)->getResized(), DataCache::Instance()->getGuiPic(UI_MentatExit_Pressed)->getResized());
	m_butExit->setPosition(UPoint(200, 168).getScaled());
	m_butExit->onClick.connect(
        boost::bind(&MentatInfoMenuState::doExit, this) );    
	    m_container->addChild(m_butExit);

	drawSpecificStuff();
}

MentatInfoMenuState::~MentatInfoMenuState()
{
}

void MentatInfoMenuState::doExit()
{
    assert(mp_parent != NULL);
    PopState();
}
