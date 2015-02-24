#include "ListOfPropagations.h"

//------------------------------------------------------------------------------
ListOfPropagations::ListOfPropagations(std::string name) {
	this->name = name;
}
//------------------------------------------------------------------------------
void ListOfPropagations::push_back(RayForPropagation* ray){
	propagations.push_back(ray);
}
//------------------------------------------------------------------------------
void ListOfPropagations::propagate_in_world_with_settings(	
	const CartesianFrame* world, const GlobalSettings* settings
) {
	std::cout << "Propagate list of " << propagations.size() << " rays...\n";

	if(settings->MultiThread())
		propagate_using_multi_thread(world, settings);
	else
		propagate_using_single_thread(world, settings);
}
//------------------------------------------------------------------------------
void ListOfPropagations::propagate_using_multi_thread(
	const CartesianFrame* world, 
	const GlobalSettings* settings
) {
	uint i, number_of_threads, thread_id, ray_counter;
	std::stringstream out;

	#pragma omp parallel shared(settings,world) private(number_of_threads, thread_id, out, ray_counter)
	{	

		PseudoRandomNumberGenerator dice_for_this_thread_only;
		ray_counter = 0;
		thread_id = omp_get_thread_num();
		number_of_threads = omp_get_num_threads();

		PropagationEnvironment env_for_this_thread_only;
		env_for_this_thread_only.world_geometry = world;
		env_for_this_thread_only.propagation_options = settings;
		env_for_this_thread_only.random_engine = &dice_for_this_thread_only;

		#pragma omp for schedule(dynamic) private(i) 
		for(i = 0; i<propagations.size(); i++ )
		{
			ray_counter++;
			propagations.at(i)->propagate_in(&env_for_this_thread_only);
		}

		out << "Thread " << thread_id+1 << "/" << number_of_threads;
		out << " is doing " << ray_counter << "/";
		out << propagations.size() << " rays. ";
		out << "Seed: " << dice_for_this_thread_only.seed() << "\n";
		cout << out.str();
	}
}
//------------------------------------------------------------------------------
void ListOfPropagations::propagate_using_single_thread(
	const CartesianFrame* world, 
	const GlobalSettings* settings
) {
	PseudoRandomNumberGenerator dice;

	PropagationEnvironment env;
	env.world_geometry = world;
	env.propagation_options = settings;
	env.random_engine = &dice;

	for(uint i = 0; i<propagations.size(); i++ )
		propagations.at(i)->propagate_in(&env);
		//PropagateSingleRay(world,settings,&dice,i);

	cout << "Single thread is doing all the rays\n";	
}
//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const ListOfPropagations& list) {
    os << list.get_print();
    return os;
}
//------------------------------------------------------------------------------
std::string ListOfPropagations::get_print()const {
	std::stringstream out;
	
	out << " _____List_of_Rays_____\n";
	out << "| name: " << name << "\n";
	out << "| number of Rays: " << propagations.size() << "\n";
	out << "| here are 1/1e4 of the rays:\n";
	for(int i=0; i<propagations.size()/1e4; i++)
		out << *propagations.at(i) << "\n";
	out << "|______________________\n";
	
	return out.str();
}