#include "boost/bind.hpp"


#include "Application.h"
#include "DataCache.h"
#include "MentatMenu.h"
#include "SoundPlayer.h"

#include "gui2/Label.h"
#include "pakfile/Cpsfile.h"
#include <iostream>

MentatMenuState::MentatMenuState(HOUSETYPE newHouse)
{
	m_song = new song;

	house = newHouse;
	m_surf = DataCache::Instance()->getGuiPic(UI_MentatBackground, newHouse)->getResized();
	switch(house) {
		case HOUSE_ATREIDES:
		case HOUSE_FREMEN:
			m_song->filename = "SOUND:DUNE7.ADL";
			m_song->track = 6;

			m_eyesAnim = new AnimationLabel(DataCache::Instance()->getAnimation("Anim_AtreidesEyes"));
			m_eyesAnim->setPosition(sizeRelativePoint(UPoint(40, 80)));
			m_container->addChild(m_eyesAnim);

			m_mouthAnim = new AnimationLabel(DataCache::Instance()->getAnimation("Anim_AtreidesMouth"));
			m_mouthAnim->setPosition((sizeRelativePoint(UPoint(40, 95))));
			m_container->addChild(m_mouthAnim);

			m_specialAnim = new AnimationLabel(DataCache::Instance()->getAnimation("Anim_AtreidesBook"));
			m_specialAnim->setPosition(sizeRelativePoint(UPoint(72, 151)));
			m_container->addChild(m_specialAnim);

			m_shoulderAnim = new AnimationLabel(DataCache::Instance()->getAnimation("Anim_AtreidesShoulder"));
			m_shoulderAnim->setPosition(sizeRelativePoint(UPoint(128, 128)));


			break;
		case HOUSE_ORDOS:
		case HOUSE_MERCENARY:
			m_song->filename = "SOUND:DUNE7.ADL";
			m_song->track = 4;

			m_eyesAnim = new AnimationLabel(DataCache::Instance()->getAnimation("Anim_OrdosEyes"));
			m_eyesAnim->setPosition(UPoint(32, 191));
			m_container->addChild(m_eyesAnim);

			m_mouthAnim = new AnimationLabel(DataCache::Instance()->getAnimation("Anim_OrdosMouth"));
			m_mouthAnim->setPosition(UPoint(32, 223));
			m_container->addChild(m_mouthAnim);

			m_specialAnim = new AnimationLabel(DataCache::Instance()->getAnimation("Anim_OrdosRing"));
			m_specialAnim->setPosition(UPoint(178, 320));
			m_container->addChild(m_specialAnim);

			m_shoulderAnim = new AnimationLabel(DataCache::Instance()->getAnimation("Anim_OrdosShoulder"));
			m_shoulderAnim->setPosition(UPoint(256,286));


			break;
		case HOUSE_HARKONNEN:
			m_song->filename = "SOUND:DUNE7.ADL";
			m_song->track = 2;

			m_eyesAnim = new AnimationLabel(DataCache::Instance()->getAnimation("Anim_HarkonnenEyes"));
			m_eyesAnim->setPosition(UPoint(64, 207));
			m_container->addChild(m_eyesAnim);

			m_mouthAnim = new AnimationLabel(DataCache::Instance()->getAnimation("Anim_HarkonnenMouth"));
			m_mouthAnim->setPosition(UPoint(64, 239));
			m_container->addChild(m_mouthAnim);

			m_shoulderAnim = new AnimationLabel(DataCache::Instance()->getAnimation("Anim_HarkonnenShoulder"));
			m_shoulderAnim->setPosition(UPoint(256,238));

			break;
		case HOUSE_SARDAUKAR:
		default:
			m_song->filename = "SOUND:DUNE7.ADL";
			m_song->track = 2;

			m_eyesAnim = new AnimationLabel(DataCache::Instance()->getAnimation("Anim_BeneGesseritEyes"));
			m_eyesAnim->setPosition(sizeRelativePoint(UPoint(64, 80)));
			m_container->addChild(m_eyesAnim);

			m_mouthAnim = new AnimationLabel(DataCache::Instance()->getAnimation("Anim_BeneGesseritMouth"));
			m_mouthAnim->setPosition(sizeRelativePoint(UPoint(56, 96)));
			m_container->addChild(m_mouthAnim);

			break;

	}
	
}

MentatMenuState::~MentatMenuState() {
}

int MentatMenuState::Execute(float dt)
{
    m_surf->blitToScreen();

    return 0;
}

// Should needs to be drawn later, otherwise graphics will be drawn on top of it
void MentatMenuState::drawSpecificStuff() {
	if(house != HOUSE_SARDAUKAR)
		m_container->addChild(m_shoulderAnim);
}
