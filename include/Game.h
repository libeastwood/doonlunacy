#ifndef DUNE_GAME_H
#define DUNE_GAME_H

class MentatClass;
class MessageBox;

extern float fps;
extern int  messageTimer[];
extern MessageBox messageBox;

void do_game();
void game_loadMap();
void game_loop();

void game_options(void *var);
void game_mentat(void *var);
void game_mentat_exited();
void setMentat(MentatClass *m);

void setAttackMode(void* data);
void mcvDeploy();

void devastatorDestruct();

void game_quit();
void game_resume();
void game_settings();
void game_cancelSaveEntry();
void game_setSaveGameSpot(void* data);
void game_saveEntry();
void game_save();
void game_loadEntry(void* data);
void game_load();
void game_cancel();
void game_loadEntryCallback(int num);
  
void settings_ok();
void settings_cancel();

void init_save_load_windows();

int resolveItemPicture(int id);

void setupView();
void drawCursor();
void drawScreen();
void doInput();
void doUnitIcon();
void netUpdate();
void processObjects();
void checkSelectionInterface();

#endif // DUNE_GAME_H
