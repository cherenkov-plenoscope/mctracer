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
#include "Photon.h"
#include <sstream>
#include "TrajectoryFactory.h"
#include "PropagationEnvironment.h"
#include "PseudoRandomNumberGenerator.h"

//=================================
class ListOfPropagations{
	
	std::string name;
	std::vector<RayForPropagation*> propagations;

	uint number_of_trajectories_handed_out = 0;
public:
	
	ListOfPropagations(std::string new_name_for_list_of_propagations);
	
	void push_back(RayForPropagation* ray);
	
	void propagate_in_world_with_settings(
		const CartesianFrame* world, 
		const GlobalSettings* settings
	);

	std::string get_print()const;	

	uint get_number_of_propagations()const;

	uint get_number_of_propagations_absorbed_in_object(
		const CartesianFrame* obj
	)const;

	std::string get_csv_print_for_propagations_ending_in(const CartesianFrame* frame)const;

	CartesianFrame* get_trajectories()const;

	CartesianFrame* get_next_trajectoy();

	bool has_still_trajectoies_left()const;

	CartesianFrame* get_mean_trajectoy_in_world_using_options(
		const CartesianFrame* world, const GlobalSettings* settings
	);
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
