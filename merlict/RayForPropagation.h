// Copyright 2014 Sebastian A. Mueller
#ifndef MERLICT_RAYFORPROPAGATION_H_
#define MERLICT_RAYFORPROPAGATION_H_

#include <vector>
#include <stdint.h>
#include <string>
#include "merlict/Ray.h"
#include "merlict/Intersection.h"
#include "merlict/SurfaceEntity.h"

namespace merlict {

enum Interaction {
    production,
    absorption_in_void,

    absorption_in_medium,
    absorption_on_surface,

    fresnel_reflection_on_surface,
    reflection_on_surface,

    refraction_to_outside,
    refraction_to_inside,

    scattering
};

std::string interaction_str(Interaction type);

class RayForPropagation :public Ray{
    friend class TrajectoryFactory;

 protected:
    std::vector<Intersection> intersection_history;
    std::vector<Interaction> interaction_history;

 public:
    int32_t simulation_truth_id;
    // The sumilation truth identifier is a encoding additional information of
    // the photon.
    //
    // If simulation_truth_id >= 0,
    // encodes the index of the input photons e.g. out of a CORSIKA Cherenkov
    // photon file

    RayForPropagation(const Vec3 support, const Vec3 direction);
    void push_back_intersection_and_type_to_propagation_history(
        const Intersection& interact,
        const Interaction type);
    std::string str()const;
    double accumulated_distance()const;
    unsigned int num_interactions()const;
    const Intersection& get_intersection_at(const unsigned int index)const;
    Interaction final_interaction()const;
    const Intersection& get_final_intersection()const;
    virtual double time_of_flight()const;
    Vec3 get_final_intersection_incident_vector_in_object_frame()const;

 protected:
    void push_back_production_of_ray();
    std::string history_str()const;
};

}  // namespace merlict

#endif  // MERLICT_RAYFORPROPAGATION_H_
