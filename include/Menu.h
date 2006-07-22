#ifndef DUNE_MENU_H
#define DUNE_MENU_H

#include "SDL.h"
#include "DuneConstants.h"

class HBox;
class VBox;
class Window;

int mainLoop();

int houseNameToNum(char* house);
void init_loadWindow();
void realign_buttons();

extern SDL_mutex    *mutex_button, // = NULL,
                    *mutex_currentWidget, // = NULL,
                    *mutex_playersJoined; // = NULL;

extern SDL_Surface	*menu,
                        *menuText;

extern int	backgroundOffsetX,
                backgroundOffsetY,
                multi_buttonRow,
                playersJoined;

extern HBox     multi_hbox,
                custom_hbox,
                custom_clientHbox[MAX_PLAYERS],
                multiClient_hbox,
                multiClient_clientHbox[MAX_PLAYERS],
                multiServer_hbox,
                multiServer_clientHbox[MAX_PLAYERS],
                single_hbox,
                options_hbox;

extern VBox	main_vbox,
                custom_vbox,
                custom_vbox2,
                custom_vbox3,
                single_vbox,
                single_vbox2,
                multi_vbox,
                multi_vbox2,
                multiClient_vbox,
                multiClient_vbox2,
                multiClient_vbox3,
                multiServer_vbox,
                multiServer_vbox2,
                multiServer_vbox3,
                options_vbox;

extern Window	about_window;


/*#pragma once

class Menu
{
public:
	Menu();
	~Menu();
};*/

#endif // DUNE_MENU_H
