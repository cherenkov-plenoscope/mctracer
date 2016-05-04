#include "gtest/gtest.h"
#include "Core/Random/Random.h"
#include "Core/Histogram1D.h"
#include "Tools/Tools.h"

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

    for(uint i=0; i<100; i++) {
        Random::Mt19937 prng(i);
        EXPECT_EQ(i, prng.get_seed());
    }
}
//------------------------------------------------------------------------------
TEST_F(RandomGeneratorTest, Mt19937_set_and_get_seed) {

    Random::Mt19937 prng;

    for(uint i=0; i<100; i++) {
        prng.set_seed(i);
        EXPECT_EQ(i, prng.get_seed());
    }
}
//------------------------------------------------------------------------------
TEST_F(RandomGeneratorTest, uniform_0_to_1_stddev) {

    Random::Mt19937 prng(0);
    std::vector<double> samples;

    for(uint i=0; i<42*1337; i++) 
        samples.push_back(prng.uniform());

    EXPECT_NEAR(1.0/sqrt(12.0), numeric::stddev(samples), 1e-3);
}
//------------------------------------------------------------------------------
TEST_F(RandomGeneratorTest, generator_point_on_disc) {

    Random::Mt19937 prng(0);

    uint n_points = 1e6;
    double disc_radius = 1.337;
    std::vector<Vec3> points;

    for(uint i=0; i<n_points; i++)
        points.push_back(prng.get_point_on_xy_disc_within_radius(disc_radius));

    // mean position
    Vec3 sum = Vec3::null;
    for(Vec3 p: points)
        sum = sum + p;

    Vec3 mean = sum*(1.0/double(points.size()));

    EXPECT_NEAR(0.0, mean.x(), 1e-2);
    EXPECT_NEAR(0.0, mean.y(), 1e-2);
    EXPECT_EQ(0.0, sum.z());

    // distibution is evenly spread
    std::vector<double> counts_in_evaluation_bins;

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

    double mean_count = numeric::mean(counts_in_evaluation_bins);
    double stddev_count = numeric::stddev(counts_in_evaluation_bins);

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

    uint n_samples = 1e6;
    std::vector<double> samples;

    for(uint i=0; i<n_samples; i++)
        samples.push_back(sfd.draw(prng.uniform()));

    //--------------------
    // fill samples drawn from distribution into histogram

    uint bin_count = uint(pow(double(n_samples), 1.0/3.0));
    std::vector<double> bin_edges = numeric::linspace(
        f.get_limits().get_lower(),
        f.get_limits().get_upper(), 
        bin_count
    );

    Histogram1D histo(samples, bin_edges);
    //std::cout << histo;

    //--------------------
    // normalize histogram
    
    uint drawn_f_integral = 0;
    double f_integral = 0;
    for(uint i=0; i<histo.bins.size(); i++) {
        drawn_f_integral = drawn_f_integral + histo.bins[i];
        f_integral = f_integral + f(bin_edges[i]);
    }

    std::vector<double> drawn_f_normalized;
    for(uint i=0; i<histo.bins.size(); i++)
        drawn_f_normalized.push_back( 
            double(histo.bins[i])/double(drawn_f_integral)
        );

    //--------------------
    // compare initial distribution and samples drawn from distribution

    std::vector<double> ys = numeric::linspace(
        f.get_limits().get_lower(),
        f.get_limits().get_upper(), 
        sqrt(n_samples)
    );

    double max_f = ys.front();
    for(uint i=0; i<ys.size()-1; i++)
        if(max_f < ys[i])
            max_f = ys[i];
    
    for(uint i=0; i<bin_edges.size()-1; i++)
        EXPECT_NEAR(
            f(bin_edges[i])/f_integral, 
            drawn_f_normalized[i], 
            max_f*1e-2
        );
}
//------------------------------------------------------------------------------
TEST_F(RandomGeneratorTest, draw_from_poisson_distribution) {

    Random::Mt19937 prng(0);
    double sum = 0.0;
    const double rate = 1e6;

    for(uint i=0; i<uint(rate); i++)
        sum += prng.expovariate(rate);

    EXPECT_NEAR(1.0, sum, 1e-3);
}
//------------------------------------------------------------------------------
TEST_F(RandomGeneratorTest, conventional_disc_1M_draws) {

    const double r = 2.23;
    Random::Mt19937 prng(0);
    for(uint i=0; i<uint(1e5); i++) {
        Vec3 p = prng.get_point_on_xy_disc_within_radius_slow(r);
        EXPECT_TRUE(r*r >= p*p);
    }
}
//------------------------------------------------------------------------------
TEST_F(RandomGeneratorTest, rejection_sampling_disc_1M_draws) {

    const double r = 2.23;
    Random::Mt19937 prng(0);
    for(uint i=0; i<uint(1e5); i++) {
        Vec3 p = prng.get_point_on_xy_disc_within_radius(r);
        EXPECT_TRUE(r*r >= p*p);
    }
}
//------------------------------------------------------------------------------