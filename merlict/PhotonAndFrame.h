// Copyright 2014 Sebastian A. Mueller
#ifndef MERLICT_PHOTONANDFRAME_H_
#define MERLICT_PHOTONANDFRAME_H_

#include "merlict/Photon.h"
#include "merlict/Frame.h"
#include "merlict/RayAndFrame.h"
#include "merlict/PropagationEnvironment.h"

namespace merlict {

class Propagator {
 public:
    Intersection isec;
    PropagationEnvironment env;
    Photon* ph;

    Propagator(
        Photon* photon,
        PropagationEnvironment env);
    void propagate();
    bool limit_of_interactions_is_not_reached_yet()const;
    void work_on_first_causal_intersection();
    bool absorbed_in_medium_before_reaching_surface()const;
    void interact_with_object();
    void get_absorbed_in_void_space();
    void reflect_on_surface_and_propagate_on(const Interaction type);
    void reach_boundary_layer();
    void fresnel_refraction_and_reflection();
    void pass_the_boundary_layer(
        const Fresnel &fresnel);
    void propagate_on_after_boundary_layer(
        const Fresnel &fresnel);
    void get_absorbed_on_surface();
};

}  // namespace merlict

#endif  // MERLICT_PHOTONANDFRAME_H_
