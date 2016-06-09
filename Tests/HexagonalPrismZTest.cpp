#include "gtest/gtest.h"
#include "Scenery/HexagonalPrismZ.h"

using namespace std;

class HexagonalPrismZTest : public ::testing::Test {
};
//------------------------------------------------------------------------------
TEST_F(HexagonalPrismZTest, throw_when_negativ_radius) {
	HexagonalPrismZ hexBound;
  	EXPECT_THROW({ hexBound.set_outer_radius(-55.0);}, TracerException);
}
//------------------------------------------------------------------------------
TEST_F(HexagonalPrismZTest, throw_when_zero_radius) {
	HexagonalPrismZ hexBound;
 	EXPECT_THROW({ hexBound.set_outer_radius(0.0);}, TracerException);
}
//------------------------------------------------------------------------------
TEST_F(HexagonalPrismZTest, is_inside) {

	double outer_radius = 1.0;
	double inner_radius = outer_radius * cos( M_PI/6.0 );

	HexagonalPrismZ hexBound;
	hexBound.set_outer_radius(1.0);

	Vec3 vec(0.0, 0.0, 0.0);
	EXPECT_TRUE(hexBound.is_inside(&vec));

	vec.set(0.0, inner_radius-0.01, 0.0);
	EXPECT_TRUE(hexBound.is_inside(&vec));

	vec.set(0.0, inner_radius+0.01, 0.0);
	EXPECT_FALSE(hexBound.is_inside(&vec));

	vec.set(outer_radius-0.01, 0.0, 0.0);
	EXPECT_TRUE(hexBound.is_inside(&vec));

	vec.set(outer_radius+0.01, 0.0, 0.0);
	EXPECT_FALSE(hexBound.is_inside(&vec));

	vec.set(120.0, 5.0, 0.0);
	EXPECT_FALSE(hexBound.is_inside(&vec));
}