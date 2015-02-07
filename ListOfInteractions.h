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
	std::vector<Intersection*> Interactions;
	void print()const;
	std::string get_print()const;
	~ListOfInteractions();
	double get_accumulative_distance()const;
	friend std::ostream& operator<<(std::ostream& os, const ListOfInteractions& intersec_list);
};
#endif // __LISTOFINTERACTIONS_H_INCLUDED__ 