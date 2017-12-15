#include "gtest/gtest.h"
#include "Core/Random/Random.h"
#include "Core/Histogram1D.h"
#include "Tools/Tools.h"
#include "Tools/Numeric.h"
#include <array>
using std::array;
using std::vector;

class RandomGeneratorTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(RandomGeneratorTest, fake_constant) {

    for(double i=0.0; i<100.0; i++) {
        Random::FakeConstant prng(i);
        for(int j=0; j<10; j++)
            EXPECT_EQ(i, prng.uniform());
    }
}
//------------------------------------------------------------------------------
TEST_F(RandomGeneratorTest, Mt19937_init_and_get_seed) {

    for(unsigned int i=0; i<100; i++) {
        Random::Mt19937 prng(i);
        EXPECT_EQ(i, prng.get_seed());
    }
}
//------------------------------------------------------------------------------
TEST_F(RandomGeneratorTest, Mt19937_set_and_get_seed) {

    Random::Mt19937 prng;

    for(unsigned int i=0; i<100; i++) {
        prng.set_seed(i);
        EXPECT_EQ(i, prng.get_seed());
    }
}
//------------------------------------------------------------------------------
TEST_F(RandomGeneratorTest, uniform_0_to_1_stddev) {

    Random::Mt19937 prng(0);
    vector<double> samples;

    for(unsigned int i=0; i<42*1337; i++) 
        samples.push_back(prng.uniform());

    EXPECT_NEAR(1.0/sqrt(12.0), Numeric::stddev(samples), 1e-3);
}
//------------------------------------------------------------------------------
TEST_F(RandomGeneratorTest, generator_point_on_disc) {

    Random::Mt19937 prng(0);

    unsigned int n_points = 1e6;
    double disc_radius = 1.337;
    vector<Vec3> points;

    for(unsigned int i=0; i<n_points; i++)
        points.push_back(prng.get_point_on_xy_disc_within_radius(disc_radius));

    // mean position
    Vec3 sum = Vec3::ORIGIN;
    for(Vec3 p: points)
        sum = sum + p;

    Vec3 mean = sum*(1.0/double(points.size()));

    EXPECT_NEAR(0.0, mean.x, 1e-2);
    EXPECT_NEAR(0.0, mean.y, 1e-2);
    EXPECT_EQ(0.0, sum.z);

    // distibution is evenly spread
    vector<double> counts_in_evaluation_bins;

    double evaluation_disc_radius = disc_radius/5.0;
    
    for(
        double r=evaluation_disc_radius; 
        r<disc_radius-evaluation_disc_radius; 
        r=r+evaluation_disc_radius
    ) {
        for(double phi=0; phi<2.0*M_PI; phi=phi+M_PI/3.0) {

            double counts_in_evaluation_bin = 0;
            Vec3 eval_disc_pos(r*cos(phi), r*sin(phi), 0.0);
            
            for(Vec3 p: points)
                if(eval_disc_pos.distance_to(p) <= evaluation_disc_radius)
                    counts_in_evaluation_bin++;

            counts_in_evaluation_bins.push_back(counts_in_evaluation_bin);
        }
    }

    double mean_count = Numeric::mean(counts_in_evaluation_bins);
    double stddev_count = Numeric::stddev(counts_in_evaluation_bins);

    EXPECT_TRUE(stddev_count/mean_count < 1e-2);
    //std::cout << mean_count << " pm " << stddev_count << "\n";
}
//------------------------------------------------------------------------------
TEST_F(RandomGeneratorTest, draw_from_distribution) {

    //--------------------
    // create distributions

    Function::Polynom3 f(0.0, 1.0, 0.0, 0.0, Function::Limits(0.0, 1.0));

    //--------------------
    // sample from distribution

    Random::Mt19937 prng(0);
    Random::SamplesFromDistribution sfd(&f);

    unsigned int n_samples = 1e6;
    vector<double> samples;

    for(unsigned int i=0; i<n_samples; i++)
        samples.push_back(sfd.draw(prng.uniform()));

    //--------------------
    // fill samples drawn from distribution into histogram

    unsigned int bin_count = (unsigned int)(pow(double(n_samples), 1.0/3.0));
    vector<double> bin_edges = Numeric::linspace(
        f.limits().get_lower(),
        f.limits().get_upper(), 
        bin_count
    );

    Histogram1D histo(samples, bin_edges);
    //std::cout << histo;

    //--------------------
    // normalize histogram
    
    unsigned int drawn_f_integral = 0;
    double f_integral = 0;
    for(unsigned int i=0; i<histo.bins.size(); i++) {
        drawn_f_integral = drawn_f_integral + histo.bins[i];
        f_integral = f_integral + f.evaluate(bin_edges[i]);
    }

    vector<double> drawn_f_normalized;
    for(unsigned int i=0; i<histo.bins.size(); i++)
        drawn_f_normalized.push_back( 
            double(histo.bins[i])/double(drawn_f_integral)
        );

    //--------------------
    // compare initial distribution and samples drawn from distribution

    vector<double> ys = Numeric::linspace(
        f.limits().get_lower(),
        f.limits().get_upper(), 
        sqrt(n_samples)
    );

    double max_f = ys.front();
    for(unsigned int i=0; i<ys.size()-1; i++)
        if(max_f < ys[i])
            max_f = ys[i];
    
    for(unsigned int i=0; i<bin_edges.size()-1; i++)
        EXPECT_NEAR(
            f.evaluate(bin_edges[i])/f_integral, 
            drawn_f_normalized[i], 
            max_f*1e-2
        );
}
//------------------------------------------------------------------------------
TEST_F(RandomGeneratorTest, draw_from_poisson_distribution) {

    Random::Mt19937 prng(0);
    double sum = 0.0;
    const double rate = 1e6;

    for(unsigned int i=0; i<(unsigned int)(rate); i++)
        sum += prng.expovariate(rate);

    EXPECT_NEAR(1.0, sum, 1e-3);
}
//------------------------------------------------------------------------------
TEST_F(RandomGeneratorTest, conventional_disc_1M_draws) {

    const double r = 2.23;
    Random::Mt19937 prng(0);
    for(unsigned int i=0; i<(unsigned int)(1e5); i++) {
        Vec3 p = prng.get_point_on_xy_disc_within_radius_slow(r);
        EXPECT_TRUE(r*r >= p*p);
    }
}
//------------------------------------------------------------------------------
TEST_F(RandomGeneratorTest, rejection_sampling_disc_1M_draws) {

    const double r = 2.23;
    Random::Mt19937 prng(0);
    for(unsigned int i=0; i<(unsigned int)(1e5); i++) {
        Vec3 p = prng.get_point_on_xy_disc_within_radius(r);
        EXPECT_TRUE(r*r >= p*p);
    }
}
//------------------------------------------------------------------------------
TEST_F(RandomGeneratorTest, sphere_point_picking) {

    Random::Mt19937 prng(0);

    for(double max_zenith=5.0; max_zenith<10.0; max_zenith += 1.5) {

        const unsigned int n_points = 1e4*(max_zenith*max_zenith)/25.0;
        vector<Vec3> points;
        for(unsigned int i=0; i<n_points; i++) {
            points.push_back(
                prng.get_point_on_unitsphere_within_polar_distance(
                    Deg2Rad(max_zenith))
            );
        }

        const unsigned int n_test_points = 0.01*n_points;
        vector<Vec3> test_points;
        for(unsigned int i=0; i<n_test_points; i++) {
            test_points.push_back(
                prng.get_point_on_unitsphere_within_polar_distance(
                    Deg2Rad(max_zenith - 3.0))
            );
        }

        const double inclusion_radius = Deg2Rad(3.0);
        vector<unsigned int> inclusion(n_test_points);
        for(unsigned int i=0; i<n_test_points; i++) inclusion[i] = 0;

        for(unsigned int i=0; i<n_test_points; i++) {
            for(unsigned int j=0; j<n_points; j++) {
                double distance = (points[j] - test_points[i]).norm();
                if(distance <= inclusion_radius)
                    inclusion[i]++;
            }
        }

        double sum = 0.0;
        for(unsigned int i=0; i<n_test_points; i++) sum += (double)inclusion[i];
        const double mean = sum/(double)n_test_points;

        double s = 0.0;
        for(unsigned int val: inclusion) s += ((double)val - mean)*((double)val - mean);
        const double stddev = sqrt(s/inclusion.size());
        
        const double relative_spread = stddev/mean;

        EXPECT_NEAR(relative_spread, 0.01, 0.01);
    }
}