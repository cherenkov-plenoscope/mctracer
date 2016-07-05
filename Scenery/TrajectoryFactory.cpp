#include "TrajectoryFactory.h"
//------------------------------------------------------------------------------
const Color TrajectoryFactory::trajectory_col = Color(255,128,128);
const Color TrajectoryFactory::absorption_in_void_col = Color(128,128,255);
const Color TrajectoryFactory::interaction_col = Color(128,255,128);
//------------------------------------------------------------------------------
TrajectoryFactory::TrajectoryFactory(const RayForPropagation* _ray) {
	ray = _ray;
	radius_of_trajectory_in_m = 0.01;
}
//------------------------------------------------------------------------------
void TrajectoryFactory::set_trajectory_radius(const double radius) {
	radius_of_trajectory_in_m = radius;
}
//------------------------------------------------------------------------------
void TrajectoryFactory::add_trajectory(Frame* frame)const {
	
	Frame* trajectory = frame->append<Frame>();
	trajectory->set_name_pos_rot(
		"trajectory_" + std::to_string(ray->identifier),
	 	Vec3::null, 
	 	Rot3::null
	);

	for(uint i=0; i < ray->get_number_of_interactions_so_far(); i++) {

		if(is_not_the_last_intersection(i)) {

			Cylinder* ray_trajectory = trajectory->append<Cylinder>();
			ray_trajectory->set_name_pos_rot(
				get_trajectory_of_part_index(i),	 	
				Vec3::null, 
		 		Rot3::null
		 	);
		 	ray_trajectory->set_cylinder(
		 		radius_of_trajectory_in_m, 
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

		intersection_indicator->set_radius(radius_of_trajectory_in_m*2.0);

		if(ray->interaction_type_history.at(i) == absorption_in_void)
			intersection_indicator->set_outer_color(&absorption_in_void_col);
		else
			intersection_indicator->set_outer_color(&interaction_col);	
	}

	// post init
	trajectory->init_tree_based_on_mother_child_relations();
}
//------------------------------------------------------------------------------
bool TrajectoryFactory::is_not_the_last_intersection(
	const uint part_index
)const {
	return part_index < ray->get_number_of_interactions_so_far() - 1;
}
//------------------------------------------------------------------------------
std::string TrajectoryFactory::get_trajectory_of_part_index(
	const uint part_index
)const {
	std::stringstream name_trajectory;
	name_trajectory << "ID_" << ray->identifier;
	name_trajectory << "_part_" << part_index+1;
	return name_trajectory.str();
}
//------------------------------------------------------------------------------
std::string TrajectoryFactory::get_intersection_point_name_of_part(
	const uint part_index
)const {
	std::stringstream name;
	name << "ID_" << ray->identifier << "_";
	name << ray->get_type_print(ray->interaction_type_history.at(part_index));
	return name.str();
}