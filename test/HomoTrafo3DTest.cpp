#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "Core/HomoTrafo3D.h"

class HomoTrafo3DTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(HomoTrafo3DTest, default_is_unit_trafo) {
    HomoTrafo3D trafo;

    HomoTrafo3D unit_frafo;
    unit_frafo.set_unity();

    EXPECT_EQ(unit_frafo, trafo);
}
//------------------------------------------------------------------------------
TEST_F(HomoTrafo3DTest, unit_trafo_has_no_translation) {
    HomoTrafo3D trafo;
    trafo.set_unity();

    // translation should be zero 
    EXPECT_EQ(Vector3D::null, trafo.get_translation());
}
//------------------------------------------------------------------------------
TEST_F(HomoTrafo3DTest, unit_trafo_has_no_rotation) {
    HomoTrafo3D trafo;
    trafo.set_unity();

    // and rotation should be unity as well
    Vector3D unit_x(1.0, 0.0, 0.0);
    EXPECT_EQ(unit_x, trafo.get_transformed_orientation(unit_x));

    Vector3D unit_y(0.0, 1.0, 0.0);
    EXPECT_EQ(unit_y, trafo.get_transformed_orientation(unit_y));

    Vector3D unit_z(0.0, 0.0, 1.0);
    EXPECT_EQ(unit_z, trafo.get_transformed_orientation(unit_z));
}
//------------------------------------------------------------------------------
TEST_F(HomoTrafo3DTest, unit_trafo_is_its_own_inverse) {
    HomoTrafo3D trafo;
    trafo.set_unity();

    HomoTrafo3D inverse_trafo = trafo;
    inverse_trafo.inverse();

    EXPECT_EQ(inverse_trafo, trafo);
}
//------------------------------------------------------------------------------
TEST_F(HomoTrafo3DTest, translation_without_rotation) {
    Vector3D translation(0.0, 0.0, 133.7);
    Rotation3D rotation(0.0, 0.0, 0.0);

    HomoTrafo3D frame1_to_frame2;
    frame1_to_frame2.set_transformation(rotation, translation);

    Vector3D my_vector_in_frame1(3.141, 4.2, 0.0);
    Vector3D my_vector_in_frame2(3.141, 4.2, 133.7);

    EXPECT_EQ(
        my_vector_in_frame2, 
        frame1_to_frame2.get_transformed_position(my_vector_in_frame1)
    );

    // and also the other/inverse way round
    HomoTrafo3D frame2_to_frame1 = frame1_to_frame2.inverse();

    EXPECT_EQ(
        my_vector_in_frame1, 
        frame2_to_frame1.get_transformed_position(my_vector_in_frame2)
    );    
}
//------------------------------------------------------------------------------