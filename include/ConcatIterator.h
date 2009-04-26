#ifndef CONCATITERATOR_H_INCLUDED
#define CONCATITERATOR_H_INCLUDED

#include <list>
#include "Log.h"

template <class T> class ConcatIterator {
	
private:
	typedef std::list<T> TList;
	typedef TList* TListPointer;
	typedef typename TList::iterator TList_Iter;
	typedef std::list< TListPointer> TListOfList;
	typedef typename TListOfList::iterator TListOfList_Iter;


	TList_Iter List_Iter;
	TListOfList_Iter ListOfList_Iter;
	TListOfList ListOfList;
public:
	ConcatIterator() { };
	~ConcatIterator() { };

	void addList(std::list<T>& _List) {
		if(_List.empty()) {
			// ignore this list
			return;
		}
		
		if(ListOfList.empty()) {
			ListOfList.push_back(&_List);
			ListOfList_Iter = ListOfList.begin();
			List_Iter = (*ListOfList_Iter)->begin();
		} else {
			ListOfList.push_back(&_List);
		}
	}
	
	bool IterationFinished() {
		if(ListOfList.empty()) {
			return true;
		}
		
		if(ListOfList_Iter == ListOfList.end()) {
			return true;
		}
		
		return false;
	}
	
	T operator* () {
		if(IterationFinished()) {
			LOG(LV_ERROR, "ConcatIterator", "Cannot dereference because iteration is finished.");
			exit(EXIT_FAILURE);
		}
		
		return *List_Iter;
	}
	
	void operator++ () {
		if(IterationFinished()) {
			LOG(LV_ERROR, "ConcatIterator", "Cannot increment because iteration is finished.\n");
			exit(EXIT_FAILURE);
		}
		
		List_Iter++;
		if(List_Iter == (*ListOfList_Iter)->end()) {
			// use next list
			ListOfList_Iter++;
			if(ListOfList_Iter == ListOfList.end()) {
				return;
			}
			
			List_Iter = (*ListOfList_Iter)->begin();
		}
	}
};

#endif // CONCATITERATOR_H_INCLUDED
