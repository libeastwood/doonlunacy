#include "boost/bind.hpp"

#include "Application.h"
#include "Gfx.h"
#include "MainMenuBase.h"
#include "ResMan.h"
#include "SoundPlayer.h"
#include "FontManager.h"

#include "pakfile/Cpsfile.h"
#include "gui2/Frame.h"

MainMenuBaseState::MainMenuBaseState()
{
    m_vbox = new VBox();
}

void MainMenuBaseState::draw()
{
    Font* font = FontManager::Instance()->getFont("INTRO:INTRO.FNT");
    Uint16 textw, texth;

    m_vbox->fit(2);
    Uint16 x = (set->GetWidth() / 2) - (m_vbox->w / 2);
	m_vbox->setPosition(UPoint(x - 5, set->GetHeight()/2 + 82));
    m_vbox->reshape();

    m_container->addChild(m_vbox);

    Frame3 backgroundFrame(0, UPoint(set->GetWidth(), set->GetHeight()),
            DataCache::Instance()->getGuiPic(UI_MenuBackground).get(), 3);

    m_menuBackground->blitFrom(backgroundFrame.getPicture().get());

    drawMainImage();

    ImagePtr tmp;
    tmp.reset(new Image(UPoint(bw + 20, (m_vbox->getSize() * 2) + (m_vbox->getSize() * bh) + 55)));
    Frame1 menuTopFrame(236, UPoint(bw + 20, 30));
    font->extents("Doon Lunacy", textw, texth);
    font->render("Doon Lunacy", menuTopFrame.getPicture()->getSurface(), menuTopFrame.getPicture()->getSize().x/2 - textw/2,
            menuTopFrame.getPicture()->getSize().y/2 - texth/2, 32);

    tmp->blitFrom(menuTopFrame.getPicture().get(), UPoint(0, 0));

    Frame2 menuFrame((Uint32)0, UPoint(bw + 20, (m_vbox->getSize() * 2) + (m_vbox->getSize() * bh) + 25));

    tmp->blitFrom(menuFrame.getPicture().get(), UPoint(0, 31));
    tmp->setColorKey();
    m_menuBackground->blitFrom(tmp.get(), UPoint(x - 15, set->GetHeight()/2 + 36));

    m_menuBackground->blitFrom(DataCache::Instance()->getGuiPic(UI_Mentat_HeraldHarkonnen).get(), UPoint(11, 11));
    m_menuBackground->blitFrom(DataCache::Instance()->getGuiPic(UI_Mentat_HeraldAtreides).get(), UPoint(set->GetWidth() - 66, 11));
    m_menuBackground->blitFrom(DataCache::Instance()->getGuiPic(UI_Mentat_HeraldOrdos).get(), UPoint(11, set->GetHeight() - 67));

    // I'd really prefer 77, 30, but then text gets fscked up..?
    Frame2 versionBox(116, UPoint(100, 30));
    tmp = versionBox.getPicture();
    
    font->extents(VERSION, textw, texth);
    // And here I'd prefer actually black, but I seem to lack understanding of the palette stuff.. :/
    font->render(VERSION, tmp->getSurface(), tmp->getSize().x/2 - textw/2, tmp->getSize().y/2 - texth/2, 57);

    m_menuBackground->blitFrom(tmp.get(), UPoint(set->GetWidth() - 120, set->GetHeight() - 48));



}

void MainMenuBaseState::drawMainImage(){
    int len;
    uint8_t *data = ResMan::Instance()->readFile("FINALE:BIGPLAN.CPS", &len);
    Uint16 x = (set->GetWidth() / 2) - 
                (m_vbox->w / 2);
    CpsfilePtr m_cps (new Cpsfile(data, len));

    ImagePtr tmp;
    Frame1 planet(m_cps->getPicture(), UPoint(320, 200));
    tmp = planet.getPicture();
    m_menuBackground->blitFrom(tmp.get(), UPoint(x - 70, set->GetHeight()/2 - 172));

}

MainMenuBaseState::~MainMenuBaseState()
{
    delete m_vbox;
}

int MainMenuBaseState::Execute(float dt)
{
	if(set->GetMusic() && !Mix_GetMusicHookData()){
		SoundPlayer::Instance()->playMusic(MUSIC_PEACE, 10);
	}
    if((m_menuBackground->getSize() != UPoint(set->GetWidth(), set->GetHeight()) || m_drawMenu)){
        draw();
	m_drawMenu = false;
    }

	m_menuBackground->blitToScreen(SPoint(0, 0));
    return 0;
}
