#include "SkirmishMenu.h"

#include "Application.h"
#include "houses.h"
#include "DataCache.h"
#include "Settings.h"
#include "FontManager.h"

#include "gui2/Frame.h"

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

SkirmishMenuState::~SkirmishMenuState()
{
}


void SkirmishMenuState::drawMainImage(){
    Uint16 x = (Settings::Instance()->GetWidth() / 2) - 
                (m_vbox->w / 2);
    Frame1 housechoice(DataCache::Instance()->getGuiPic(UI_HouseChoiceBackground).get(), UPoint(320, 200));
//    housechoice.getPicture()->recolor(0, 236);

    m_menuBackground->blitFrom(housechoice.getPicture().get(), UPoint(x - 70, Settings::Instance()->GetHeight()/2 - 172));
    ImagePtr tmp;
    Frame2 difficultyBox(236, UPoint(112, 30));
    tmp = difficultyBox.getPicture();

    Font* font = FontManager::Instance()->getFont("INTRO:INTRO.FNT");
    Settings* set = Settings::Instance();
    Uint16 textw, texth;

    font->extents("Difficulty", textw, texth);
    font->render("Difficulty", tmp->getSurface(), tmp->getSize().x/2 - textw/2, tmp->getSize().y/2 - texth/2, 32);

    m_menuBackground->blitFrom(tmp.get(), UPoint(x - 150, set->GetHeight()/2 + 36));

    Frame1 missionBox((Uint32)0, UPoint(113, 58));
    tmp = missionBox.getPicture();
    font->extents("Mission", textw, texth);
    font->render("Mission", tmp->getSurface(), tmp->getSize().x/2 - textw/2, tmp->getSize().y/2 - texth/2, 32);

    m_menuBackground->blitFrom(tmp.get(), UPoint(x + 250, set->GetHeight()/2 + 36));

    

}

void SkirmishMenuState::doStart()
{
}

void SkirmishMenuState::doCancel()
{
    assert(mp_parent != NULL);
    PopState();
}

