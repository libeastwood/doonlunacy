#include "PriorityQ.h"

PriorityQ::PriorityQ(int mx)            // constructor
{
	m_maxSize = mx;
	m_currentSize = 0;
	m_heapArray = new TerrainClass*[m_maxSize];
	for (int i = 0; i < m_maxSize; i++)
		m_heapArray[i] = NULL;
}

PriorityQ::~PriorityQ()
{
	while (!empty())
		pop_front();

	delete[] m_heapArray;
}

void PriorityQ::trickleDown(int index)
{
	int smallerChild;
	TerrainClass* top = m_heapArray[index];        // save root
	while(index < m_currentSize/2)        // not on bottom row
	{
		int leftChild = 2*index+1;
		int rightChild = leftChild+1;
							  // find smaller child
		if(rightChild < m_currentSize && m_heapArray[leftChild]->f < m_heapArray[rightChild]->f)
			smallerChild = leftChild;
		else
			smallerChild = rightChild;
							  // top >= largerChild?
		if(top->f <= m_heapArray[smallerChild]->f)
			break;
							  // shift child up
		m_heapArray[index] = m_heapArray[smallerChild];
		index = smallerChild;             // go down
	}  // end while
	m_heapArray[index] = top;             // root to index
}

void PriorityQ::trickleUp(int index)
{
	int parent = (index - 1)/2;
	TerrainClass* bottom = m_heapArray[index];

	while (index > 0 && m_heapArray[parent]->f > bottom->f)
	{
		m_heapArray[index] = m_heapArray[parent];
		index = parent;
		parent = (parent - 1)/2;
	}

	m_heapArray[index] = bottom;
}

bool PriorityQ::push_back(TerrainClass* newNode)
{
	if (m_currentSize == m_maxSize)
		return false;
	m_heapArray[m_currentSize] = newNode;
	trickleUp(m_currentSize++);

	return true;
}

TerrainClass* PriorityQ::find(UPoint pos)
{
	if (empty())
		return NULL;
	
	int tempCurrent = 0;
	while((m_heapArray[tempCurrent]->x != pos.x) || (m_heapArray[tempCurrent]->y != pos.y))
	{
		tempCurrent++;
		if (m_heapArray[tempCurrent] == NULL)
			return NULL;
	}
	
	return m_heapArray[tempCurrent];
}
TerrainClass* PriorityQ::pop_front()           // delete item with max key
{                           // (assumes non-empty list)
	TerrainClass* root = m_heapArray[0];
	m_heapArray[0] = m_heapArray[--m_currentSize];
	trickleDown(0);

	return root;
}

TerrainClass* PriorityQ::removeNodeWithKey(UPoint pos)
{
	if (empty())
		return NULL;
	
	int tempCurrent = 0;
	while ((m_heapArray[tempCurrent]->x != pos.x) || (m_heapArray[tempCurrent]->y != pos.y))
	{
		tempCurrent++;
		if (m_heapArray[tempCurrent] == NULL)
			return NULL;
	}

	TerrainClass* foundNode = m_heapArray[tempCurrent];

	m_heapArray[tempCurrent] = m_heapArray[--m_currentSize];
	for (int i = m_currentSize/2 - 1; i >= 0; i--)
		trickleDown(i);
	
	return foundNode;
}

