// Copyright 2014 Sebastian A. Mueller
#include "merlict/PhotonAndFrame.h"

namespace merlict {

Propagator::Propagator(
    Photon* photon,
    PropagationEnvironment environment
): env(environment), ph(photon) {
    propagate();
}

void Propagator::propagate() {
    if (limit_of_interactions_is_not_reached_yet())
        work_on_first_causal_intersection();
}

bool Propagator::limit_of_interactions_is_not_reached_yet()const {
    return ph->num_interactions() <=
        env.config->max_num_interactions_per_photon;
}

void Propagator::work_on_first_causal_intersection() {
    isec = rays_first_intersection_with_frame(ph, env.root_frame);

    if (isec.does_intersect() && !absorbed_in_medium_before_reaching_surface())
        interact_with_object();
    else
        get_absorbed_in_void_space();
}

bool Propagator::absorbed_in_medium_before_reaching_surface()const {
    const double one_over_e_way =
        isec.half_way_depth_coming_from(ph->wavelength);
    const double survival_prob =
        exp(-isec.distance_to_ray_support()/one_over_e_way);
    return env.prng->uniform() > survival_prob;
}

void Propagator::interact_with_object() {
    if (
        isec.facing_reflection_propability(ph->wavelength) >=
        env.prng->uniform()
    )
        reflect_on_surface_and_propagate_on(REFLECTION_ON_SURFACE);
    else
        reach_boundary_layer();
}

void Propagator::get_absorbed_in_void_space() {
    ph->push_back_intersection_and_interaction(
        isec,
        ABSORPTION_IN_VOID);
}

void Propagator::reflect_on_surface_and_propagate_on(const Interaction type) {
    ph->set_support_and_direction(
        isec.position_in_root_frame(),
        isec.reflection_direction_in_root_frame(ph->direction()));

    ph->push_back_intersection_and_interaction(
        isec,
        type);

    propagate();
}

void Propagator::reach_boundary_layer() {
    if (isec.boundary_layer_is_transparent())
        fresnel_refraction_and_reflection();
    else
        get_absorbed_on_surface();
}

void Propagator::fresnel_refraction_and_reflection() {
    Fresnel fresnel(
        isec.object2root()->
            orientation_inverse(ph->direction()),
        isec.surface_normal_of_facing_surface_in_object_frame(),
        isec.refractive_index_coming_from(ph->wavelength),
        isec.refractive_index_going_to(ph->wavelength));

    if (fresnel.reflection_propability() > env.prng->uniform())
        reflect_on_surface_and_propagate_on(FRESNEL_REFLECTION_ON_SURFACE);
    else
        pass_the_boundary_layer(fresnel);
}

void Propagator::pass_the_boundary_layer(const Fresnel &fresnel) {
    if (isec.from_outside_to_inside())
        ph->push_back_intersection_and_interaction(
            isec,
            REFRACTION_TO_INSIDE);
    else
        ph->push_back_intersection_and_interaction(
            isec,
            REFRACTION_TO_OUTSIDE);

    propagate_on_after_boundary_layer(fresnel);
}

void Propagator::propagate_on_after_boundary_layer(const Fresnel &fresnel) {
    if (isec.object()->has_restrictions_on_frames_to_propagate_to() &&
        !isec.going_to_default_refractive_index()
    )
        env.root_frame =
            isec.object()->allowed_frame_to_propagate_to();
    else
        env.root_frame =
            isec.object()->root();

    ph->set_support_and_direction(
        isec.position_in_root_frame(),
        isec.object2root()->orientation(
            fresnel.get_refrac_dir_in_object_system()));

    propagate();
}

void Propagator::get_absorbed_on_surface() {
    ph->push_back_intersection_and_interaction(
        isec,
        ABSORPTION_ON_SURFACE);
}

}  // namespace merlict
