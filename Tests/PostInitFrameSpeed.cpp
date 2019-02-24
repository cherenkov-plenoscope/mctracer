// Copyright 2014 Sebastian A. Mueller
#include "gtest/gtest.h"
#include "scenery/SegmentedReflector/SegmentedReflector.h"
#include "Core/mctracer.h"

using namespace relleums;

class PostInitFrameSpeed : public ::testing::Test {
 protected:
    Frame world;
    Frame* reflector;
    PropagationConfig settings;
    double Zd_Rad = 45.0;
    double Az_Rad = 75.0;
    Rot3 new_rot;

    PostInitFrameSpeed() {
        new_rot.set(0.0, Zd_Rad, deg2rad(180.0) - Az_Rad);

        world.set_name_pos_rot("world", VEC3_ORIGIN, ROT3_UNITY);
        reflector = world.append<Frame>();
        reflector->set_name_pos_rot("reflector", VEC3_ORIGIN, ROT3_UNITY);

        SegmentedReflector::Config cfg;
        cfg.focal_length = 1e6;
        cfg.DaviesCotton_over_parabolic_mixing_factor = 0.0;
        cfg.gap_between_facets = 0.00001;
        cfg.max_outer_aperture_radius = 0.1;
        cfg.min_inner_aperture_radius = 0.0;
        cfg.facet_inner_hex_radius = 0.01;

        SegmentedReflector::Factory factory(cfg);
        factory.add_reflector_mirror_facets_to_frame(reflector);

        world.init_tree_based_on_mother_child_relations();
    }
};

TEST_F(PostInitFrameSpeed, post_init_based_on_mother) {
    for (int i = 0; i < 1e3; i++) {
        Rot3 rot(0.0, 0.0, static_cast<double>(i)/1.0e6);
        reflector->update_rotation(rot);
    }
}
