#include "gtest/gtest.h"
#include "Core/HomTra3.h"

class HomTra3Test : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(HomTra3Test, default_is_unit_trafo) {
    HomTra3 trafo;

    HomTra3 unit_frafo;
    unit_frafo.set_transformation(
        Vec3(1,0,0), 
        Vec3(0,1,0), 
        Vec3(0,0,1), Vec3(0,0,0)
    );

    EXPECT_EQ(unit_frafo, trafo);
}
//------------------------------------------------------------------------------
TEST_F(HomTra3Test, unit_trafo_has_no_translation) {
    HomTra3 trafo;

    // translation should be zero 
    EXPECT_EQ(Vec3::null, trafo.get_translation());
}
//------------------------------------------------------------------------------
TEST_F(HomTra3Test, unit_trafo_has_no_rotation) {
    HomTra3 trafo;

    // and rotation should be unity as well
    Vec3 unit_x(1.0, 0.0, 0.0);
    EXPECT_EQ(unit_x, trafo.get_transformed_orientation(unit_x));

    Vec3 unit_y(0.0, 1.0, 0.0);
    EXPECT_EQ(unit_y, trafo.get_transformed_orientation(unit_y));

    Vec3 unit_z(0.0, 0.0, 1.0);
    EXPECT_EQ(unit_z, trafo.get_transformed_orientation(unit_z));
}
//------------------------------------------------------------------------------
TEST_F(HomTra3Test, unit_trafo_is_its_own_inverse) {
    HomTra3 trafo;

    HomTra3 inverse_trafo = trafo;
    inverse_trafo.inverse();

    EXPECT_EQ(inverse_trafo, trafo);
}
//------------------------------------------------------------------------------
TEST_F(HomTra3Test, translation_without_rotation) {
    Vec3 translation(0.0, 0.0, 133.7);
    Rot3 rotation(0.0, 0.0, 0.0);

    HomTra3 frame1_to_frame2;
    frame1_to_frame2.set_transformation(rotation, translation);

    Vec3 my_vector_in_frame1(3.141, 4.2, 0.0);
    Vec3 my_vector_in_frame2(3.141, 4.2, 133.7);

    EXPECT_EQ(
        my_vector_in_frame2, 
        frame1_to_frame2.get_transformed_position(my_vector_in_frame1)
    );

    // and also the other/inverse way round
    HomTra3 frame2_to_frame1 = frame1_to_frame2.inverse();

    EXPECT_EQ(
        my_vector_in_frame1, 
        frame2_to_frame1.get_transformed_position(my_vector_in_frame2)
    );    
}
//------------------------------------------------------------------------------