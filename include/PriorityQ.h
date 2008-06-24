#ifndef DUNE_PRIORITYQ_H
#define DYBE_PRIORITYQ_H

#include "Gfx.h"
#include "TerrainClass.h"

/*!
 *  This is priority queue used by A* path search algorithm.
 *  Although some names of member functions resemble those of
 *  standard containers, they behave a bit differently.
 *  E.g. pop_front not only removes element from queue, but also
 *  returns a pointer to it. Everything else is virtually the same.
 */

class PriorityQ
{
  public:
    PriorityQ(int mx);            // constructor
    ~PriorityQ();

    //! Adds an element to the queue if maximum size of queue is not reached
    bool push_back(TerrainClass* newNode);
    
    /*!
    
     *  Returns a pointer to TerrainClass that has the same cooridantes as pos
     *  @param pos coordinates of searched TerrainClass
     *  @return pointer to terrain class if present
     */
    TerrainClass* find(UPoint pos);
    
    //! Remove the first element from priority queue
    TerrainClass* pop_front();

    /*!
     *  If queue contains an element with the same coordinates as pos,
     *  the element will be removed
     *  @param pos coordinates of searched TerrainClass
     *  @return element is removed from queue, but a pointer to it is returned
     */     
    TerrainClass* removeNodeWithKey(UPoint pos);

    inline bool empty() 
    { 
        return (m_currentSize == 0);
    }

  private:
    void trickleDown(int index);
    void trickleUp(int index);


    int m_currentSize, // size of array
    m_maxSize; // number of items in array

    TerrainClass** m_heapArray;
};

#endif // DUNE_PRIORITYQ_H
