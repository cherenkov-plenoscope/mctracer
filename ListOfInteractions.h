//=================================
// include guard
#ifndef __LISTOFINTERACTIONS_H_INCLUDED__ 
#define __LISTOFINTERACTIONS_H_INCLUDED__ 

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream> 
#include <list>
#include "Intersection.h"
#include "ListOfInteractions.h"
//=================================
class ListOfInteractions{
public:	
	list<Intersection*> Interactions;
	void show()const;
	~ListOfInteractions();
	double get_accumulative_distance()const;
	CsvRow getCsvRow(GlobalSettings& settings)const;
};
#endif // __LISTOFINTERACTIONS_H_INCLUDED__ 
