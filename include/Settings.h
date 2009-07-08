#ifndef DUNE_SETTINGS_H
#define DUNE_SETTINGS_H


#include <boost/python.hpp>
#include <SDL.h>
#include <string>

#include "Point.h"

#include "DuneConstants.h"
#include "Definitions.h"

namespace python = boost::python;


typedef struct
{
	GAMETYPE gameType;
	PLAYERTYPE playerType;

	bool	concreteRequired,
		doubleBuffered,
		finished,
		isHuman[MAX_PLAYERS],
		update,
		won;

	char	lastAddress[MAX_LINE],
		localPlayerName[MAX_NAMELENGTH],
		mapFilename[MAX_LINE],
		playerName[MAX_PLAYERS][MAX_NAMELENGTH],
		screen[MAX_LINE],
		serverString[MAX_LINE];

	int	maxPlayers,
		width, height,
		winFlags,

		playerColour[MAX_PLAYERS],
		playerHouse[MAX_PLAYERS],
		playerTeam[MAX_PLAYERS],
		playerMapNumber[MAX_PLAYERS],
		playerNum,
		techLevel,
		numPlayers,
		gamespeed,
		campaignLevel;

	float  unitScale;


	DIFFICULTYTYPE	playerDifficulty[MAX_PLAYERS];

	Uint16	port;

	//IPaddress	serverIp;
} SETTINGSTYPE;

extern SETTINGSTYPE settings;


#include "singleton.h"
#include "Log.h"

class Settings: public Singleton<Settings>
{
	friend class Singleton<Settings>;
	friend class SoundPlayer;
	friend class HScale;
	protected:
	Settings();

	// protected so Application can change them 
	UPoint m_resolution;
	bool m_fullscreen;
	int m_debug;
	bool m_doubleBuffered;
	bool m_playIntro;
	bool m_updated;

	//! Used for frame independent movement.
	int m_gameSpeed;

	//! @name SFX
	//@{
	//! whether sound should be played
	bool m_soundOn;

	//! volume of sound effects
	int	m_sfxVolume;

	//! volume of voice over sounds
	int	m_voiceVolume;

	//! volume of unit responses played when a unit is selected
	int	m_responseVolume;
	//@}

	//! @name MUSIC
	//@{

	//! whether music should be played  
	bool m_musicOn;

	//! music volume
	int	m_musicVolume;
	//@}

	//! how many nodes units will search to path before stopping
	int m_maxPathSearch;

	int m_emuOpl;

	std::string m_dataDir;

	python::object	main;
	/*
	   python::object	global;*/
	python::object	local;
	public:
	void load();
	void save();

	void ParseFile(const char* fn);
	void ParseOptions(int argc, char* argv[]);

	inline int GetGameSpeed() { return m_gameSpeed; }

	inline int GetMaxSearchPath() { return m_maxPathSearch; }

	inline UPoint GetResolution() { return m_resolution; }

	inline bool GetFullScreen() { return m_fullscreen; }

	inline bool GetDoubleBuffered() { return m_doubleBuffered; }

	std::string GetDataDir() { return m_dataDir; }

	EMUOPL GetEmuOpl();

	bool GetSound() { return m_soundOn; }
	bool GetMusic() { return m_musicOn; }
	bool GetPlayIntro() { return m_playIntro; }
	void ToggleSound() { local["config"]["sound"]["sound_on"] = m_soundOn = !m_soundOn, m_updated = true; }
	void ToggleMusic() { local["config"]["sound"]["music_on"] = m_musicOn = !m_musicOn, m_updated = true; }
	void ToggleIntro() { local["config"]["play_intro"] = m_playIntro = !m_playIntro, m_updated = true; }
	UPoint ToggleResolution();
	bool ToggleFullscreen();
	void SetResolution(UPoint resolution);
    
	EMUOPL ToggleEmuOpl();

};

#endif // DUNE_SETTINGS_H
