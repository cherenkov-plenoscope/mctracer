// Copyright 2014 Sebastian A. Mueller
#include "Scenery/TrajectoryFactory.h"
#include <sstream>
using std::string;

namespace relleums {

TrajectoryFactory::TrajectoryFactory(const RayForPropagation* _ray):
    ray(_ray),
    radius_of_trajectory(0.01),
    trajectory(&Frame::VOID_FRAME) {}

void TrajectoryFactory::set_trajectory_radius(const double radius) {
    radius_of_trajectory = radius;
}

void TrajectoryFactory::append_trajectory_to(Frame* root_frame) {
    trajectory = root_frame->append<Frame>();
    trajectory->set_name_pos_rot(
        "trajectory_" + std::to_string(ray->simulation_truth_id),
        VEC3_ORIGIN,
        ROT3_UNITY);

    for (
        unsigned int i = 0;
        i < ray->get_number_of_interactions_so_far();
        i++
    ) {
        if (is_not_the_last_intersection(i)) {
            Cylinder* ray_trajectory = trajectory->append<Cylinder>();
            ray_trajectory->set_name_pos_rot(
                get_trajectory_of_part_index(i),
                VEC3_ORIGIN,
                ROT3_UNITY);
            ray_trajectory->set_cylinder(
                radius_of_trajectory,
                ray->intersection_history.at(i).
                    position_in_root_frame(),
                ray->intersection_history.at(i+1).
                    position_in_root_frame());
            ray_trajectory->set_outer_color(&COLOR_RED);
            ray_trajectory->set_inner_color(&COLOR_RED);
        }

        Sphere* intersection_indicator = trajectory->append<Sphere>();
        intersection_indicator->set_name_pos_rot(
            get_intersection_point_name_of_part(i),
            ray->intersection_history.at(i).
                position_in_root_frame(),
            ROT3_UNITY);

        intersection_indicator->set_radius(radius_of_trajectory*2.0);

        if (ray->interaction_history.at(i) == absorption_in_void)
            intersection_indicator->set_outer_color(&COLOR_DARK_GRAY);
        else
            intersection_indicator->set_outer_color(&COLOR_GREEN);
    }
}

void TrajectoryFactory::erase_trajectory_from(Frame* root_frame) {
    root_frame->erase(trajectory);
}

bool TrajectoryFactory::is_not_the_last_intersection(
    const unsigned int part_index
)const {
    return part_index < ray->get_number_of_interactions_so_far() - 1;
}

std::string TrajectoryFactory::get_trajectory_of_part_index(
    const unsigned int part_index
)const {
    std::stringstream name_trajectory;
    name_trajectory << "ID_" << ray->simulation_truth_id;
    name_trajectory << "_part_" << part_index+1;
    return name_trajectory.str();
}

std::string TrajectoryFactory::get_intersection_point_name_of_part(
    const unsigned int part_index
)const {
    std::stringstream name;
    name << "ID_" << ray->simulation_truth_id << "_";
    name << ray->get_type_print(ray->interaction_history.at(part_index));
    return name.str();
}

}  // namespace relleums
