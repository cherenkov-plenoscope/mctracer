// Copyright 2014 Sebastian A. Mueller
#include <array>
#include "catch.hpp"
#include "merlict/merlict.h"
namespace ml = merlict;
using std::array;
using std::vector;


TEST_CASE("RandomGeneratorTest: fake_constant", "[merlict]") {
    for (double i = 0.0; i < 100.0; i++) {
        ml::random::FakeConstant prng(i);
        for (int j = 0; j < 10; j++)
            CHECK(prng.uniform() == i);
    }
}

TEST_CASE("RandomGeneratorTest: Mt19937_init_and_get_seed", "[merlict]") {
    for (unsigned int i = 0; i < 100; i++) {
        ml::random::Mt19937 prng(i);
        CHECK(prng.seed() == i);
    }
}

TEST_CASE("RandomGeneratorTest: Mt19937_set_and_get_seed", "[merlict]") {
    ml::random::Mt19937 prng;
    for (unsigned int i = 0; i < 100; i++) {
        prng.set_seed(i);
        CHECK(prng.seed() == i);
    }
}

TEST_CASE("RandomGeneratorTest: uniform_0_to_1_stddev", "[merlict]") {
    ml::random::Mt19937 prng(0);
    vector<double> samples;
    for (unsigned int i = 0; i < 42*1337; i++)
        samples.push_back(prng.uniform());
    CHECK(1.0/sqrt(12.0) == Approx(ml::numeric::stddev(samples)).margin(1e-3));
}

TEST_CASE("RandomGeneratorTest: generator_point_on_disc", "[merlict]") {
    ml::random::Mt19937 prng(0);
    unsigned int n_points = 1e6;
    double disc_radius = 1.337;
    vector<ml::Vec3> points;
    for (unsigned int i = 0; i < n_points; i++)
        points.push_back(prng.get_point_on_xy_disc_within_radius(disc_radius));
    // mean position
    ml::Vec3 sum = ml::VEC3_ORIGIN;
    for (ml::Vec3 p : points)
        sum = sum + p;
    ml::Vec3 mean = sum*(1.0/static_cast<double>(points.size()));
    CHECK(0.0 == Approx(mean.x).margin(1e-2));
    CHECK(0.0 == Approx(mean.y).margin(1e-2));
    CHECK(sum.z == 0.0);
    // distibution is evenly spread
    vector<double> counts_in_evaluation_bins;
    double evaluation_disc_radius = disc_radius/5.0;
    for (
        double r = evaluation_disc_radius;
        r < disc_radius-evaluation_disc_radius;
        r = r+evaluation_disc_radius
    ) {
        for (double phi = 0; phi < 2.0*M_PI; phi = phi+M_PI/3.0) {
            double counts_in_evaluation_bin = 0;
            ml::Vec3 eval_disc_pos(r*cos(phi), r*sin(phi), 0.0);
            for (ml::Vec3 p : points)
                if (eval_disc_pos.distance_to(p) <= evaluation_disc_radius)
                    counts_in_evaluation_bin++;
            counts_in_evaluation_bins.push_back(counts_in_evaluation_bin);
        }
    }
    double mean_count = ml::numeric::mean(counts_in_evaluation_bins);
    double stddev_count = ml::numeric::stddev(counts_in_evaluation_bins);
    CHECK(stddev_count/mean_count < 1e-2);
    // std::cout << mean_count << " pm " << stddev_count << "\n";
}

TEST_CASE("RandomGeneratorTest: draw_from_distribution", "[merlict]") {
    // -------------------
    // create distributions
    ml::function::Func1 f(ml::function::polynom3(0, 1, 0, 0, 0.0, 1.0, 4096));
    // -------------------
    // sample from distribution
    ml::random::Mt19937 prng(0);
    ml::random::SamplesFromDistribution sfd(&f);
    unsigned int n_samples = 1e6;
    vector<double> samples;
    for (unsigned int i = 0; i < n_samples; i++)
        samples.push_back(sfd.draw(prng.uniform()));
    // -------------------
    // fill samples drawn from distribution into histogram
    unsigned int bin_count = static_cast<unsigned int>(
        pow(static_cast<double>(n_samples), 1.0/3.0));
    vector<double> bin_edges = ml::numeric::linspace(
        f.limits.lower,
        f.limits.upper,
        bin_count);
    ml::Histogram1 histo(samples, bin_edges);
    // std::cout << histo;
    // -------------------
    // normalize histogram
    unsigned int drawn_f_integral = 0;
    double f_integral = 0;
    for (unsigned int i = 0; i < histo.bins.size(); i++) {
        drawn_f_integral = drawn_f_integral + histo.bins[i];
        f_integral = f_integral + f.evaluate(bin_edges[i]);
    }
    vector<double> drawn_f_normalized;
    for (unsigned int i = 0; i < histo.bins.size(); i++)
        drawn_f_normalized.push_back(
            static_cast<double>(histo.bins[i])/
            static_cast<double>(drawn_f_integral));
    // -------------------
    // compare initial distribution and samples drawn from distribution
    vector<double> ys = ml::numeric::linspace(
        f.limits.lower,
        f.limits.upper,
        sqrt(n_samples));
    double max_f = ys.front();
    for (unsigned int i = 0; i < ys.size()-1; i++)
        if (max_f < ys[i])
            max_f = ys[i];
    for (unsigned int i = 0; i < bin_edges.size()-1; i++)
        CHECK(f.evaluate(bin_edges[i])/f_integral == Approx(drawn_f_normalized[i]).margin(max_f*1e-2));
}

