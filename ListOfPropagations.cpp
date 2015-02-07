#include "ListOfPropagations.h"

//------------------------------------------------------------------------------
ListOfPropagations::ListOfPropagations(std::string name) {
	this->name = name;
}
//------------------------------------------------------------------------------
void ListOfPropagations::push_back(Ray* ptr_to_ray_to_push_back){
	propagations.push_back(ptr_to_ray_to_push_back);
}
//------------------------------------------------------------------------------
void ListOfPropagations::propagate(	
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

		#pragma omp for schedule(dynamic) private(i) 
		for(i = 0; i<propagations.size(); i++ )
		{
			ray_counter++;
			PropagateSingleRay(
				world,settings,
				&dice_for_this_thread_only,
				i
			);
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

	for(uint i = 0; i<propagations.size(); i++ )
		PropagateSingleRay(world,settings,&dice,i);

	cout << "Single thread is doing all the rays\n";	
}
//------------------------------------------------------------------------------
void ListOfPropagations::PropagateSingleRay(	
	const CartesianFrame* world, 
	const GlobalSettings* settings,
	PseudoRandomNumberGenerator *dice,
	unsigned long long index
) {
	ListOfInteractions* history_of_this_specific_ray;
	history_of_this_specific_ray = new ListOfInteractions;

	propagations.at(index)->SetHistory(history_of_this_specific_ray);

	uint interaction_count = 0;

	propagations.at(index)->propagate(
		world,
		history_of_this_specific_ray,
		interaction_count,
		settings,
		dice
	);
}
//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const ListOfPropagations& list) {
    os << list.get_print();
    return os;
}
//------------------------------------------------------------------------------
void ListOfPropagations::disp()const {
	std::cout << get_print();
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