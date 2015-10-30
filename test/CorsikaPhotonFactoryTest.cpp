#include "gtest/gtest.h"
#include "MmcsCorsikaFileIO/CorsikaPhotonFactory.h"

class CorsikaPhotonFactoryTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(CorsikaPhotonFactoryTest, convert_photons) {
      
    const std::vector<float> corsika_photon = 
    	{1.2, 3.4, 0.0, 0.0, 1e-9, 1e5, 1.0, 433};
    //   x    y    xcos ycos time  zem  weight lambda
    //   cm   cm   1    1    ns    cm   1    nm
    
    const uint id = 1337;
    PseudoRandomNumberGenerator prng(0);

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
     
    double absorbed = 0;
    double passed = 0;
    double total = 1e5;

    PseudoRandomNumberGenerator prng(0);

    for(double i=0; i<total; i++) {

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