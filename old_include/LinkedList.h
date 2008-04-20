#ifndef DUNE_LINKEDLIST_H
#define DUNE_LINKEDLIST_H

#include "Link.h"

class LinkedList
{
public:
	LinkedList();
	~LinkedList();
	inline bool isEmpty() { return first == NULL; }
	void insertFirst(void* newData);
	void insertLast(void* newData);
	inline void* getFirst() { return first->dData; }
	//void* findNodeWithKey(COORDTYPE* location);
	void* getElement(int elementNum);
	//void* removeNodeWithKey(COORDTYPE* location);
	void* removeFirst();
	void clearList();

private:
	Link	*first, *last;
};

#endif //  DUNE_LINKEDLIST_H





/*
class LinkedList
{
public:
	LinkedList();
	void insertFirst(void* newObject);
	void insertLast(void* newObject);
	void* deleteFirst();
	
	inline bool isEmpty() { return (first == NULL); }

private:
	Link	*first,
			*last;
};
*/

