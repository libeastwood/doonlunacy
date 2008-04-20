#ifndef DUNE_MAPGENERATOR_H
#define DUNE_MAPGENERATOR_H

#include "DuneConstants.h"

void clear_terrain(int tile, int type);
void smooth_spot(int i, int j);
void smooth_terrain();
void fixWalls(int xPos, int yPos);
void make_random_map(int sizeX, int sizeY);
int readMapPlayerNumbers(char* filename, int numbers[MAX_PLAYERS]);
int readMapMaxPlayers(char* filename);
bool loadMap(char* mapName);

#endif // DUNE_MAPGENERATOR_H
