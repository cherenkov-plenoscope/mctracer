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
	const Frame* world, const GlobalSettings* settings
) {
	//std::cout << "Propagate list of " << propagations.size() << " rays...\n";

	if(settings->MultiThread())
		propagate_using_multi_thread(world, settings);
	else
		propagate_using_single_thread(world, settings);
}
//------------------------------------------------------------------------------
void ListOfPropagations::propagate_using_multi_thread(
	const Frame* world, 
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
		//cout << out.str();
	}
}
//------------------------------------------------------------------------------
void ListOfPropagations::propagate_using_single_thread(
	const Frame* world, 
	const GlobalSettings* settings
) {
	PseudoRandomNumberGenerator dice;

	PropagationEnvironment env;
	env.world_geometry = world;
	env.propagation_options = settings;
	env.random_engine = &dice;

	for(uint i = 0; i<propagations.size(); i++ )
		propagations.at(i)->propagate_in(&env);;

	//cout << "Single thread is doing all the rays\n";	
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
	out << "| here are some of the rays:\n";
	for(uint i=0; i<propagations.size(); i++)
		out << *propagations.at(i) << "\n";
	out << "|______________________\n";
	
	return out.str();
}
//------------------------------------------------------------------------------
uint ListOfPropagations::get_number_of_propagations()const {
	return propagations.size();
}
//------------------------------------------------------------------------------
uint ListOfPropagations::get_number_of_propagations_absorbed_in_object(
	const Frame* obj
)const {
	uint counter = 0;

	for(RayForPropagation* ray : propagations)
		if(ray->get_final_intersection()->get_intersecting_object() == obj)
			counter++;

	return counter;
}
//------------------------------------------------------------------------------
std::string ListOfPropagations::get_csv_print_for_propagations_ending_in(
	const Frame* frame
)const {
	
	uint counter = 0;
	std::stringstream body;
	for(RayForPropagation* ray : propagations) {
		
		if(ray->get_final_intersection()->get_intersecting_object() == frame) {

			counter++;
			// ID
			body << ray->get_id() << ", ";
			// x in obj sys
			
			body << ray->get_final_intersection()->
				get_intersection_vector_in_object_system().x() << ", ";
			// y in obj sys
			body << ray->get_final_intersection()->
				get_intersection_vector_in_object_system().y() << ", ";
			// time
			body << ray->get_time_of_flight();
			body << "\n";
		}
	}

	std::stringstream head;
	head << "# ListOfPropagations: " << name << "\n";
	head << "# final absorbtions in: " << frame->get_path_in_tree_of_frames() << "\n";
	head << "# " << 100.0*double(counter)/double(propagations.size()) << " percent\n";
	head << "# ID, x_obj_sys [m], y_obj_sys [m], time[s]\n";

	std::stringstream out;
	out << head.str() << body.str();
	return out.str();
}
//------------------------------------------------------------------------------
Frame* ListOfPropagations::get_trajectories()const {
	
	Frame* all_trajectories = new Frame;
	
	all_trajectories->set_name_pos_rot(
		name,
	 	Vector3D::null, 
	 	Rotation3D::null
	);

	for(RayForPropagation* ray : propagations) {
		TrajectoryFactory factory(ray);
		all_trajectories->set_mother_and_child(factory.get_trajectory());
	}

	all_trajectories->init_tree_based_on_mother_child_relations();

	return all_trajectories;
}
//------------------------------------------------------------------------------
Frame* ListOfPropagations::get_mean_trajectoy_in_world_using_options(
	const Frame* world, const GlobalSettings* settings
) {

	Vector3D mean_support = Vector3D(0.0, 0.0 ,0.0);
	Vector3D mean_direction = Vector3D::null;

	uint num_of_rays = 0;
	for(RayForPropagation* ray : propagations) {
		num_of_rays++;
		mean_direction = mean_direction + ray->Direction();
		mean_support = mean_support + ray->Support();
	}

	mean_support = mean_support/num_of_rays;
	mean_direction = mean_direction/num_of_rays;
	mean_direction.normalize();

	Photon mean_photon(mean_support, mean_direction, 433.0);

	PseudoRandomNumberGenerator dice;
	PropagationEnvironment env;
	env.world_geometry = world;
	env.propagation_options = settings;
	env.random_engine = &dice;

	mean_photon.propagate_in(&env);


	std::cout << mean_photon;

	TrajectoryFactory factory(&mean_photon);
	return factory.get_trajectory();
}
//------------------------------------------------------------------------------
Frame* ListOfPropagations::get_next_trajectoy() {

	std::cout << *(propagations.at(number_of_trajectories_handed_out));

	TrajectoryFactory factory(
		propagations.at(number_of_trajectories_handed_out++)
	);
	return factory.get_trajectory();
}
//------------------------------------------------------------------------------
bool ListOfPropagations::has_still_trajectoies_left()const {
	return number_of_trajectories_handed_out <= propagations.size();
}