#ifndef DUNE_MAINMENUBASE_H
#define DUNE_MAINMENUBASE_H

#include "MenuBase.h"

class VBox;
class Frame;
class GraphicsLabel;
class MainMenuBaseState : public MenuBaseState 
{
    public:
        MainMenuBaseState();
        virtual int Execute(float dt);
        virtual const char* GetName() { return "MainMenuBaseState"; }

    protected:
        VBox* m_vbox;
	Frame *m_middleFrame,
	      *m_menuFrame,
	      *m_menuBottomFrame;
	GraphicsLabel *m_versionLabel,
	      *m_harkonnenHerald,
	      *m_atreidesHerald,
	      *m_ordosHerald;
        static const int bw = 180;
        static const int bh = 20;
        void draw();
	virtual void drawSpecifics();
	virtual void resize();
};

#endif // DUNE_MAINMENU_H

