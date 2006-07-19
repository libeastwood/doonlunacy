#include "globals.h"

class ListIterator;
class Widget;

//static Widget* focusedWidget;

ListIterator        *selectedLists;
ListIterator        *unitList;
ListIterator        *selectedList;
ListIterator        *structureList;
ListIterator        *bulletList;

ListIterator        *airUnitBlitList;
ListIterator        *groundUnitBlitList;
ListIterator        *infantryBlitList;
ListIterator        *selectionBlitList;
ListIterator        *undergroundUnitBlitList;

int	structuresCreated,
                unitsCreated;


bool     shade,
                fog_wanted;

bool     attackMoveMode,
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

int lookDist[11];

GAMESTATETYPE gameState;

bool debug;

bool messageMode;
bool showFPS;

bool			loadingGame;

char messageBuffer[MESSAGE_BUFFER_SIZE][MAX_LINE];


