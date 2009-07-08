#include <boost/bind.hpp>
#include <eastwood/CpsFile.h>
#include <eastwood/WsaFile.h>

#include "states/CutSceneState2.h"

#include "DataCache.h"
#include "FontManager.h"
#include "GameData.h"
#include "PythonObjects.h"
#include "ResMan.h"
#include "SoundPlayer.h"
#include "Sfx.h"

#include "gui2/Button.h"
#include "gui2/Frame.h"
#include "gui2/Label.h"

// ------------------------------------------------------------------
// CutSceneState

CutSceneState::CutSceneState(std::string scene) : m_scene(scene)
{
    UPoint resolution = set->GetResolution();

    try  {
	python::object cutscenes = python::import("cutscenes");
	m_scenes = getPyObjectVector<python::object>(cutscenes.attr("intro"));
    }
    catch(python::error_already_set const &) {
	LOG(LV_FATAL, "CutSceneState", "Error loading data: %s", m_scene.c_str());
	PyErr_Print();
	exit(EXIT_FAILURE);
    }
    ImagePtr background(new Image(resolution));
    m_backgroundFrame->changeBackground(background);
    m_sceneFrame = NULL;
    m_curScene = 0;
    m_drawMenu = true;
    m_font = FontManager::Instance()->getFont("INTRO:INTRO.FNT");
    m_loop = NULL;
    TransparentButton *m_skipButton = new TransparentButton(resolution);
    m_skipButton->setSize(resolution);
    m_skipButton->setPosition(UPoint(0,0));
    m_skipButton->onClick.connect(
            boost::bind(&CutSceneState::skipCutScene, this) );
    m_backgroundFrame->addChild(m_skipButton);

}

CutSceneState::~CutSceneState()
{
    if(m_loop)
	delete m_loop;
}

void CutSceneState::skipCutScene()
{
    LOG(LV_INFO, "CutSceneState", "Skipping cutscene: '%S'", &(ConstString)m_scene);

    m_curScene = -1;
}

void CutSceneState::loadScene(int scene)
{
    UPoint resolution = set->GetResolution();
    if(m_sceneFrame != NULL)
	m_backgroundFrame->deleteChild(m_sceneFrame);
    m_sceneFrame = new Frame(ImagePtr(new Image(resolution)));
    m_backgroundFrame->addChild(m_sceneFrame);
    m_animFrame = new Frame();
    m_sceneFrame->addChild(m_animFrame);
    m_textFrame = new Frame(ImagePtr(new Image(UPoint(300,50).getScaled())));
    m_sceneFrame->addChild(m_textFrame);
    std::string filename, palettefile;
    if(m_loop)
	free(m_loop);
    m_loop = NULL;
    int song,
	loopAnimFrames = 0;
    float fps;
    bool continuation;
    m_sounds.clear();
    m_textStrings.clear();

    try     {
	python::object curScene = m_scenes[scene];
	getPyObject<std::string>(curScene.attr("palette"), &palettefile);
	getPyObject(curScene.attr("hold"), &m_hold);
	getPyObject(curScene.attr("textColor"), &m_textColor);
	getPyObject(curScene.attr("continuation"), &continuation);
	getPyObject(curScene.attr("fps"), &fps);
	getPyObject(curScene.attr("fadeIn"), &m_fadeIn);
	getPyObject(curScene.attr("fadeOut"), &m_fadeOut);
	getPyObject(curScene.attr("textFadeIn"), &m_textFadeIn);

	if(getPyObject(curScene.attr("song"), &song))
	    SoundPlayer::Instance()->playMusic(MUSIC_INTRO, song);

	Rect rectLoop;
	if(getPyObject(curScene.attr("loop"), &rectLoop)) {
	    m_loop = new loop;
	    m_loop->loopAt = rectLoop.x;
	    m_loop->rewindTo = rectLoop.y;
	    m_loop->loops = rectLoop.w;
	    m_loop->wait = rectLoop.h;
	    loopAnimFrames = (m_loop->loopAt - m_loop->rewindTo) * m_loop->loops + m_loop->wait;
	}

	if(!nonePyObject(curScene.attr("text"))) {
	    std::vector<python::object> textStrings = getPyObjectVector<python::object>(curScene.attr("text"));
	    for (int i = textStrings.size()-1; i >= 0; i--) {
		int time = python::extract<int>(textStrings[i][0]);
		std::string text;
		if(getPyObjectType(textStrings[i][1], 0) == "int")
		    text = DataCache::Instance()->getIntroString(python::extract<int>(textStrings[i][1]));
		else
		    text = python::extract<std::string>(textStrings[i][1]);
		m_textStrings[time] = text;
	    }
	}
	if(!nonePyObject(curScene.attr("sound"))) {
	    python::dict sounds = python::extract<python::dict>(curScene.attr("sound"));
	    python::list keys = sounds.keys();
	    keys.sort();
	    while(keys) {
		int time = python::extract<int>(keys.pop());
		std::string type = getPyObjectType(sounds[time], 0);
		SoundPtr snd;
		if(type == "tuple") {
		    python::tuple soundObj = python::extract<python::tuple>(sounds[time]);
		    snd.reset(new Sound());
		    for(int i = 0; i < python::len(soundObj); i++) {
			std::string sound = getPyObjectType((soundObj[i]), 0);
			SoundPtr tmp = DataCache::Instance()->getGameData(sound)->getSound();
			snd->concatSound(tmp);
		    }
		}
		else
		    snd = DataCache::Instance()->getGameData(type)->getSound();

		m_sounds[time] = snd;
	    }
	}
	m_textPosition = python::extract<SPoint>(curScene.attr("textPosition"));
	m_textFrame->setPosition(m_textPosition.getScaled() + SPoint(0, m_backgroundFrame->getSize().y /2 - m_textFrame->getSize().y/2));

	m_animPosition = python::extract<SPoint>(curScene.attr("animPosition"));

	m_animLabel = new AnimationLabel();
	ImagePtr animFrame;
	if(!getPyObject(curScene.attr("filename"), &filename))
	    m_animLabel->addFrame(ImagePtr(new Image(UPoint(1,1))), true);
	else {
	    size_t len;
	    uint8_t *data = ResMan::Instance()->readFile(filename, &len);
	    if(filename.substr(filename.length()-3, 3) == "CPS") {
		CpsFile cpsfile(data, len);
		m_animLabel->addFrame(ImagePtr(new Image(cpsfile.getSurface())), true);
	    }
	    else {
		WsaFile wsafile(data, len, DataCache::Instance()->getPalette(palettefile), continuation ? m_lastFrame.get() : NULL);
		std::vector<ImagePtr> wsaFrames;
		for(uint32_t i = 0; i < wsafile.getNumFrames(); i++)
		    wsaFrames.push_back(ImagePtr(new Image(wsafile.getSurface(i))));

		for(uint32_t i = 0; i < wsaFrames.size() + loopAnimFrames; i++)
		    if(i < wsaFrames.size())
		    {
			m_animLabel->addFrame(wsaFrames[i], true);
			if(m_loop && (int)i == m_loop->loopAt && m_loop->loops > -1)
			{
			    if(m_loop->wait > 0)
				m_animLabel->addPause(m_loop->wait), m_loop->wait=0;
			    i = m_loop->rewindTo, m_loop->loops--;
			}
		    }
		m_lastFrame = wsaFrames.back();
	    }
	}
	m_animLabel->addPause(m_hold);

	m_animLabel->setFrameRate(fps);

	SPoint pos = (m_backgroundFrame->getSize() /2) - m_animLabel->getSize()/2 + m_animPosition.getScaled();
	// Don't allow picture picture to be placed outside of screen
	if(pos.x < 0) pos.x = 0;
	if(pos.y < 0) pos.y = 0;
	m_animLabel->setPosition(pos);

	m_sceneFrame->addChild(m_animLabel);

    }
    catch(python::error_already_set const &) {
	LOG(LV_FATAL, "CutSceneState", "Error loading data: %s", m_scene.c_str());
	PyErr_Print();
	exit(EXIT_FAILURE);
    }
    m_sceneStart = SDL_GetTicks();
    m_framePlayed.resize(m_animLabel->getNumFrames());
}