TEST_CASE("RandomGeneratorTest: draw_from_poisson_distribution", "[merlict]") {
    ml::random::Mt19937 prng(0);
    double sum = 0.0;
    const double rate = 1e6;
    for (unsigned int i = 0; i< rate; i++)
        sum += prng.expovariate(rate);
    CHECK(1.0 == Approx(sum).margin(1e-3));
}

TEST_CASE("RandomGeneratorTest: conventional_disc_1M_draws", "[merlict]") {
    const double r = 2.23;
    ml::random::Mt19937 prng(0);
    for (unsigned int i = 0; i < (unsigned int)(1e5); i++) {
        ml::Vec3 p = prng.get_point_on_xy_disc_within_radius_slow(r);
        CHECK(r*r >= p*p);
    }
}

TEST_CASE("RandomGeneratorTest: rejection_sampling_disc_1M_draws", "[merlict]") {
    const double r = 2.23;
    ml::random::Mt19937 prng(0);
    for (unsigned int i = 0; i < (unsigned int)(1e5); i++) {
        ml::Vec3 p = prng.get_point_on_xy_disc_within_radius(r);
        CHECK(r*r >= p*p);
    }
}

TEST_CASE("RandomGeneratorTest: full_sphere", "[merlict]") {
    unsigned int n = static_cast<unsigned int>(1e6);
    ml::random::Mt19937 prng(0);
    ml::random::ZenithDistancePicker zd_picker(0, M_PI);
    ml::random::UniformPicker az_picker(0, 2*M_PI);
    ml::Vec3 mean_position = ml::VEC3_ORIGIN;
    for (unsigned int i = 0; i < n; i++) {
        mean_position = mean_position +
            ml::random::draw_point_on_sphere(&prng, zd_picker, az_picker);
    }
    mean_position = mean_position/static_cast<double>(n);
    CHECK(mean_position.x == Approx(0.0).margin(1e-3));
    CHECK(mean_position.y == Approx(0.0).margin(1e-3));
    CHECK(mean_position.z == Approx(0.0).margin(1e-3));
}

TEST_CASE("RandomGeneratorTest: octo_sphere", "[merlict]") {
    unsigned int n = static_cast<unsigned int>(1e6);
    ml::random::Mt19937 prng(0);
    ml::random::ZenithDistancePicker zd_picker(0, M_PI/2);
    ml::random::UniformPicker az_picker(0, M_PI/2);
    ml::Vec3 mean_position = ml::VEC3_ORIGIN;
    for (unsigned int i = 0; i < n; i++) {
        mean_position = mean_position +
            ml::random::draw_point_on_sphere(&prng, zd_picker, az_picker);
    }
    mean_position = mean_position/static_cast<double>(n);
    CHECK(mean_position.x == Approx(0.5).margin(1e-3));
    CHECK(mean_position.y == Approx(0.5).margin(1e-3));
    CHECK(mean_position.z == Approx(0.5).margin(1e-3));
}

TEST_CASE("RandomGeneratorTest: octo_sphere_minus_z", "[merlict]") {
    unsigned int n = static_cast<unsigned int>(1e3);
    ml::random::Mt19937 prng(0);
    ml::random::ZenithDistancePicker zd_picker(M_PI/2, M_PI);
    ml::random::UniformPicker az_picker(0, M_PI/2);
    ml::Vec3 mean_position = ml::VEC3_ORIGIN;
    for (unsigned int i = 0; i < n; i++) {
        mean_position = mean_position +
            ml::random::draw_point_on_sphere(&prng, zd_picker, az_picker);
    }
    mean_position = mean_position/static_cast<double>(n);
    CHECK(mean_position.x == Approx(0.5).margin(2e-2));
    CHECK(mean_position.y == Approx(0.5).margin(2e-2));
    CHECK(mean_position.z == Approx(-0.5).margin(2e-2));
}

TEST_CASE("RandomGeneratorTest: normal_distribution", "[merlict]") {
    const vector<double> means = {-1.9, -.3, 0.0, 4.6};
    const vector<double> stds = {0.2, 0.7, 1.0, 2.3};
    for (double mean: means) {
        for (double std: stds) {
            ml::random::Mt19937 prng(0);
            vector<double> samples;
            for (unsigned int i = 0; i < 10*1000; i++)
                samples.push_back(prng.normal(mean, std));
            CHECK(std == Approx(ml::numeric::stddev(samples)).margin(1e-1));
            CHECK(mean == Approx(ml::numeric::mean(samples)).margin(1e-1));
        }
    }
}
