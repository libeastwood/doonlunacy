#include "boost/bind.hpp"

#include "Application.h"
#include "Gfx.h"
#include "MainMenuBase.h"
#include "ResMan.h"
#include "Settings.h"
#include "SoundPlayer.h"
#include "Font.h"

#include "gui2/Frame.h"

MainMenuBaseState::MainMenuBaseState()
{
    m_vbox = new VBox();
}

void MainMenuBaseState::draw()
{
    m_vbox->fit(2);
    Uint16 x = (Settings::Instance()->GetWidth() / 2) - 
                (m_vbox->w / 2);
	m_vbox->setPosition(UPoint(x - 5, Settings::Instance()->GetHeight()/2 + 71));
    m_vbox->reshape();

    m_container->addChild(m_vbox);

    Frame3 backgroundFrame(0, UPoint(Settings::Instance()->GetWidth(), Settings::Instance()->GetHeight()),
            DataCache::Instance()->getGuiPic(UI_MenuBackground).get(), 3);
    m_menuBackground = backgroundFrame.getPicture();

    m_menuBackground->blitFrom(backgroundFrame.getPicture().get());
    int len;
    uint8_t *data = ResMan::Instance()->readFile("FINALE:MAPPLAN.CPS", &len);

    CpsfilePtr m_cps (new Cpsfile(data, len));

    ImagePtr tmp;
    Frame1 planet(m_cps->getPicture(), UPoint(320, 200));
    tmp = planet.getPicture();
    // Not satisfied with this recoloring, what goes wrong here..?
    tmp->recolor(COLOUR_HARKONNEN, 87);
    tmp->recolor(92, 95);
    m_menuBackground->blitFrom(tmp.get(), UPoint(x - 70, Settings::Instance()->GetHeight()/2 - 172));


    tmp.reset(new Image(UPoint(192, (m_vbox->getSize() * 2) + (m_vbox->getSize() * bh) + 44)));
    Frame1 menuTopFrame(236, UPoint(192,19));
    tmp->blitFrom(menuTopFrame.getPicture().get(), UPoint(0, 0));

    Frame2 menuFrame((Uint32)0, UPoint(192, (m_vbox->getSize() * 2) + (m_vbox->getSize() * bh) + 25));

    tmp->blitFrom(menuFrame.getPicture().get(), UPoint(0, 20));
    tmp->setColorKey();
//    tmp = menuFrame.getPicture();
    m_menuBackground->blitFrom(tmp.get(), UPoint(x - 11, Settings::Instance()->GetHeight()/2 + 36 ));

//    m_menuBackground->blitFrom(tmp.get(), UPoint(x - 11, Settings::Instance()->GetHeight()/2 + 55));

    m_menuBackground->blitFrom(DataCache::Instance()->getGuiPic(UI_Mentat_HeraldHarkonnen).get(), UPoint(11, 11));
    m_menuBackground->blitFrom(DataCache::Instance()->getGuiPic(UI_Mentat_HeraldAtreides).get(), UPoint(Settings::Instance()->GetWidth() - 66, 11));
    m_menuBackground->blitFrom(DataCache::Instance()->getGuiPic(UI_Mentat_HeraldOrdos).get(), UPoint(11, Settings::Instance()->GetHeight() - 67));

    // I'd really prefer 77, 30, but then text gets fscked up..?
    Frame2 versionBox(116, UPoint(100, 30));
    tmp = versionBox.getPicture();
    Font* font = FontManager::Instance()->getFont("INTRO:INTRO.FNT");
    Uint16 textw, texth;
    font->extents(VERSION, textw, texth);
    // And here I'd prefer actually black, but I seem to lack understanding of the palette stuff.. :/
    font->render(VERSION, tmp, tmp->getSize().x/2 - textw/2, tmp->getSize().y/2 - texth/2, 57);

    m_menuBackground->blitFrom(tmp.get(), UPoint(Settings::Instance()->GetWidth() - 120, Settings::Instance()->GetHeight() - 48));



}
MainMenuBaseState::~MainMenuBaseState()
{
    delete m_vbox;
}

int MainMenuBaseState::Execute(float dt)
{
	if(!Mix_GetMusicHookData()){
		SoundPlayer::Instance()->playMusic(MUSIC_PEACE, 10);
	}

	m_menuBackground->blitToScreen(SPoint(0, 0));
    return 0;
}
