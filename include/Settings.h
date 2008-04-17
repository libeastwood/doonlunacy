#ifndef DUNE_SETTINGS_H
#define DUNE_SETTINGS_H

#include "DuneConstants.h"
#include "SDL.h"
#include "SDL_net.h"
#include <string>

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

	int		maxPathSearch,	//how many nodes units will search to path before stopping
			maxPlayers,
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

        double  unitScale;
	

	DIFFICULTYTYPE	playerDifficulty[MAX_PLAYERS];
			
	Uint16	port;

	IPaddress	serverIp;
} SETTINGSTYPE;

extern SETTINGSTYPE settings;


#include "singleton.h"

#include "ConfigFile.h"

class Settings: public Singleton<Settings>
{
    friend class Singleton<Settings>;
    friend class Application;
    
    protected:
        Settings();

        // protected so Application can change them 
        int m_width, m_height;
        bool m_fullscreen;
        bool m_debug;
        bool m_doubleBuffered;

        std::string m_dataDir;
        
        ConfigFile::NodePtr configFile;

    public:
        void load();
        void save();
    
        void ParseFile(const char* fn);
        void ParseOptions(int argc, char* argv[]);

        inline int GetWidth() 
                { return m_width; }
        inline int GetHeight()
                { return m_height; }

        inline bool GetFullScreen()
                { return m_fullscreen; }

        inline bool GetDebug()
                { return m_debug; }
        void SetDebug(bool x) 
                { m_debug = x; } 

        inline bool GetDoubleBuffered() 
                { return m_doubleBuffered; }

        std::string GetDataDir() { return m_dataDir; }

};

#endif // DUNE_SETTINGS_H
