#ifndef DUNE_HOUSES_H
#define DUNE_HOUSES_H

typedef enum
{
	HOUSE_ATREIDES = 'A',		//house definitions
	HOUSE_ORDOS = 'O',
	HOUSE_HARKONNEN = 'H',
	HOUSE_SARDAUKAR = 'S',
	HOUSE_FREMEN = 'F',
	HOUSE_MERCENARY = 'M',
	NUM_HOUSES
} HOUSETYPE;


extern int houseColour[NUM_HOUSES];

#endif // DUNE_HOUSES_H
