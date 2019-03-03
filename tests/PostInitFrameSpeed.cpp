// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "merlict/scenery/segmented_imaging_reflector/segmented_imaging_reflector.h"
#include "merlict/merlict.h"
namespace ml = merlict;


TEST_CASE("PostInitFrameSpeed: post_init_based_on_mother", "[merlict]") {
    double Zd_Rad = 45.0;
    double Az_Rad = 75.0;

    ml::Rot3 new_rot(0.0, Zd_Rad, ml::deg2rad(180.0) - Az_Rad);

    ml::Scenery scenery;
    scenery.functions.add(
        "mirror_reflectivity",
        ml::function::Func1({{200e-9, 1}, {1200e-9, 1}}));
    scenery.colors.add("mirror_color", ml::Color(128, 128, 128));
    scenery.colors.add("inner_mirror_color", ml::Color(255, 255, 255));
    ml::SurfaceEntity* reflector = scenery.root.append<ml::SurfaceEntity>();
    reflector->set_name_pos_rot("reflector", ml::VEC3_ORIGIN, ml::ROT3_UNITY);

    ml::segmented_imaging_reflector::Config cfg;
    cfg.focal_length = 1e6;
    cfg.DaviesCotton_over_parabolic_mixing_factor = 0.0;
    cfg.gap_between_facets = 0.00001;
    cfg.max_outer_aperture_radius = 0.1;
    cfg.min_inner_aperture_radius = 0.0;
    cfg.facet_inner_hex_radius = 0.01;

    ml::segmented_imaging_reflector::Factory factory(cfg);
    factory.add_to_SurfaceEntity(reflector);

    scenery.root.init_tree_based_on_mother_child_relations();
}
