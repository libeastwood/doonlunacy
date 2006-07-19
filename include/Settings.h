#ifndef DUNE_SETTINGS_H
#define DUNE_SETTINGS_H

#include "DuneConstants.h"
#include "SDL.h"
#include "SDL_net.h"

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

#endif // DUNE_SETTINGS_H
