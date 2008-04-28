#include "HouseChoiceInfoMenu.h"

#include "Application.h"
#include "Settings.h"
#include "DataCache.h"
#include "pakfile/Cpsfile.h"
#include "boost/bind.hpp"
#include "pakfile/Animation.h"
#include "gui2/Label.h"
#include "gui2/Button.h"

HouseChoiceInfoMenuState::HouseChoiceInfoMenuState(HOUSETYPE newHouse) : MentatMenuState(HOUSE_SARDAUKAR)
{
    int len;
    uint8_t * data;
	if (newHouse == HOUSE_ATREIDES)
		data = ResMan::Instance()->readFile("MENTAT:FARTR.WSA", &len);
	else if (newHouse == HOUSE_ORDOS)
		data = ResMan::Instance()->readFile("MENTAT:FORDOS.WSA", &len);
	else if (newHouse == HOUSE_HARKONNEN)
		data = ResMan::Instance()->readFile("MENTAT:FHARK.WSA", &len);
    SDL_Palette* palette = Application::Instance()->Screen()->getSurface()->format->palette;

    WsafilePtr m_wsa (new Wsafile(data, len));

	Animation* ret = m_wsa->getAnimation(0, m_wsa->getNumFrames() - 1, palette, false);
	ret->setFrameRate(12);
	
	m_planetAnimation = new AnimationLabel(ret);
	m_planetAnimation->setPosition(UPoint(256,127));
	m_container->addChild(m_planetAnimation);

    m_butYes = new GraphicButton(DataCache::Instance()->getGuiPic(UI_MentatYes)->getResized(2), DataCache::Instance()->getGuiPic(UI_MentatYes_Pressed)->getResized(2));
	m_butYes->setPosition(UPoint(336,366));
	m_container->addChild(m_butYes);

    m_butNo = new GraphicButton(DataCache::Instance()->getGuiPic(UI_MentatNo)->getResized(2), DataCache::Instance()->getGuiPic(UI_MentatNo_Pressed)->getResized(2));
	m_butNo->setPosition(UPoint(480,366));    
    m_container->addChild(m_butNo);

}

HouseChoiceInfoMenuState::~HouseChoiceInfoMenuState()
{
}
