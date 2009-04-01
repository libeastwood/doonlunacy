#ifndef DUNE_MMATH_H
#define DUNE_MMATH_H

#include "Gfx.h"
#include "DuneConstants.h"
#include "dMath.h"

int power(int num1, int num2);
bool testIfInt(float number);
float fixDouble(float number);
int roundUp(float number);

#ifdef _WIN32
inline int round(float number) { return ((int)(number + 0.5)); }
inline int lround(float number) { return round(number); }
#endif

float dest_angle(UPoint p1, UPoint p2);
inline int getRandomInt(int min, int max) { return ((rand() % (++max-min)) + min); }
template<typename T>
T getRandomOf(int numParam, ...) {
	int nthParam = getRandomInt(0,numParam-1);
	
	va_list arg_ptr;
	va_start(arg_ptr, numParam);
	
	T ret = va_arg(arg_ptr, T);
	
	for(int i = 1; i <= nthParam; i++) {
		ret = va_arg(arg_ptr, T);
	}
	va_end(arg_ptr);
	
	return ret;
}

float normalize(float value, float max);
float min_delta(float from, float to, float max);
float trajectory_angle(float x1, float y1, float x2, float y2);
float distance_from(UPoint p1, UPoint p2);
float distance_from(float x, float y, float to_x, float to_y);
float hypotinuse_of(float x, float y);
float blockDistance(UPoint p1, UPoint p2);
float closestBlockDistance(UPoint objectLocation, UPoint targetLocation, UPoint size);
void split(char value[][MAX_LINE+1], char* line, char seperator);
int toInt(char* line);

#endif // DUNE_MMATH_H
