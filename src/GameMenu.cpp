#include "Settings.h"
#include "Editor.h"
#include "MapGenerator.h"
#include "MapClass.h"
#include "ResMan.h"
#include "pakfile/Cpsfile.h"
#include "gui2/Frame.h"
#include "gui2/VBox.h"
#include "gui2/Button.h"
#include "DataCache.h"

GameMenuState::GameMenuState()
{
    m_mapWidget = new MapWidget();
    m_mapWidget->setPosition(UPoint(0,56));
    m_mapWidget->setSize(UPoint(set->GetWidth() - 135, set->GetHeight() - 56));
    mentatBox = new VBox();
    optionsBox = new VBox();

    drawMenu();
}


GameMenuState::~GameMenuState()
{

}

void GameMenuState::drawMenu()
{
    int len;
    uint8_t *data;
    data = ResMan::Instance()->readFile("DUNE:SCREEN.CPS", &len);
    CpsfilePtr cps(new Cpsfile(data, len));

    ImagePtr screen(cps->getPicture());
    ImagePtr tmp(screen->getPictureCrop(Rect(241, 40, 13, 32)));
    ImagePtr sidebar(new Image(UPoint(135, set->GetHeight() - 56)));
    sidebar->blitFrom(tmp.get(), UPoint(0, 0));
    tmp.reset(screen->getPictureCrop(Rect(241, 72, 13, 13))); // bar
    for(int i = 32; i <= 140 - 13; i += 13)
	    sidebar->blitFrom(tmp.get(), UPoint(0, i));
    for(int i = 157; i <= set->GetHeight(); i += 13)
	    sidebar->blitFrom(tmp.get(), UPoint(0, i));

    tmp.reset(screen->getPictureCrop(Rect(241, 118, 13, 32))); // bar-mid
    sidebar->blitFrom(tmp.get(), UPoint(0, 125));
    tmp.reset(screen->getPictureCrop(Rect(254, 127, 9, 5)));
    sidebar->blitFrom(tmp.get(), UPoint(0 + 13, 133));
    tmp.reset(screen->getPictureCrop(Rect(264, 127, 10, 5)));
    for(int i = 22; i <= set->GetWidth(); i += 9)
	    sidebar->blitFrom(tmp.get(), UPoint(i, 133));
    tmp.reset(screen->getPictureCrop(Rect(315, 127, 5, 5)));
    sidebar->blitFrom(tmp.get(), UPoint(139, 133));
    m_menuBackground->blitFrom(sidebar.get(), UPoint(set->GetWidth() - 135, 56));

    sidebar.reset(new Image(UPoint(set->GetWidth(), 56)));
    for(int i = 0; i <= set->GetWidth(); i += 150){
	    sidebar->blitFrom(DataCache::Instance()->getGuiPic(UI_BlankFiller).get(), UPoint(i, 0));
	    sidebar->blitFrom(DataCache::Instance()->getGuiPic(UI_BlankFiller).get(), UPoint(i, 15));
    }
    tmp.reset(screen->getPictureCrop(Rect(0, 0, 183, 16)));
    sidebar->blitFrom(tmp.get(), UPoint(0, 5));
    Frame2 versionBox(116, UPoint(set->GetWidth(), 28));
    sidebar->blitFrom(versionBox.getPicture().get(), UPoint(0, 26));
    tmp.reset(screen->getPictureCrop(Rect(201, 0, 119, 16)));
    sidebar->blitFrom(tmp.get(), UPoint(set->GetWidth() - 135, 5));
    m_menuBackground->blitFrom(sidebar.get(), UPoint(0, 0));


    // create Mentat button
    ImagePtr mentat = ImagePtr(screen->getPictureCrop(Rect(16, 1, 80, 16)));
    ImagePtr mentatPressed = ImagePtr(screen->getPictureCrop(Rect(16, 1, 80, 16)));
    mentatPressed->recolor(115, 116);
    mentatPressed->recolor(108, 226);
    mentatPressed->recolor(109, 110);
    mentatPressed->recolor(147, 146);
    mentatPressed->putPixel(UPoint(0,15), 116);    

    mentatPressed->drawHLine(UPoint(1,15), 79, 114);
    mentatPressed->drawVLine(UPoint(79,1), 14, 114);

    GraphicButton *mentatButton = new GraphicButton(mentat, mentatPressed);
    mentatBox->addChild(mentatButton);
    mentatBox->fit(2);

    mentatBox->setPosition(UPoint(16,6));
    mentatBox->reshape();
    m_container->addChild(mentatBox);

    // create Options button
    ImagePtr options = ImagePtr(screen->getPictureCrop(Rect(104, 1, 80, 16)));
    ImagePtr optionsPressed = ImagePtr(screen->getPictureCrop(Rect(104, 1, 80, 16)));
    optionsPressed->recolor(115, 116);
    optionsPressed->recolor(108, 226);
    optionsPressed->recolor(109, 110);
    optionsPressed->recolor(147, 146);
    optionsPressed->putPixel(UPoint(0,15), 116);    

    optionsPressed->drawHLine(UPoint(1,15), 79, 114);
    optionsPressed->drawVLine(UPoint(79,1), 14, 114);

    GraphicButton *optionsButton = new GraphicButton(options, optionsPressed);
    optionsBox->addChild(optionsButton);
    optionsBox->fit(2);

    optionsBox->setPosition(UPoint(104,6));
    optionsBox->reshape();
    m_container->addChild(optionsBox);


    screen.reset();
    sidebar.reset();	    
    tmp.reset();
}
int GameMenuState::Execute(float dt)
{
   m_menuBackground->blitToScreen(SPoint(0, 0));

   SDL_Event event;
   SDL_PollEvent(&event);
   if (event.type == SDL_KEYDOWN) {
	   switch (event.key.keysym.sym) {
		case SDLK_ESCAPE : PopState(); break;
		default : break;
	   }
   }
    return 0;
}

