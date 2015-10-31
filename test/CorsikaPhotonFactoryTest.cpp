#include "gtest/gtest.h"
#include "MmcsCorsikaFileIO/CorsikaPhotonFactory.h"

class CorsikaPhotonFactoryTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(CorsikaPhotonFactoryTest, convert_photons) {
      
    const std::vector<float> corsika_photon = 
    	{1.2, 3.4, 0.0, 0.0, 1e-9, 1e5, 0.999, 433};
    //   x    y    xcos ycos time  zem  weight lambda
    //   cm   cm   1    1    ns    cm   1    nm
    
    const uint id = 1337;
    Random::FakeConstant prng(0.0);

    CorsikaPhotonFactory cpf(corsika_photon, id, &prng);
    
    Photon* ph;

    ASSERT_TRUE(cpf.passed_atmosphere());
    	
    ph = cpf.get_photon();

    EXPECT_EQ(id, ph->get_id());
    EXPECT_NEAR(433e-9, ph->get_wavelength(), 1e-9);

    // since the x,y angles are zero, the support vector can be tested
    EXPECT_NEAR(0.012, ph->Support().x(), 1e-9);
    EXPECT_NEAR(0.034, ph->Support().y(), 1e-9);
    EXPECT_NEAR(1e3, ph->Support().z(), 1e-9);
    ph->delete_history();
}
//------------------------------------------------------------------------------
TEST_F(CorsikaPhotonFactoryTest, execute_atmospheric_absorption) {
     
	// In Corsika photons can have a 'weight' to represent a bunch of photons.
	// This was introduced in Corsika to save computations and disk space.
	// Corsika does not recomend to have weights above 5.
	// Mctracer has no concet of weighted photon bunches. 
	// We force Corsika to produce single photons, i.e. the maximum weight
	// in the Corsika output is 1.0.
	// A weight below 1.0 means, that the photon passed Corsika's atmosphere
	// simulation with a probability of this weigth.
	// In mctracer we collapse this photons probability when the photon
	// is created in mctracer. This is why the CorsikaPhotonFactory is 
	// given a PseudoRandomNumberGenerator.

    double absorbed = 0;
    double passed = 0;
    double total = 1e5;

    Random::Mt19937 prng(Random::zero_seed);

    for(double i=0; i<total-1; i++) {

    	float weight = i/total; // from 0.0 to 1.0
	    const std::vector<float> corsika_photon = 
	    	{1.2, 3.4, 0.0, 0.0, 1e-9, 1e3, weight, 433};
	    //   x    y    xcos ycos time  zem  weight lambda
	    //   cm   cm   1    1    ns    cm   1    nm
	    
	    CorsikaPhotonFactory cpf(corsika_photon, 1337, &prng);

	    if(cpf.passed_atmosphere())
	    	passed++;
	    else
	    	absorbed++;
    }

    double passed_ratio = passed/total;
    EXPECT_NEAR(0.5, passed_ratio, 1e-2);
}
//------------------------------------------------------------------------------
TEST_F(CorsikaPhotonFactoryTest, mctracer_rejects_photon_weight_below_0) {
 
    const uint id = 1337;
    Random::Mt19937 prng(Random::zero_seed);
    const std::vector<float> corsika_photon = 
    	{1.2, 3.4, 0.0, 0.0, 1e-9, 1e5, -0.1, 433};
    
    EXPECT_THROW(
    	CorsikaPhotonFactory cpf(corsika_photon, id, &prng), 
    	CorsikaPhotonFactory::BadPhotonWeight
    ); 
}
//------------------------------------------------------------------------------
TEST_F(CorsikaPhotonFactoryTest, mctracer_accepts_photon_weight_equal_1) {
 
    const uint id = 1337;
    Random::Mt19937 prng(Random::zero_seed);
    const std::vector<float> corsika_photon = 
    	{1.2, 3.4, 0.0, 0.0, 1e-9, 1e5, 1.0, 433};
    
    EXPECT_NO_THROW(
    	CorsikaPhotonFactory cpf(corsika_photon, id, &prng)
    ); 
}
//------------------------------------------------------------------------------
TEST_F(CorsikaPhotonFactoryTest, mctracer_rejects_photon_weight_above_1) {
 
    const uint id = 1337;
    Random::Mt19937 prng(Random::zero_seed);
    const std::vector<float> corsika_photon = 
    	{1.2, 3.4, 0.0, 0.0, 1e-9, 1e5, 1.1, 433};
    
    EXPECT_THROW(
    	CorsikaPhotonFactory cpf(corsika_photon, id, &prng), 
    	CorsikaPhotonFactory::BadPhotonWeight
    ); 
}
//------------------------------------------------------------------------------
TEST_F(CorsikaPhotonFactoryTest, mctracer_accepts_photon_weight_equal_0) {
 
    const uint id = 1337;
    Random::Mt19937 prng(Random::zero_seed);
    const std::vector<float> corsika_photon = 
    	{1.2, 3.4, 0.0, 0.0, 1e-9, 1e5, 0.0, 433};
    
    EXPECT_NO_THROW(
    	CorsikaPhotonFactory cpf(corsika_photon, id, &prng)
    ); 
}
//------------------------------------------------------------------------------
TEST_F(CorsikaPhotonFactoryTest, mctracer_accepts_photon_weight_btw_0_and_1) {
 
    const uint id = 1337;
    Random::Mt19937 prng(Random::zero_seed);
    const std::vector<float> corsika_photon = 
    	{1.2, 3.4, 0.0, 0.0, 1e-9, 1e5, 0.4455, 433};
    
    EXPECT_NO_THROW(
    	CorsikaPhotonFactory cpf(corsika_photon, id, &prng)
    ); 
}
//------------------------------------------------------------------------------
TEST_F(CorsikaPhotonFactoryTest, zero_weight_is_passed_on_zero_from_prng) {
 
    const std::vector<float> corsika_photon = 
    {1.2, 3.4, 0.0, 0.0, 1e-9, 1e5, 0.0, 433};
    
    const uint id = 1337;
    Random::FakeConstant prng(0.0);

   	CorsikaPhotonFactory cpf(corsika_photon, id, &prng);

   	EXPECT_TRUE(cpf.passed_atmosphere());
}
//------------------------------------------------------------------------------
TEST_F(CorsikaPhotonFactoryTest, bad_shape) {
 
    const std::vector<float> corsika_photon = {1.2, 3.4, 0.0};
    
    const uint id = 1337;
    Random::Mt19937 prng(Random::zero_seed);

    EXPECT_THROW(
    	CorsikaPhotonFactory cpf(corsika_photon, id, &prng), 
    	CorsikaPhotonFactory::BadCorsikaPhotonShape
    );   
}
//------------------------------------------------------------------------------
TEST_F(CorsikaPhotonFactoryTest, z_emission_height) {
 
    const std::vector<float> corsika_photon = 
    {1.2, 3.4, 0.0, 0.0, 1e-9, 1e5, 0.4455, 433};
    
    const uint id = 1337;
    Random::FakeConstant prng(0.0);
	
	CorsikaPhotonFactory cpf(corsika_photon, id, &prng);

    ASSERT_TRUE(cpf.passed_atmosphere());
    EXPECT_EQ(1e3, cpf.get_photon()->Support().z());
}
//------------------------------------------------------------------------------