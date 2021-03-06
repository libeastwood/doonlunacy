#ifndef DUNE_STATES_CUTSCENESTATE_H
#define DUNE_STATES_CUTSCENESTATE_H

#include <map>
#include <string>
#include <vector>
#include <boost/python.hpp>
#include <boost/dynamic_bitset.hpp>
#include <eastwood/FntFile.h>
#include "Definitions.h"
#include "Point.h"
#include "states/MenuBase.h"


typedef struct {
    int loopAt,
	rewindTo,
	loops,
	wait;
} loop;

class AnimationLabel;
class Frame;
class CutSceneState : public MenuBaseState 
{

    public:
	CutSceneState(std::string path);
	~CutSceneState();

	virtual const char* GetName() { return "CutSceneState"; }
	int Execute(float ft);

    private:
	void loadScene(int scene);
	void skipCutScene();
	AnimationLabel *m_animLabel;
	Frame *m_sceneFrame,
	      *m_animFrame,
	      *m_textFrame;
	eastwood::FntFile *m_font;
	ImagePtr m_lastFrame;
	SPoint m_textPosition,
	       m_animPosition;
	bool m_fadeIn,
	     m_fadeOut,
	     m_textFadeIn;
	loop *m_loop;
	int m_numAnimFrames,
		 m_totalAnimFrames,
		 m_curScene,
		 m_sceneStart,
		 m_hold,
		 m_textColor;
	std::map<int, std::string> m_textStrings;
	std::map<int, SoundPtr> m_sounds;
	std::string m_scene;
	std::vector<python::object> m_scenes;
	boost::dynamic_bitset<> m_framePlayed;
};

#endif // DUNE_STATES_CUTSCENESTATE_H
