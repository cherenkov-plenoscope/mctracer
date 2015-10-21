#include "gtest/gtest.h"
#include "Core/ReflectionProperties.h"

class ReflectionPropertiesTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(ReflectionPropertiesTest, ConstructorAndGetter) {

    const double coef = 0.1337;
    ReflectionProperties refl(coef);
    EXPECT_EQ(coef, refl() );
}
//------------------------------------------------------------------------------
TEST_F(ReflectionPropertiesTest, NonNegative) {
    
    EXPECT_THROW(ReflectionProperties refl(-0.5);, TracerException);
}
//------------------------------------------------------------------------------