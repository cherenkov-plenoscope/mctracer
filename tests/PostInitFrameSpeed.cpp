// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "Core/scenery/segmented_imaging_reflector/segmented_imaging_reflector.h"
#include "Core/mctracer.h"

using namespace relleums;

TEST_CASE("PostInitFrameSpeed: post_init_based_on_mother", "[mctracer]") {


    Frame world;
    Frame* reflector;
    PropagationConfig settings;
    double Zd_Rad = 45.0;
    double Az_Rad = 75.0;

    Rot3 new_rot(0.0, Zd_Rad, deg2rad(180.0) - Az_Rad);

    Scenery scenery;
    reflector = scenery.root.append<Frame>();
    reflector->set_name_pos_rot("reflector", VEC3_ORIGIN, ROT3_UNITY);

    segmented_imaging_reflector::Config cfg;
    cfg.focal_length = 1e6;
    cfg.DaviesCotton_over_parabolic_mixing_factor = 0.0;
    cfg.gap_between_facets = 0.00001;
    cfg.max_outer_aperture_radius = 0.1;
    cfg.min_inner_aperture_radius = 0.0;
    cfg.facet_inner_hex_radius = 0.01;

    segmented_imaging_reflector::Factory factory(cfg);
    factory.add_reflector_mirror_facets_to_frame(reflector);

    scenery.root.init_tree_based_on_mother_child_relations();

    for (int i = 0; i < 1e3; i++) {
        Rot3 rot(0.0, 0.0, static_cast<double>(i)/1.0e6);
        reflector->update_rotation(rot);
    }
}
