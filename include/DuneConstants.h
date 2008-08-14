#ifndef DUNE_CONSTANTS_H
#define DUNE_CONSTANTS_H

#define MAPMAGIC 3358395
#define SAVEMAGIC 5342535

#define	NUM_SAVEGAMES 6
#define GAMESPEED_MAX 25
#define GAMESPEED_MIN 5
#define VOLUME_MAX 100
#define VOLUME_MIN 0

#define MESSAGE_BUFFER_SIZE 5
#define MAX_PLAYERS 6
#define MAX_CLIENTYS 5
#define TICK_INTERVAL 30
#define PORT 5029
#define BUTTON_CLICK_TIME 100
#define NUM_MULTIGAMES 20

#define MSG_ACCEPT 'a'
#define MSG_ATTACK 't'
#define MSG_BUILDCOMMAND 'b'
#define MSG_BUILDCOMPLETE 'p'
#define MSG_BUTTONHIT 'h'
#define MSG_CANCELCOMMAND 'n'
#define MSG_CONSISTANCY 'o'
#define MSG_CREATEOBJECT 'r'
#define MSG_DEVASTATE 'k'
#define MSG_DEVIATE 'v'
#define MSG_JOIN 'j'
#define MSG_MESSAGE 'm'
#define MSG_OBJECTCOMMAND 'c'
#define MSG_OBJECTDESTROY 'd'
#define MSG_PALACESPECIAL 'l'
#define MSG_PALACEENABLE 'u'
#define MSG_PLACESTRUCTURE 'g'
#define MSG_QUIT 'q'
#define MSG_SETATTACKMODE 'i'
#define MSG_STARPORTORDER 'e'
#define MSG_STARPORTCANCEL 'f'
#define MSG_START 's'

#define BUILDERBUTTONSHEIGHT 48
#define BUILDERBUTTONSMINX 51
#define BUILDERBUTTONSMAXX 115
#define BUILDERBUTTONSMINY 201
#define BUILDERBUTTONSMAXY 423
#define BUILDERBUTTONSSPACING 10

#define BUILDERBUTTONSUPDOWNMINX 39
#define BUILDERBUTTONSUPDOWNMAXX 118
#define BUILDERBUTTONSUPMINY 167
#define BUILDERBUTTONSUPMAXY 188
#define BUILDERBUTTONSDOWNMINY 435
#define BUILDERBUTTONSDOWNMAXY 456

#define MAX_LINE 256
#define MAX_MUSICFILES 1000
#define MAX_MSGLENGTH 30
#define MAX_NAMELENGTH 20
#define MAX_UNITSOUNDS 5
#define MAX_XSIZE 256
#define MAX_YSIZE 256
#define ROCKFILLER 1		//how many times random generator will try to remove sand "holes" for rock from the map
#define SPICEFILLER 1		//for spice
#define ROCKFIELDS 10		//how many fields it will randomly generate
#define SPICEFIELDS 10
#define BUILDRANGE 2
#define RANDOMSPICEMIN 300	//how much spice on each spice tile
#define RANDOMSPICEMAX 500
#define RANDOMTHICKSPICEMIN 200
#define RANDOMTHICKSPICEMAX 300
#define ROCKBITS 10			//how many of the single bits of rock
#define conv2char 2.0 * PI / 256
#define BLOCKSIZE 16		//size of tile pieces 16x16
#define GAMEBARWIDTH 144
//#define NONE -1
#define NONE ((Uint32) -1) // unsigned -1
#define INVALID_POS	(-1)
#define RANDOM -2

#define DEVIATIONTIME 1000
#define HARVESTERMAXSPICE 700
#define HARVESTSPEED 0.20
#define HARVESTEREXTRACTSPEED 0.75
#define RANDOMHARVESTMOVE 300
#define HEAVILYDAMAGEDRATIO 0.3	//if health/maxHealth < this, when damaged will become heavily damage- smoke and shit
#define HEAVILYDAMAGEDSPEEDMULTIPLIER 0.666666666666
#define MAXBUILDLIST 100		//the max amount of items a builder can have in its to build list
#define BUILDTIMEMULTIPLIER 10	//more of this will make things build slower
#define RANDOMTURNTIMER 2000	//less of this makes units randomly turn more
#define CHECKTIME 10
#define NUMSELECTEDLISTS 9
#define NUM_INFANTRY_PER_CELL 5		//how many infantry can fit in a cell

#define LASTSANDFRAME 2	//is number spice output frames - 1

#define UNIT_REPAIRCOST 0.1
#define DEFAULT_GUARDRANGE 10 //0 - 10, how far unit will search for enemy when guarding
#define DEFAULT_STARTINGCREDITS 3000
#define DEFAULT_TURNSPEED 0.625
#define DEFAULT_TURRETTURNSPEED 0.625
#define DEFAULT_UNITFRAMETIME 5
#define DEFAULT_UNITVIEWRANGE 5
#define DEVIATION_TIME 10000

#define TILE_VIEWRANGE 2

#define NUM_ANGLES 8
#define MAXANGLE 7
#define RIGHT 0
#define RIGHTUP 1
#define UP 2
#define LEFTUP 3
#define LEFT 4
#define LEFTDOWN 5
#define DOWN 6
#define RIGHTDOWN 7


typedef enum {SCOUT, STANDGROUND, DEFENSIVE, AGGRESSIVE} ATTACKTYPE;
typedef enum {NOTBLOCKED, COMBLOCKED, MOUNTAIN, INFANTRY} BLOCKEDTYPE;
typedef enum {EASY, MEDIUM, HARD/*, IMPOSSIBLE*/} DIFFICULTYTYPE;
typedef enum {GS_NONE, START, BEGUN, END} GAMESTATETYPE;
typedef enum {CUSTOM, MULTIPLAYER, ORIGINAL, RANDOMMAP, SKIRMISH} GAMETYPE;
typedef enum {ATREIDES, ORDOS, HARKONNEN, SARDAUKAR, FREMEN, MERCENERY} PLAYERHOUSE;
typedef enum {CLIENT, SERVER, SINGLE} PLAYERTYPE;

#endif // DUNE_CONSTANTS_H
