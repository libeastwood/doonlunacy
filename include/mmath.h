#ifndef DUNE_MMATH_H
#define DUNE_MMATH_H

#include "Gfx.h"
#include "DuneConstants.h"
#include "dMath.h"

int power(int num1, int num2);
bool testIfInt(double number);
double fixDouble(double number);
int roundUp(double number);

#ifdef _WIN32
inline int round(double number) { return ((int)(number + 0.5)); }
inline int lround(double number) { return round(number); }
#endif

double dest_angle(UPoint p1, UPoint p2);
int getRandomInt(int min, int max);
int getRandomOf(int numParam, ...);
double normalize(double value, double max);
double min_delta(double from, double to, double max);
double trajectory_angle(double x1, double y1, double x2, double y2);
double distance_from(UPoint p1, UPoint p2);
double distance_from(double x, double y, double to_x, double to_y);
double hypotinuse_of(double x, double y);
double blockDistance(UPoint p1, UPoint p2);
double closestBlockDistance(UPoint objectLocation, UPoint targetLocation, UPoint size);
void split(char value[][MAX_LINE+1], char* line, char seperator);
int toInt(char* line);

#endif // DUNE_MMATH_H
