#ifndef DUNE_MMATH_H
#define DUNE_MMATH_H

#include <sys/time.h>
#include <boost/random.hpp>

#include "Gfx.h"
#include "DuneConstants.h"
#include "dMath.h"

static int getSeed() {
    struct timeval test;
    gettimeofday(&test, NULL);
    return test.tv_sec | test.tv_usec;
}
static boost::mt19937 randomEngine(getSeed());

template <typename T>
inline T getRandom(T min, T max) {
    boost::uniform_smallint<T> distribution(min, max);
    boost::variate_generator<boost::mt19937&, boost::uniform_smallint<T> > generator(randomEngine, distribution);

    return generator();
}

template <>
inline int getRandom<int>(int min, int max){
    boost::uniform_int<int> distribution(min, max);
    boost::variate_generator<boost::mt19937&, boost::uniform_int<int> > generator(randomEngine, distribution);
    return generator();
}

template <>
inline float getRandom<float>(float min, float max){
    boost::uniform_real<float> distribution(min, max);
    boost::variate_generator<boost::mt19937&, boost::uniform_real<float> > generator(randomEngine, distribution);
    return generator();
}

int power(int num1, int num2);
bool testIfInt(float number);
float fixDouble(float number);
int roundUp(float number);

#ifdef _WIN32
inline int round(float number) { return ((int)(number + 0.5)); }
inline int lround(float number) { return round(number); }
#endif

float dest_angle(UPoint p1, UPoint p2);

template<typename T>
T getRandomOf(T numParam, ...) {
	int nthParam = getRandom(0, numParam);
	
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
