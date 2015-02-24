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
#include "RayForPropagation.h"
#include <sstream>
#include "PropagationEnvironment.h"
#include "PseudoRandomNumberGenerator.h"

//=================================
class ListOfPropagations{
	
	std::string name;
	std::vector<RayForPropagation*> propagations;
public:
	
	ListOfPropagations(std::string new_name_for_list_of_propagations);
	
	void push_back(RayForPropagation* ray);
	
	void propagate_in_world_with_settings(
		const CartesianFrame* world, 
		const GlobalSettings* settings
	);

	std::string get_print()const;	
private:

	void propagate_using_single_thread(
		const CartesianFrame* world, 
		const GlobalSettings* settings
	);

	void propagate_using_multi_thread(
		const CartesianFrame* world, 
		const GlobalSettings* settings
	);

	friend std::ostream& operator<<(
		std::ostream& os,
		const ListOfPropagations& list
	);
};
#endif // __LISTOFPROPAGATIONS_H_INCLUDED__
