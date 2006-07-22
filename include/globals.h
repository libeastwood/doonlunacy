#ifndef DUNE_GLOBALS_H
#define DUNE_GLOBALS_H

#include "DuneConstants.h"

// fwd decs
class ListIterator;
class MapClass;
class SoundPlayerClass;
class MentatClass;

extern SoundPlayerClass* soundPlayer;

//extern MentatClass *mentat;

extern ListIterator        *selectedLists;
extern ListIterator        *unitList;
extern ListIterator        *selectedList;
extern ListIterator        *structureList;
extern ListIterator        *bulletList;

extern ListIterator        *airUnitBlitList;
extern ListIterator        *groundUnitBlitList;
extern ListIterator        *infantryBlitList;
extern ListIterator        *selectionBlitList;
extern ListIterator        *undergroundUnitBlitList;

extern int	structuresCreated,
                unitsCreated;

extern bool     edit_mode;

extern bool     shade,
                fog_wanted;

extern bool     attackMoveMode,
                builderSelectMode,
                fullscreenMode,
                groupCreateMode,
                menuMode,
                moveDownMode,
                moveLeftMode,
                moveRightMode,
                moveUpMode,
                placingMode,
                shift,
                radarMode,
                selectionMode,
                quitGame ;

extern int lookDist[11];

extern GAMESTATETYPE gameState;

extern bool debug;

extern bool messageMode;
extern bool showFPS;

extern bool			inMainMenu;
extern bool			loadingGame;

extern char messageBuffer[MESSAGE_BUFFER_SIZE][MAX_LINE];

#endif // DUNE_GLOBALS_H
