#include <string.h>
#include <cstdlib>
#include <cmath>
#include <bitset>
#include <vector>
#include <algorithm>
#include <SDL.h>

#include "ValMatrix.h"
#include "Point.h"
#include "MapSeed.h"

// global seed value
static uint32_t Seed;


// some values
static const std::bitset<21> BoolArray(std::string("010011110011110000100"));

// some offsets
static const int8_t OffsetArray1[]={
    0,-1,1,-16,16,
    -17,17,-15,15,
    -2,	2,-32,32,
    -4,	4,-64,64,
    -30,30,-34,34
};

// some more offsets
static const uint8_t OffsetArray2[]={
    0,0,4,0,4,0,4,4,0,0,0,4,0,4,4,4,0,0,0,2,0,
    2,0,4,0,0,2,0,2,0,4,0,4,0,4,2,4,2,4,4,0,4,
    2,4,2,4,4,4,0,0,4,4,2,0,2,2,0,0,2,2,4,0,2,
    2,0,2,2,2,2,2,4,2,2,2,0,4,2,2,4,4,2,2,2,4,
    0,0,4,0,4,0,4,4,0,0,0,4,0,4,4,4,0,0,0,2,0,
    2,0,4,0,0,2,0,2,0,4,0,4,0,4,2,4,2,4,4,0,4,
    2,4,2,4,4,4,4,0,0,4,2,0,2,2,0,0,2,2,4,0,2,
    2,0,2,2,2,2,2,4,2,2,2,0,4,2,2,4,4,2,2,2,4
};

// TileTypes
static const int16_t TileTypes[] = {
    220, 221, 222, 229, 230, 231, 213, 214, 215, 223, 224, 225, 232, 233, 234, 216,
    217, 218, 226, 227, 228, 235, 236, 237, 219, 217, 218, 226, 227, 228, 235, 236,
    237, 238, 239, 244, 245, 125, 240, 246, 247, 241, 242, 248, 249, 241, 243, 248,
    249, 241, 242, 248, 250, 241, 243, 248, 250, 251, 252, 253, 258, 259, 260, 223,
    224, 225, 232, 233, 234, 254, 255, 256, 261, 262, 263, 257, 255, 256, 261, 262,
    263, 254, 255, 256, 261, 264, 265, 257, 255, 256, 261, 264, 265, 254, 255, 256,
    261, 266, 267, 257, 255, 256, 261, 266, 267, 210, 268, 269, 273, 274, 275, 223,
    224, 225, 232, 233, 234, 270, 271, 272, 276, 277, 278, 270, 271, 272, 279, 277,
    278, 270, 271, 272, 276, 277, 278, 270, 271, 272, 279, 277, 278, 270, 271, 272,
    276, 277, 278, 270, 271, 272, 279, 277, 278, 238, 239, 244, 245, 125, 240, 246,
    247, 280, 281, 282, 283, 280, 281, 282, 284, 238, 239, 244, 245, 125, 240, 246,
    247, 285, 286, 288, 289, 287, 286, 290, 289, 143, 291, 295, 296, 125, 240, 246,
    247, 292, 293, 297, 298, 294, 293, 297, 298, 238, 239, 244, 245, 125, 240, 246,
    247, 299, 300, 301, 302, 299, 300, 301, 303, 238, 239, 244, 245, 125, 240, 246,
    247, 304, 305, 306, 307, 304, 305, 306, 308, 210, 211, 212, 220, 221, 222, 229,
    230, 231, 213, 214, 215, 223, 313, 225, 232, 233, 234, 309, 310, 311, 314, 315,
    316, 319, 320, 321, 312, 310, 311, 314, 315, 316, 319, 320, 321, 309, 310, 311
};


/**
  Calculates the sinus like it is needed for the map creation algorithm 
  \param i	The argument of the sinus. One period is 256 long
  \return An integer between -126 and +127
  */
static inline int8_t getSinus(int i) {
    float tmp = ((sin( (((float)i)-0.0047)/128.0 * M_PI) + 1.0) * 127.001707289) - 127.001708285;
    int x = (int) tmp;
    return (x <= -126) ? -126 : x;
}

/**
  Calculates the cosinus like it is needed for the map creation algorithm 
  \param i	The argument of the cosinus. One period is 256 long
  \return An integer between -126 and +127
  */
static inline int8_t getCosinus(int i) {
    float tmp = ((cos( (((float)i)-0.0047)/128.0 * M_PI) + 1.0) * 127.001707289) - 127.001708285;
    int x = (int) tmp;
    return (x <= -126) ? -126 : x;
}

