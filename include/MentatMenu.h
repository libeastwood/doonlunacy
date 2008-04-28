#ifndef MENTATMENU_H_INCLUDED
#define MENTATMENU_H_INCLUDED

#include "MenuBase.h"
#include "houses.h"
class Label;
class AnimationLabel;
class MentatMenuState : public MenuBaseState
{
	public:
		MentatMenuState(HOUSETYPE newHouse);
		~MentatMenuState();

		void setText(std::string text);
		int Execute(float dt);
		virtual const char* GetName() { return "MentatMenuState"; }
	
	protected:
		HOUSETYPE house;
		ImagePtr m_surf;
		Label* m_textLabel;
		AnimationLabel*	m_eyesAnim;
		AnimationLabel*	m_mouthAnim;
		AnimationLabel*	m_specialAnim;
		AnimationLabel*	m_shoulderAnim;
};

#endif // MENTATMENU_H_INCLUDED

