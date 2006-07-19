#ifndef DUNE_LINK_H
#define DUNE_LINK_H

#include "Math.h"

class Link
{
public:
	Link(void* dd);

//	double	iData;
	void* dData;
	Link* next;
	Link* previous;
};

class Node
{
public:
	Node(COORDTYPE* location, double diff);

	double	cost,
			f,
			heuristic,
			iData;

	COORDTYPE	dData;

	Node	*parent,
			*previous,
			*next;
};

#endif // DUNE_LINK_H
