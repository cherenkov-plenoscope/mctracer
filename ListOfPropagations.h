//=================================
// include guard
#ifndef __LISTOFPROPAGATIONS_H_INCLUDED__
#define __LISTOFPROPAGATIONS_H_INCLUDED__

//=================================
// forward declared dependencies
class GlobalSettings;
//=================================
// included dependencies
#include <iostream>
#include <omp.h>
#include "Ray.h"
#include "CsvHandler.h"
#include <list>
#include <sstream>

//=================================
class ListOfPropagations{
	
	std::string name;
	std::vector<Ray*> propagations;
public:
	ListOfPropagations(std::string new_name_for_list_of_propagations);
	void push_back(Ray* ptr_to_ray_to_push_back);
	void disp()const;
	void propagate(	
		const CartesianFrame* world, 
		const GlobalSettings* settings
	);
private:
	void PropagateSingleRay(	
		const CartesianFrame* world, 
		const GlobalSettings* settings,
		PseudoRandomNumberGenerator *dice,
		unsigned long long index
	);
	std::string get_print()const;	

	void propagate_using_single_thread(
		const CartesianFrame* world, 
		const GlobalSettings* settings
	);

	void propagate_using_multi_thread(
		const CartesianFrame* world, 
		const GlobalSettings* settings
	);

	friend std::ostream& operator<<(std::ostream& os,const ListOfPropagations& list);
};
#endif // __LISTOFPROPAGATIONS_H_INCLUDED__