/**
  Converts a 2d coordinate to a 1d.
  \param x	The coordinate in x-direction
  \param y	The coordinate in y-direction
  \return The 1-dimensional coordinate
  */
static inline int16_t MapArray2DToMapArray1D(int16_t x,int16_t y) {
    return x | (y << 6);
}

/**
	Converts a 2d coordinate to a 1d. This function is basically the same
	as MapArray2DToMapArray1D() but limits the x to (0-63).
	\param x	The coordinate in x-direction
	\param y	The coordinate in y-direction
	\return The 1-dimensional coordinate
*/
static inline int16_t MapArray2DToMapArray1D_OOB(int16_t x,int16_t y) {
    return MapArray2DToMapArray1D(x & 0x3F,y);
}


/**
	This function smooth the map in the local neighbourhood of index.
	\param	index		The position which should be smoothed
	\param	pMapArray	Pointer to the map that should be smoothed
	\return none
*/
static void SmoothNeighbourhood(int16_t index, std::vector<uint32_t> &pMapArray) {
    int16_t TileType;
    int16_t Pos;

    TileType = (int16_t) pMapArray[index];

    if(TileType == 8) {
	pMapArray[index] = 9;
	SmoothNeighbourhood(index,pMapArray);
    } else if (TileType == 9) {
	for(int16_t y = -1; y <= 1; y++) {
	    for(int16_t x = -1; x <= 1; x++) {
		Pos = MapArray2DToMapArray1D( (index & 0x3F)+x,((index >> 6) & 0x3F)+y);
		if(Pos < 0)
		    continue;
		if(Pos >= 64*64)
		    continue;

		if(BoolArray[pMapArray[Pos]] == 1) {
		    pMapArray[index] = 8;
		    continue;
		} else {
		    if(pMapArray[Pos] == 9)
			continue;

		    pMapArray[Pos] = 8;
		}
	    }
	}
    } else {
	if(BoolArray[TileType] == 0) {
	    pMapArray[index] = 8;
	}
    }
}

/**
	Creates new random value.
	\return	The new random value
*/
static uint16_t SeedRand() {
	uint8_t a;
	uint8_t carry;
	uint8_t old_carry;

	// little endian is more useful for this algorithm
	Seed = SDL_SwapLE32(Seed);
	uint8_t* pSeed = (uint8_t*) &Seed;
	
	// shift right
	a = pSeed[0];
	a = a >> 1;
	
	// shift right in carry
	carry = a & 0x01;
	a = a >> 1;

	// rotate left through carry
	old_carry = carry;
	carry = (pSeed[2] & 0x80) >> 7;
	pSeed[2] = pSeed[2] << 1;
	pSeed[2] = pSeed[2] | old_carry;

	// rotate left through carry
	old_carry = carry;
	carry = (pSeed[1] & 0x80) >> 7;
	pSeed[1] = pSeed[1] << 1;
	pSeed[1] = pSeed[1] | old_carry;

	// invert carry
	carry = (carry == 1 ? 0 : 1);

	// subtract with carry
	a = ((uint16_t) a) - (((uint16_t) pSeed[0]) + ((uint16_t) carry));

	// shift right
	carry = a & 0x01;
	a = a >> 1;
	
	// rotate right through carry
	pSeed[0] = (pSeed[0] >> 1) | (carry << 7);
	
	// xor
	a = pSeed[0] ^ pSeed[1];

	// convert back to native endianess
	Seed = SDL_SwapLE32(Seed);

	return ((uint16_t) a);

}