int CutSceneState::Execute(float ft)
{
    Uint32 curFrame;

    if(m_curScene == -1)
	return -1;

    if(m_drawMenu)
    {
	loadScene(m_curScene);
	m_drawMenu = false;
    }
    curFrame = m_animLabel->getCurFrame();

    if(curFrame == 0 && m_fadeIn)
	m_fadeIn = m_backgroundFrame->fadeInChildren();

    if(curFrame == m_animLabel->getNumFrames() - 1)
    {
	if(m_fadeOut)
	    m_fadeOut = m_backgroundFrame->fadeOutChildren();
	else
	{
	    LOG(LV_INFO, "CutSceneState", "Scene %d playtime: %f s", m_curScene, (SDL_GetTicks()- m_sceneStart)/1000.f);
	    m_curScene++;
	    if(m_loop != NULL)
	    {
		delete m_loop;
		m_loop = NULL;
	    }
	    if(m_curScene == (int)m_scenes.size())
		return -1;
	    m_drawMenu = true;
	}
    }

    if((m_textStrings.find(curFrame + 2) != m_textStrings.end()) ||
	    (!m_fadeOut && curFrame == m_animLabel->getNumFrames() - 2))
	m_textFrame->fadeOut(10);


    if(!m_framePlayed[curFrame]) {
	if(m_textStrings.find(curFrame) != m_textStrings.end()) {
	    ImagePtr tmp(new Image(UPoint(420, 45)));
	    tmp->setColorKey();
	    std::string text = m_textStrings[curFrame];
	    uint8_t numLines = 0;
	    int linebreak = text.find("\n",0)+ 1;
	    std::string thisLine;
	    // This is a bit hairy and needs to be cleaned up a bit..
	    Uint16 textw, texth;

	    while(text.substr(0, linebreak-1).length() > 0){
		thisLine = text.substr(0, linebreak-1);
		if(linebreak != -1)
		    thisLine += " ";

		m_font->extents(thisLine, textw, texth);

		tmp->renderText(thisLine, m_font, tmp->getSize().x/2 - textw/2, 10+(numLines++*20) - texth/2, m_textColor);

		if(linebreak == -1 || text == text.substr(linebreak, text.length()-linebreak))
		    break;
		text = text.substr(linebreak, text.length()-linebreak);
		linebreak = text.find("\n",0);
	    }
	    //FIXME: Really lame, need to figure out how to change font size in a sane way..
	    tmp = tmp->getResized(UPoint(320, 50).getScaled());
	    m_textFrame->changeBackground(tmp);

	    m_textFadeIn = true;
	}
	if(m_sounds.find(curFrame) != m_sounds.end())
	    SoundPlayer::Instance()->playSound(m_sounds[curFrame]);
    }
    if(m_textFadeIn)
	m_textFadeIn = m_textFrame->fadeIn(10);


    m_framePlayed[curFrame] = true;
    return 0;
}
