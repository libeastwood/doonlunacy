#include "MentatMenu.h"

#include "Application.h"
#include "Settings.h"
#include "DataCache.h"
#include "pakfile/Cpsfile.h"
#include "boost/bind.hpp"
#include "gui2/Label.h"

MentatMenuState::MentatMenuState(HOUSETYPE newHouse)
{
	house = newHouse;
	Image * mentat = DataCache::Instance()->getGuiPic(UI_MentatBackground, newHouse).get();
	m_surf.reset(mentat);
    m_surf = m_surf->getResized(2);

	switch(house) {
		case HOUSE_ATREIDES:
		case HOUSE_FREMEN:
			m_eyesAnim = new AnimationLabel(DataCache::Instance()->getAnimation(Anim_AtreidesEyes));
			m_eyesAnim->setPosition(UPoint(80, 191));
			m_container->addChild(m_eyesAnim);

			m_mouthAnim = new AnimationLabel(DataCache::Instance()->getAnimation(Anim_AtreidesMouth));
			m_mouthAnim->setPosition(UPoint(80, 223));
			m_container->addChild(m_mouthAnim);

			m_specialAnim = new AnimationLabel(DataCache::Instance()->getAnimation(Anim_AtreidesBook));
			m_specialAnim->setPosition(UPoint(145, 336));
			m_container->addChild(m_specialAnim);

			break;
		case HOUSE_ORDOS:
		case HOUSE_MERCENARY:
			m_eyesAnim = new AnimationLabel(DataCache::Instance()->getAnimation(Anim_OrdosEyes));
			m_eyesAnim->setPosition(UPoint(32, 191));
			m_container->addChild(m_eyesAnim);

			m_mouthAnim = new AnimationLabel(DataCache::Instance()->getAnimation(Anim_OrdosMouth));
			m_mouthAnim->setPosition(UPoint(32, 223));
			m_container->addChild(m_mouthAnim);

			m_specialAnim = new AnimationLabel(DataCache::Instance()->getAnimation(Anim_OrdosRing));
			m_specialAnim->setPosition(UPoint(178, 320));
			m_container->addChild(m_specialAnim);

			break;
		case HOUSE_HARKONNEN:
			m_eyesAnim = new AnimationLabel(DataCache::Instance()->getAnimation(Anim_HarkonnenEyes));
			m_eyesAnim->setPosition(UPoint(64, 207));
			m_container->addChild(m_eyesAnim);

			m_mouthAnim = new AnimationLabel(DataCache::Instance()->getAnimation(Anim_HarkonnenMouth));
			m_mouthAnim->setPosition(UPoint(64, 239));
			m_container->addChild(m_mouthAnim);

			break;
		case HOUSE_SARDAUKAR:
		default:
			m_eyesAnim = new AnimationLabel(DataCache::Instance()->getAnimation(Anim_BeneGesseritEyes));
			m_eyesAnim->setPosition(UPoint(128, 190));
			m_container->addChild(m_eyesAnim);

			m_mouthAnim = new AnimationLabel(DataCache::Instance()->getAnimation(Anim_BeneGesseritMouth));
			m_mouthAnim->setPosition(UPoint(112, 223));
			m_container->addChild(m_mouthAnim);

			break;

	}
	
/*	TextLabel.SetTextColor(255);
	TextLabel.SetVisible(false);*/
}

MentatMenuState::~MentatMenuState() {
}

/*void MentatMenuState::setText(std::string text)
{
	m_textLabel.SetText(text.c_str());
	TextLabel.SetVisible(true);
	TextLabel.Resize(620,120);
}*/

int MentatMenuState::Execute(float dt)
{
    m_surf.get()->blitToScreen(SPoint(Settings::Instance()->GetWidth() / 2 - m_surf->getSurface()->w/2, 
                        Settings::Instance()->GetHeight() / 16));

    return 0;
}

/*void MentatMenu::DrawSpecificStuff() {
	Point shoulderPos;
	switch(house) {
		case HOUSE_ATREIDES:
		case HOUSE_FREMEN: {
			shoulderPos = Point(256,257) + GetPosition();
		} break;
		case HOUSE_ORDOS:
		case HOUSE_MERCENARY: {
			shoulderPos = Point(256,257) + GetPosition();
		} break;
		case HOUSE_HARKONNEN:
		case HOUSE_SARDAUKAR:
		default: {
			shoulderPos = Point(256,209) + GetPosition();
		} break;
	}
	
	shoulderAnim.Draw(screen,shoulderPos);
	TextLabel.Draw(screen,Point(10,5) + GetPosition());
}*/
