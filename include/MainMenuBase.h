#ifndef DUNE_MAINMENUBASE_H
#define DUNE_MAINMENUBASE_H

#include "MenuBase.h"
#include "gui2/Button.h"
#include "gui2/VBox.h"
#include "Gfx.h"
#include "SDL.h"

class MainMenuBaseState : public MenuBaseState 
{
    public:
        MainMenuBaseState();
        ~MainMenuBaseState();
        int Execute(float dt);

    protected:
        VBox* m_vbox;
        static const int bw = 180;
        static const int bh = 20;
        void draw();
        void drawMainImage();
};

#endif // DUNE_MAINMENU_H