/**
	Creates a random map.
	\param Para_Seed	Seed of the map
	\param pResultMap	Should be uint16_t[64*64]
*/
void createMapWithSeed(uint32_t Para_Seed,uint16_t *pResultMap)
{
    std::vector<uint8_t> Array4x4TerrainGrid(16*16+16+1);
    std::vector<uint32_t> MapArray(65*65);
    UPoint point;
    uint16_t randNum;
    uint16_t randNum2;
    uint16_t randNum3;
    int16_t index;
    uint16_t max;
    int16_t Point1;
    int16_t Point2;

    std::vector<uint16_t> curMapRow(128);
    std::vector<uint16_t> oldMapRow(128);
    ValMatrix<uint32_t> Area(3, 3);

    Seed = Para_Seed;

    for(int16_t i = 0; i < (int16_t)Array4x4TerrainGrid.size()-1; i++) {
	Array4x4TerrainGrid[i] = SeedRand() & 0x0F;
	if(Array4x4TerrainGrid[i] > 0x0A)
    	    Array4x4TerrainGrid[i] = 0x0A;
    }

    for(int16_t i = SeedRand() & 0x0F;i >= 0 ;i--) {
	randNum = SeedRand() & 0xFF;
	for(int16_t j = 0; j < 21; j++) {
	    index = randNum + OffsetArray1[j];
	    index = (index >= 0) ? index : 0;
	    index = (index <= (int16_t)(Array4x4TerrainGrid.size()-1)) ? index : (Array4x4TerrainGrid.size()-1);
	    Array4x4TerrainGrid[index] = ((uint16_t) Array4x4TerrainGrid[index] + (SeedRand() & 0x0F)) & 0x0F;
	}
    }

    for(int16_t i = SeedRand() & 0x03; i >= 0; i--) {
	randNum = SeedRand() & 0xFF;
	for(int16_t j = 0; j < 21; j++) {
	    index = randNum + OffsetArray1[j];
	    index = (index >= 0) ? index : 0;
	    index = (index <= (int16_t)(Array4x4TerrainGrid.size()-1)) ? index : (Array4x4TerrainGrid.size()-1);
	    Array4x4TerrainGrid[index] = SeedRand() & 0x03;
	}
    }

    std::vector<uint8_t>::const_iterator it = Array4x4TerrainGrid.begin();
    for(int16_t y = 0; y < 64; y+=4)
	for(int16_t x = 0; x < 64; x+=4)
	    MapArray[MapArray2DToMapArray1D(x,y)] = *it++;

    for(int16_t y = 0; y < 64; y+=4) {
	for(int16_t x = 0; x < 64; x+=4) {
	    for(int16_t i = (x % 8 == 0 ? 21 : 0) ; (x % 8 == 0 ? 21 : 0) + 21 > i; i++) {
	    	uint16_t pos;
		Point1 =  MapArray2DToMapArray1D(x + OffsetArray2[4*i],y + OffsetArray2[4*i+1]);
		Point2 =  MapArray2DToMapArray1D(x + OffsetArray2[4*i+2],y + OffsetArray2[4*i+3]);
		pos = (Point1 + Point2) / 2;

		if(pos >= 64*64)
		    continue;

		Point1 =  MapArray2DToMapArray1D_OOB(x + OffsetArray2[4*i],y + OffsetArray2[4*i+1]);
		Point2 =  MapArray2DToMapArray1D_OOB(x + OffsetArray2[4*i+2],y + OffsetArray2[4*i+3]);

		MapArray[pos] = (MapArray[Point1] + MapArray[Point2] + 1) / 2;
	    }

	}
    }

    // apply box-filter to the map
    for(int16_t y = 0, row = 0; y < 64; y++) {

	// save the old row
	std::copy(curMapRow.begin(), curMapRow.end(), oldMapRow.begin());
	std::copy(&MapArray[row], &MapArray[row+64], curMapRow.begin());

	for(int16_t x = 0; x < 64; x++, row++) {

	    Area(0,0) = ((x > 0) && (y > 0)) ? oldMapRow[x-1] : curMapRow[x];
	    Area(1,0) = (y > 0) ? oldMapRow[x] : curMapRow[x];
	    Area(2,0) = ((x < 63) && (y > 0)) ? oldMapRow[x+1] : curMapRow[x];

	    Area(0,1) = (x > 0) ? curMapRow[x-1] : curMapRow[x];
	    Area(1,1) = curMapRow[x];
	    Area(2,1) = (x < 63) ? curMapRow[x+1] : curMapRow[x];

	    Area(0,2) = ((x > 0) && (y < 63)) ? MapArray[row+64 -1] : curMapRow[x];
	    Area(1,2) = (y < 63) ? MapArray[row+64] : curMapRow[x];
	    Area(2,2) = ((x < 63) && (y < 63)) ? MapArray[row+64 +1] : curMapRow[x];

	    MapArray[row] = Area.sum()/Area.size();
	}
    }

    randNum = SeedRand() & 0x0F;
    randNum = (randNum < 8 ? 8 : randNum);
    randNum = (randNum > 0x0C ? 0x0C : randNum);
    point.y = (SeedRand() & 0x03) - 1;
    point.y = ( (randNum-3) < point.y ? randNum-3 : point.y);

    for(int16_t i = 0; i < 64*64; i++) {
	point.x = (uint16_t) MapArray[i];

	if( (randNum+4) < point.x)
	    MapArray[i] = 0x06;
	else if(point.x >= randNum)
	    MapArray[i] = 0x04;
	else if(point.x <= point.y)
	    MapArray[i] = 0x02;
	else
	    MapArray[i] = 0x00;
    }

    for(int16_t i = SeedRand() & 0x2F; i != 0; i--) {
	point.y = SeedRand() & 0x3F;
	point.x = SeedRand() & 0x3F;
	index = MapArray2DToMapArray1D(point.x,point.y);

	if(BoolArray[MapArray[index]] == 1) {
	    i++;
	    continue;
	}


	randNum = SeedRand() & 0x1F;
	for(int16_t j=0; j < randNum; j++) {
	    uint16_t pos;
	    max = SeedRand() & 0x3F;

	    if(max == 0) {
		pos = index;
	    } else {
		point.y = ((index << 2) & 0xFF00) | 0x80;
		point.x = ((index & 0x3F) << 8) | 0x80;

		randNum2 = SeedRand() & 0xFF;

		while(randNum2 > max)
		    randNum2 = randNum2 >> 1;

		randNum3 = SeedRand() & 0xFF;

		point.x += (((getSinus(randNum3) * randNum2) >> 7) << 4);
		point.y += ((((-1) * getCosinus(randNum3) * randNum2) >> 7) << 4);


		if(/*(point.x < 0) || */(point.x > 0x4000) || /*(point.y < 0) ||*/ (point.y > 0x4000)) {
		    pos =  index;
		} else {
		    pos = ((point.y & 0xFF00) >> 2) | (point.x >> 8);
		}
	    }

	    if(pos >= 64*64) {
		j--;
		continue;
	    }

	    /*
	       if(pos < 0) {
	       j--;
	       continue;
	       }*/

	    SmoothNeighbourhood(pos,MapArray);
	}
    }



    //smoothing
    std::copy(&MapArray[0], &MapArray[64], curMapRow.begin());

    for(int16_t y = 0, row = 0; y < 64; y++) {
	std::copy(curMapRow.begin(), curMapRow.end(), oldMapRow.begin());
	std::copy(&MapArray[row], &MapArray[row+64], curMapRow.begin());

	for(int16_t x = 0; x < 64; x++, row++) {

	    Area(1,0) = (y > 0) ? oldMapRow[x] : MapArray[row];
	    Area(0,1) = (x > 0) ? curMapRow[x-1] : MapArray[row];
	    Area(1,1) = MapArray[row];
	    Area(2,1) = (x < 63) ? curMapRow[x+1] : MapArray[row];
	    Area(1,2) = (y < 63) ? MapArray[row+64] : MapArray[row];

	    MapArray[row] = 0;

	    switch(Area(1,1)) {

		case 4:
		    if( (Area(1,0) == 4) || (Area(1,0) == 6) )
			MapArray[row] |= 0x01;

		    if( (Area(2,1) == 4) || (Area(2,1) == 6) )
			MapArray[row] |= 0x02;

		    if( (Area(1,2) == 4) || (Area(1,2) == 6) )
			MapArray[row] |= 0x04;

		    if( (Area(0,1) == 4) || (Area(0,1) == 6) )
			MapArray[row] |= 0x08;

		    break;

		case 8:
		    if( (Area(1,0) == 8) || (Area(1,0) == 9) )
			MapArray[row] |= 0x01;

		    if( (Area(2,1) == 8) || (Area(2,1) == 9) )
			MapArray[row] |= 0x02;

		    if( (Area(1,2) == 8) || (Area(1,2) == 9) )
			MapArray[row] |= 0x04;

		    if( (Area(0,1) == 8) || (Area(0,1) == 9) )
			MapArray[row] |= 0x08;

		    break;

		default:
		    if(Area(1,0) == Area(1,1))
			MapArray[row] |= 0x01;

		    if(Area(2,1) == Area(1,1))
			MapArray[row] |= 0x02;

		    if(Area(1,2) == Area(1,1))
			MapArray[row] |= 0x04;

		    if(Area(0,1) == Area(1,1))
			MapArray[row] |= 0x08;

		    break;
	    }

	    switch(Area(1,1)) {
		case 0:
		    MapArray[row] = 0;
		    break;
		case 4:
		    MapArray[row]++;
		    break;
		case 2:
		    MapArray[row] += 0x11;
		    break;
		case 6:
		    MapArray[row] += 0x21;
		    break;
		case 8:
		    MapArray[row] += 0x31;
		    break;
		case 9:
		    MapArray[row] += 0x41;
		    break;
	    };
	}
    }

    //create resulting array
    for(int16_t i = 0; i < 64*64; i++) {
	MapArray[i] = (MapArray[i] & 0xFE00) | ((MapArray[i] <= 85) ? (MapArray[i]+127) : TileTypes[MapArray[i]-85]) | 0xF800;
	pResultMap[i] = MapArray[i] & 0x1FF;
    }
}
