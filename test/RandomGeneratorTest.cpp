#include "gtest/gtest.h"
#include "Core/PseudoRandomNumberGenerator.h"

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