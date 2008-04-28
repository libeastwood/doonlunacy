#ifndef MENTATMENU_H_INCLUDED
#define MENTATMENU_H_INCLUDED

#include "MenuBase.h"
#include "houses.h"
#include "gui2/Button.h"
#include "gui2/VBox.h"

#include "SDL.h"

class MentatMenuState : public MenuBaseState
{
	public:
		MentatMenuState(HOUSETYPE newHouse);
		~MentatMenuState();

/*	void DrawSpecificStuff();
	
	void setText(std::string text) {
		TextLabel.SetText(text.c_str());
		TextLabel.SetVisible(true);
		TextLabel.Resize(620,120);
	}*/
		int Execute(float dt);
		virtual const char* GetName() { return "MentatMenuState"; }
	
	protected:
		HOUSETYPE house;
		ImagePtr m_surf;
        VBox* m_vbox;
//		void initMenuState(HOUSETYPE newHouse);

/*	StaticContainer	WindowWidget;
	AnimationLabel	eyesAnim;
	AnimationLabel	mouthAnim;
	AnimationLabel	specialAnim;
	AnimationLabel	shoulderAnim;
	Label			TextLabel;
*/	
};

#endif // MENTATMENU_H_INCLUDED

