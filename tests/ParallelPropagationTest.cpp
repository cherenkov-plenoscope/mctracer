// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "merlict/scenery/primitive/Disc.h"
#include "merlict/Photons.h"
#include "merlict/scenery/Scenery.h"
namespace ml = merlict;


TEST_CASE("ParallelPropagationTest: propagate_once", "[merlict]") {
	const uint64_t num_photons = 1000;
	ml::random::Mt19937 prng(0u);
	std::vector<ml::Photon> photons1 =
		ml::Photons::Source::parallel_towards_z_from_xy_disc(
			1.0,
			num_photons,
			&prng);
	REQUIRE(photons1.size() == num_photons);

	ml::Scenery scenery;
	scenery.colors.add("red", ml::Color(255, 0, 0));
	scenery.functions.add(
		"fifty_fifty",
		ml::function::Func1({
			{200e-9, 0.5},
			{1200e-9, 0.5}
		}));
	ml::Disc* disc = scenery.root.add<ml::Disc>();
 	disc->set_name_pos_rot(
        "red_disc",
        ml::Vec3(0, 0, 1),
        ml::Rot3(0, 0, 0));
    disc->set_outer_color(scenery.colors.get("red"));
    disc->set_inner_color(scenery.colors.get("red"));
    disc->set_inner_reflection(scenery.functions.get("fifty_fifty"));
    disc->set_outer_reflection(scenery.functions.get("fifty_fifty"));
    disc->set_radius(5.0);
    scenery.root.init_tree_based_on_mother_child_relations();

	ml::PropagationConfig cfg;

	ml::Photons::propagate_photons_multi_thread(
		&photons1,
		&scenery.root,
		&cfg,
		&prng);

	std::vector<ml::Interaction> final_interactions_run_1(num_photons);
	for (uint64_t i = 0; i < photons1.size(); ++i) {
		final_interactions_run_1[i] = photons1[i].get_final_interaction_type();
	}

	// Now again with same SEED
	// ------------------------

	prng.set_seed(0u);
	std::vector<ml::Photon> photons2 =
		ml::Photons::Source::parallel_towards_z_from_xy_disc(
			1.0,
			num_photons,
			&prng);

	ml::Photons::propagate_photons_multi_thread(
		&photons2,
		&scenery.root,
		&cfg,
		&prng);

	std::vector<ml::Interaction> final_interactions_run_2(num_photons);
	for (uint64_t i = 0; i < photons2.size(); ++i) {
		final_interactions_run_2[i] = photons2[i].get_final_interaction_type();
	}

	REQUIRE(photons2.size() == photons1.size());
	REQUIRE(final_interactions_run_2.size() == final_interactions_run_1.size());

	// Must yield same interactions for all photons
	// --------------------------------------------

	for (uint64_t i = 0; i < final_interactions_run_1.size(); ++i) {
		CHECK(final_interactions_run_2[i] == final_interactions_run_1[i]);
	}
}
