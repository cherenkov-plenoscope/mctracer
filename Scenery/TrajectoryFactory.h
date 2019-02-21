// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_TRAJECTORYFACTORY_H_
#define SCENERY_TRAJECTORYFACTORY_H_

#include <string>
#include "Core/RayForPropagation.h"
#include "Core/SurfaceEntity.h"
#include "Scenery/Primitive/Cylinder.h"
#include "Scenery/Primitive/Sphere.h"

namespace relleums {

class TrajectoryFactory {
    // Spectate the propagation history of a photon.
    // A set of geometry is created from the photons propagation history.
    // Cylinders are placed along the straight parts of flight and balls in the
    // interaction points.
    //
    // Input:
    //    Single Photon* (Ray for Propagation)
    //
    // Output:
    //    A Frame* representing the photons path of flight
    //
    const RayForPropagation* ray;
    double radius_of_trajectory;
    Frame* trajectory;

 public:
    explicit TrajectoryFactory(const RayForPropagation* ray);
    void set_trajectory_radius(const double radius);
    void append_trajectory_to(Frame* root_frame);
    void erase_trajectory_from(Frame* root_frame);

 private:
    std::string get_trajectory_of_part_index(
        const unsigned int part_index)const;
    std::string get_intersection_point_name_of_part(
        const unsigned int part_index)const;
    bool is_not_the_last_intersection(
        const unsigned int part_index)const;
};

}  // namespace relleums

#endif  // SCENERY_TRAJECTORYFACTORY_H_
