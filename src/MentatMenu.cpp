#include "MentatMenu.h"

#include "Application.h"
#include "Settings.h"
#include "DataCache.h"
#include "pakfile/Cpsfile.h"
#include "boost/bind.hpp"

MentatMenuState::MentatMenuState(HOUSETYPE newHouse)
{
	house = newHouse;
	Image * mentat = DataCache::Instance()->getGuiPic(UI_MentatBackground, newHouse).get();
	m_surf.reset(mentat);
    m_surf = m_surf->getResized(2);

    m_vbox = new VBox();
/*    m_butYes = new GraphicButton(DataCache::Instance()->getGuiPic(UI_MentatYes)->getResized(2), DataCache::Instance()->getGuiPic(UI_MentatYes_Pressed)->getResized(2));
    m_vbox->addChild(m_butYes);

    m_vbox->fit(2);
	m_vbox->setPosition(UPoint(336,366));
    m_vbox->reshape();
    
    m_container->addChild(m_vbox);

	m_vbox = new VBox();
    m_butNo = new GraphicButton(DataCache::Instance()->getGuiPic(UI_MentatNo)->getResized(2), DataCache::Instance()->getGuiPic(UI_MentatNo_Pressed)->getResized(2));
    m_vbox->addChild(m_butNo);

    m_vbox->fit(2);
	m_vbox->setPosition(UPoint(480,366));
    m_vbox->reshape();
    
    m_container->addChild(m_vbox);*/

/*	
	// set up window
	SDL_Surface *surf;	
	surf = pDataManager->getUIGraphic(UI_MentatBackground,house);
	
	SetBackground(surf,false);

	int xpos = std::max(0,(screen->w - surf->w)/2);
	int ypos = std::max(0,(screen->h - surf->h)/2);
	
	SetCurrentPosition(xpos,ypos,surf->w,surf->h);
	
	SetWindowWidget(&WindowWidget);

	switch(house) {
		case HOUSE_ATREIDES:
		case HOUSE_FREMEN:
			anim = pDataManager->getAnimation(Anim_AtreidesEyes);
			eyesAnim.SetAnimation(anim);
			WindowWidget.AddWidget(&eyesAnim,Point(80,160),Point(anim->getFrame()->w,anim->getFrame()->h));

			anim = pDataManager->getAnimation(Anim_AtreidesMouth);
			mouthAnim.SetAnimation(anim);
			WindowWidget.AddWidget(&mouthAnim,Point(80,192),Point(anim->getFrame()->w,anim->getFrame()->h));

			anim = pDataManager->getAnimation(Anim_AtreidesBook);
			specialAnim.SetAnimation(anim);
			WindowWidget.AddWidget(&specialAnim,Point(145,305),Point(anim->getFrame()->w,anim->getFrame()->h));

			anim = pDataManager->getAnimation(Anim_AtreidesShoulder);
			shoulderAnim.SetAnimation(anim);
			// don't add shoulderAnim, draw it in DrawSpecificStuff	
			break;
		case HOUSE_ORDOS:
		case HOUSE_MERCENARY:
			anim = pDataManager->getAnimation(Anim_OrdosEyes);
			eyesAnim.SetAnimation(anim);
			WindowWidget.AddWidget(&eyesAnim,Point(32,160),Point(anim->getFrame()->w,anim->getFrame()->h));

			anim = pDataManager->getAnimation(Anim_OrdosMouth);
			mouthAnim.SetAnimation(anim);
			WindowWidget.AddWidget(&mouthAnim,Point(32,192),Point(anim->getFrame()->w,anim->getFrame()->h));

			anim = pDataManager->getAnimation(Anim_OrdosRing);
			specialAnim.SetAnimation(anim);
			WindowWidget.AddWidget(&specialAnim,Point(178,289),Point(anim->getFrame()->w,anim->getFrame()->h));

			anim = pDataManager->getAnimation(Anim_OrdosShoulder);
			shoulderAnim.SetAnimation(anim);
			// don't add shoulderAnim, draw it in DrawSpecificStuff	
			break;
		case HOUSE_HARKONNEN:
		case HOUSE_SARDAUKAR:
		default:
			anim = pDataManager->getAnimation(Anim_HarkonnenEyes);
			eyesAnim.SetAnimation(anim);
			WindowWidget.AddWidget(&eyesAnim,Point(64,176),Point(anim->getFrame()->w,anim->getFrame()->h));

			anim = pDataManager->getAnimation(Anim_HarkonnenMouth);
			mouthAnim.SetAnimation(anim);
			WindowWidget.AddWidget(&mouthAnim,Point(64,208),Point(anim->getFrame()->w,anim->getFrame()->h));

			anim = pDataManager->getAnimation(Anim_HarkonnenShoulder);
			shoulderAnim.SetAnimation(anim);
			// don't add shoulderAnim, draw it in DrawSpecificStuff				
			break;		
	}
	
	TextLabel.SetTextColor(255);
	TextLabel.SetVisible(false);*/
}

MentatMenuState::~MentatMenuState() {
}

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
