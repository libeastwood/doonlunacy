#ifndef DUNE_LISTSTUFF_H
#define DUNE_LISTSTUFF_H

class ListIterator;

void selectAll(ListIterator* aList);
void unselectAll(ListIterator* aList);
void copyList(ListIterator* from, ListIterator* to);

#endif // DUNE_LISTSTUFF_H
