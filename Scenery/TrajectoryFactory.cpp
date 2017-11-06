#include "TrajectoryFactory.h"
#include <sstream>
using std::string;

//------------------------------------------------------------------------------
const Color TrajectoryFactory::trajectory_col = Color(255,128,128);
const Color TrajectoryFactory::absorption_in_void_col = Color(128,128,255);
const Color TrajectoryFactory::interaction_col = Color(128,255,128);
//------------------------------------------------------------------------------
TrajectoryFactory::TrajectoryFactory(const RayForPropagation* _ray):
	ray(_ray), radius_of_trajectory(0.01), trajectory(&Frame::void_frame)
{}
//------------------------------------------------------------------------------
void TrajectoryFactory::set_trajectory_radius(const double radius) {
	radius_of_trajectory = radius;
}
//------------------------------------------------------------------------------
void TrajectoryFactory::append_trajectory_to(Frame* root_frame) {
	
	trajectory = root_frame->append<Frame>();
	trajectory->set_name_pos_rot(
		"trajectory_" + std::to_string(ray->simulation_truth_id),
	 	Vec3::null, 
	 	Rot3::null
	);

	for(unsigned int i=0; i < ray->get_number_of_interactions_so_far(); i++) {

		if(is_not_the_last_intersection(i)) {

			Cylinder* ray_trajectory = trajectory->append<Cylinder>();
			ray_trajectory->set_name_pos_rot(
				get_trajectory_of_part_index(i),	 	
				Vec3::null, 
		 		Rot3::null
		 	);
		 	ray_trajectory->set_cylinder(
		 		radius_of_trajectory, 
		 		ray->intersection_history.at(i).
		 			get_intersection_vector_in_world_system(),
		 		ray->intersection_history.at(i+1).
		 			get_intersection_vector_in_world_system()
		 	);
		 	ray_trajectory->set_outer_color(&trajectory_col);
		 	ray_trajectory->set_inner_color(&trajectory_col);
		}	

		Sphere* intersection_indicator = trajectory->append<Sphere>();
		intersection_indicator->set_name_pos_rot(
			get_intersection_point_name_of_part(i),
			ray->intersection_history.at(i).
				get_intersection_vector_in_world_system(),
			Rot3::null
		);

		intersection_indicator->set_radius(radius_of_trajectory*2.0);

		if(ray->interaction_history.at(i) == absorption_in_void)
			intersection_indicator->set_outer_color(&absorption_in_void_col);
		else
			intersection_indicator->set_outer_color(&interaction_col);	
	}
}
//------------------------------------------------------------------------------
void TrajectoryFactory::erase_trajectory_from(Frame* root_frame) {
	root_frame->erase(trajectory);
}
//------------------------------------------------------------------------------
bool TrajectoryFactory::is_not_the_last_intersection(
	const unsigned int part_index
)const {
	return part_index < ray->get_number_of_interactions_so_far() - 1;
}
//------------------------------------------------------------------------------
std::string TrajectoryFactory::get_trajectory_of_part_index(
	const unsigned int part_index
)const {
	std::stringstream name_trajectory;
	name_trajectory << "ID_" << ray->simulation_truth_id;
	name_trajectory << "_part_" << part_index+1;
	return name_trajectory.str();
}
//------------------------------------------------------------------------------
std::string TrajectoryFactory::get_intersection_point_name_of_part(
	const unsigned int part_index
)const {
	std::stringstream name;
	name << "ID_" << ray->simulation_truth_id << "_";
	name << ray->get_type_print(ray->interaction_history.at(part_index));
	return name.str();
}