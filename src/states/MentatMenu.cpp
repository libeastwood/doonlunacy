#include "states/MentatMenu.h"

#include "Application.h"
#include "DataCache.h"
#include "GameData.h"
#include "Gfx.h"
#include "SoundPlayer.h"

#include "gui2/Label.h"
#include "gui2/Frame.h"

#include <boost/bind.hpp>

MentatMenuState::MentatMenuState(HOUSETYPE newHouse)
{
	m_song = new song;

	house = newHouse;

	switch(house) {
		case HOUSE_ATREIDES:
		case HOUSE_FREMEN:
			m_backgroundFrame->changeBackground(DataCache::Instance()->getGameData("Mentat_Atreides")->getImage()->getResized());
			m_song->filename = "SOUND:DUNE7.ADL";
			m_song->track = 3;

			m_eyesAnim = DataCache::Instance()->getAnimationLabel("Anim_AtreidesEyes");
			m_eyesAnim->setPosition(UPoint(40, 80).getScaled());
			m_backgroundFrame->addChild(m_eyesAnim);

			m_mouthAnim = DataCache::Instance()->getAnimationLabel("Anim_AtreidesMouth");
			m_mouthAnim->setPosition(UPoint(40, 95).getScaled());
			m_backgroundFrame->addChild(m_mouthAnim);

			m_specialAnim = DataCache::Instance()->getAnimationLabel("Anim_AtreidesBook");
			m_specialAnim->setPosition(UPoint(72, 151).getScaled());
			m_backgroundFrame->addChild(m_specialAnim);

			m_shoulderAnim = DataCache::Instance()->getAnimationLabel("Anim_AtreidesShoulder");
			m_shoulderAnim->setPosition(UPoint(128, 128).getScaled());


			break;
		case HOUSE_ORDOS:
		case HOUSE_MERCENARY:
			m_backgroundFrame->changeBackground(DataCache::Instance()->getGameData("Mentat_Ordos")->getImage()->getResized());
			m_song->filename = "SOUND:DUNE7.ADL";
			m_song->track = 4;

			m_eyesAnim = DataCache::Instance()->getAnimationLabel("Anim_OrdosEyes");
			m_eyesAnim->setPosition(UPoint(16, 80).getScaled());
			m_backgroundFrame->addChild(m_eyesAnim);

			m_mouthAnim = DataCache::Instance()->getAnimationLabel("Anim_OrdosMouth");
			m_mouthAnim->setPosition(UPoint(16, 96).getScaled());
			m_backgroundFrame->addChild(m_mouthAnim);

			m_specialAnim = DataCache::Instance()->getAnimationLabel("Anim_OrdosRing");
			m_specialAnim->setPosition(UPoint(90, 144).getScaled());
			m_backgroundFrame->addChild(m_specialAnim);

			m_shoulderAnim = DataCache::Instance()->getAnimationLabel("Anim_OrdosShoulder");
			m_shoulderAnim->setPosition(UPoint(128,128).getScaled());


			break;
		case HOUSE_HARKONNEN:
			m_backgroundFrame->changeBackground(DataCache::Instance()->getGameData("Mentat_Harkonnen")->getImage()->getResized());
			m_song->filename = "SOUND:DUNE7.ADL";
			m_song->track = 2;

			m_eyesAnim = DataCache::Instance()->getAnimationLabel("Anim_HarkonnenEyes");
			m_eyesAnim->setPosition(UPoint(32, 88).getScaled());
			m_backgroundFrame->addChild(m_eyesAnim);

			m_mouthAnim = DataCache::Instance()->getAnimationLabel("Anim_HarkonnenMouth");
			m_mouthAnim->setPosition(UPoint(32, 104));
			m_backgroundFrame->addChild(m_mouthAnim);

			m_shoulderAnim = DataCache::Instance()->getAnimationLabel("Anim_HarkonnenShoulder");
			m_shoulderAnim->setPosition(UPoint(128, 104).getScaled());

			break;
		case HOUSE_SARDAUKAR:
		default:
			m_backgroundFrame->changeBackground(DataCache::Instance()->getGameData("Mentat_BeneGesserit")->getImage()->getResized());
			m_song->filename = "SOUND:DUNE7.ADL";
			m_song->track = 2;

			m_eyesAnim = DataCache::Instance()->getAnimationLabel("Anim_BeneGesseritEyes");
			m_eyesAnim->setPosition(UPoint(64, 80).getScaled());
			m_backgroundFrame->addChild(m_eyesAnim);

			m_mouthAnim = DataCache::Instance()->getAnimationLabel("Anim_BeneGesseritMouth");
			m_mouthAnim->setPosition(UPoint(56, 96).getScaled());
			m_backgroundFrame->addChild(m_mouthAnim);

			break;

	}
	
}

// Should needs to be drawn later, otherwise graphics will be drawn on top of it
void MentatMenuState::drawSpecificStuff() {
	if(house != HOUSE_SARDAUKAR)
		m_backgroundFrame->addChild(m_shoulderAnim);
}
