#include "mmath.h"
#include <math.h>
#include <stdlib.h>
#include "MapClass.h"
#include <iostream>


/*
int min(int num1, int num2)
{
	int minimum = num1;

	if (num2 < num1)
		minimum = num2;

	return minimum;
}

int max(int num1, int num2)
{
	int maximum = num2;

	if (num2 < num1)
		maximum = num1;

	return maximum;
}
*/
int power(int num1, int num2)
{
	int result = 1;
	while (num2-- > 0)
		result *= num1;

	return result;
}

bool testIfInt(float number)
{
	bool result;

	float temp = number - (int)number;
	if (temp <= 0.0001 && temp >= -0.9999)
		result = true;
	else
		result = false;

	return result;
}
float fixDouble(float number)
{
	if (fabs(number - (float)((int)number)) < 0.00001)
		number = (float)((int)number);

	return number;
}
int roundUp(float number)
{
	int result = (int)number;

	if (!testIfInt(number)) result++;

	return result;
}

float dest_angle(UPoint p1, UPoint p2)
{
        float  destAngle;

        float  diffX = p2.x - p1.x,
                diffY = p2.y - p1.y;

        if (diffX != 0)
        {
                if ((diffX >= 0) && (diffY >= 0))  //if x2 >= x1 && y2 >= y1
                {
                        destAngle = 2*PI - atan(fabs(diffY/diffX));
                }
                else if ((diffX < 0) && (diffY >= 0))  //if x2 < x1 && y2 >= y1
                {
                        destAngle = atan(fabs(diffY/diffX)) + PI;
                }
                else if ((diffX < 0) && (diffY < 0))  //if x2 < x1 && y2 < y1
                {
                        destAngle = PI - atan(fabs(diffY/diffX));
                }
                else if ((diffX >= 0) && (diffY < 0))  //if x2 >= x1 && y2 < y1
                {
                        destAngle = atan(fabs(diffY/diffX));
                }
        }
        else if (diffY <= 0)
        {
                destAngle = PI/2;
        }
        else destAngle = 3*PI/2;

        if (destAngle == 2*PI) destAngle = 0;

        return (destAngle*256.0/(2*PI));
}

int getRandomInt(int min, int max)
{
	max++;
	return ((rand() % (max-min)) + min);
}
float normalize(float value, float max)
{
	while(value < 0.0)
		value += max;
	while(value > max)
		value -= max;

	return(value);
}

float min_delta(float from, float to, float max)
{
	float d1 = from - to;
	float d2 = (from + max) - to;
	float d3 = from - (to + max);
	float val = d1;

	if(fabs(val) > fabs(d2)) val = d2;
	if(fabs(val) > fabs(d3)) val = d3;
	return(val);
}
/*
float trajectory_angle(float x1, float y1, float x2, float y2)
{ 
  float dx = min_delta(x1, x2, X_MAX);
  float dy = min_delta(y1, y2, Y_MAX);
  float alpha = atan(dy / dx);

  if(dx < 0.0)
    alpha += PI;
  alpha = normalize(alpha, PI * 2.0);

  alpha = ((alpha + PI) / (PI * 2.0)) * 256.0;
  alpha = normalize(alpha, 256.0);
 return alpha;
}
*/
/*
float wcorner(float x, float w)
{
  float value = (x - (w / 2.0) - space_x);

  while(value < (-w))
    value += X_MAX;
  while(value > (X_MAX - w))
    value -= X_MAX;
  value *= space_zoom;

  return(value);
}

float hcorner(float y, float h)
{
  float value = (y - (h / 2.0) - space_y);

  while(value < (-h))
    value += Y_MAX;
  while(value > (Y_MAX - h))
    value -= Y_MAX;
  value *= space_zoom;

  return(value);
}

float nearest_coord(float from, float to, float max)
{
  return(from - min_delta(from, to, max));
}
*/
float distance_from(UPoint p1, UPoint p2)
{
	float	first = (p1.x - p2.x),
			second = (p1.y - p2.y);

	return(sqrt(first*first + second*second));
}
float distance_from(float x, float y, float to_x, float to_y)
{
	float	first = (x - to_x),
			second = (y - to_y);

	return(sqrt(first*first + second*second));
}
float hypotinuse_of(float x, float y)
{
	return(sqrt((x * x) + (y * y)));
}
float blockDistance(UPoint p1, UPoint p2)
{
	int xDis = abs(p1.x - p2.x),
		yDis = abs(p1.y - p2.y),
		minDis = std::min(xDis, yDis);

	return ((float)std::max(xDis, yDis) + (float)minDis*(DIAGONALCOST - 1.0));
	//return (((float)minDis)*DIAGONALCOST + max(xDis, yDis) - minDis);
}

float closestBlockDistance(UPoint objectLocation, UPoint targetLocation, UPoint size)
{
	UPoint closestPoint;

	//find the closest cell of a structure from a location
	if (objectLocation.x <= targetLocation.x)	//if we are left of the structure
		closestPoint.x = targetLocation.x;	//set destination, left most point
	else if (objectLocation.x >= (targetLocation.x + size.x-1))	//vica versa
		closestPoint.x = targetLocation.x + size.x-1;
	else
		closestPoint.x = objectLocation.x;	//we are above or below at least on cell of the structure, closest path is straight

	//same deal but with y
	if (objectLocation.y <= targetLocation.y)
		closestPoint.y = targetLocation.y;
	else if (objectLocation.y >= (targetLocation.y + size.y-1))
		closestPoint.y = targetLocation.y + size.y-1;
	else
		closestPoint.y = objectLocation.y;

	return blockDistance(objectLocation, closestPoint);
}


void split(char value[][MAX_LINE], char* line, char seperator)
{
	int i = 0,
		j = 0,
		current = 0,
		length = strlen(line);

	while (i < length)
	{
		if ((line[i] != seperator) && ((line[i] != '\n') && (line[i] != '\r')))
			value[current][j++] = line[i];
		else
		{
			value[current++][j] = '\0';
			j = 0;
		}
		i++;
	}
	value[current][j] = '\0';
}

int toInt(char* line)
{
	int	i = 0,
		length = 0,
		result = 0;

	length = strlen(line);

	i = 0;
	while (length > 0)
		result += ((int)line[i++] - 48) * power(10, --length);

	return result;
}

