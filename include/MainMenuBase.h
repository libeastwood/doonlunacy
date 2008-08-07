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
        ~MainMenuBaseState();
        int Execute(float dt);
        virtual const char* GetName() { return "MainMenuBaseState"; }

    protected:
        VBox* m_vbox;
	Frame *m_backgroundFrame,
	      *m_middleFrame,
	      *m_menuFrame;
	GraphicsLabel *m_versionLabel,
	      *m_harkonnenHerald,
	      *m_atreidesHerald,
	      *m_ordosHerald;
        static const int bw = 180;
        static const int bh = 20;
        void draw();
	virtual void drawSpecifics();
	void resize();
};

#endif // DUNE_MAINMENU_H

