// Copyright 2014 Sebastian A. Mueller
#include "gtest/gtest.h"
#include "Scenery/Primitive/Disc.h"
#include "Core/Photons.h"
#include "Scenery/Scenery.h"

using namespace relleums;
using std::string;
using std::stringstream;
using std::vector;

class ParallelPropagationTest : public ::testing::Test {};

TEST_F(ParallelPropagationTest, propagate_once) {
	const uint64_t num_photons = 1000;
	Random::Mt19937 prng(0u);
	vector<Photon> photons1 = Photons::Source::parallel_towards_z_from_xy_disc(
		1.0,
		num_photons,
		&prng);
	ASSERT_EQ(num_photons, photons1.size());

	Scenery scenery;
	scenery.colors.add("red", Color(255, 0, 0));
	scenery.functions.add(
		"fifty_fifty",
		Function::Func1({
			{200e-9, 0.5},
			{1200e-9, 0.5}
		}));
	Disc* disc = scenery.root.append<Disc>();
 	disc->set_name_pos_rot(
        "red_disc",
        Vec3(0, 0, 1),
        Rot3(0, 0, 0));
    disc->set_outer_color(scenery.colors.get("red"));
    disc->set_inner_color(scenery.colors.get("red"));
    disc->set_inner_reflection(scenery.functions.get("fifty_fifty"));
    disc->set_outer_reflection(scenery.functions.get("fifty_fifty"));
    disc->set_radius(5.0);
    scenery.root.init_tree_based_on_mother_child_relations();

	PropagationConfig cfg;

	Photons::propagate_photons_multi_thread(
		&photons1,
		&scenery.root,
		&cfg,
		&prng);

	vector<Interaction> final_interactions_run_1(num_photons);
	for (uint64_t i = 0; i < photons1.size(); ++i) {
		final_interactions_run_1[i] = photons1[i].get_final_interaction_type();
	}

	// Now again with same SEED
	// ------------------------

	prng.set_seed(0u);
	vector<Photon> photons2 = Photons::Source::parallel_towards_z_from_xy_disc(
		1.0,
		num_photons,
		&prng);

	Photons::propagate_photons_multi_thread(
		&photons2,
		&scenery.root,
		&cfg,
		&prng);

	vector<Interaction> final_interactions_run_2(num_photons);
	for (uint64_t i = 0; i < photons2.size(); ++i) {
		final_interactions_run_2[i] = photons2[i].get_final_interaction_type();
	}

	ASSERT_EQ(photons1.size(), photons2.size());
	ASSERT_EQ(final_interactions_run_1.size(), final_interactions_run_2.size());

	// Must yield same interactions for all photons
	// --------------------------------------------

	for (uint64_t i = 0; i < final_interactions_run_1.size(); ++i) {
		EXPECT_EQ(
			final_interactions_run_1[i],
			final_interactions_run_2[i]);
	}
}
