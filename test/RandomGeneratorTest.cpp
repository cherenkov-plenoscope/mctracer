#include "gtest/gtest.h"
#include "Core/Random/Random.h"
#include "Core/Histogram1D.h"

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
    std::vector<Vector3D> points;

    for(uint i=0; i<n_points; i++)
        points.push_back(prng.get_point_on_xy_disc_within_radius(disc_radius));

    // mean position
    Vector3D sum = Vector3D::null;
    for(Vector3D p: points)
        sum = sum + p;

    Vector3D mean = sum*(1.0/double(points.size()));

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
            Vector3D eval_disc_pos(r*cos(phi), r*sin(phi), 0.0);
            
            for(Vector3D p: points)
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

    Random::Mt19937 prng(0);

    // f(x) = x^2 [0, 1)
    Function::Polynom3 f(0.0, 1.0, 0.0, 0.0, Function::Limits(0.0, 1.0));

    Random::SamplesFromDistribution sfd(&f, &prng);

    std::vector<double> samples;
    for(uint i=0; i<1337*1337; i++)
        samples.push_back(sfd.draw());

    std::vector<double> bins = numeric::linspace(
        f.get_limits().get_lower(),
        f.get_limits().get_upper(), 
        50
    );

    Histogram1D histo(samples, bins);

    std::vector<double> drawn_f;
    for(uint i=0; i<histo.hist.size(); i++)
        drawn_f.push_back( double(histo.hist[i])/histo.sample_size);
}
//------------------------------------------------------------------------------