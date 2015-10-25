#include "gtest/gtest.h"
#include "Core/Vector3D.h"
#include "Geometry/StlReader.h"

class StlReaderTest : public ::testing::Test {};
//----------------------------------------------------------------------
TEST_F(StlReaderTest, open_non_existing_file) {

    EXPECT_THROW( StlReader hans("not_existing_file"), StlReader::CanNotReadSTL);
}
//----------------------------------------------------------------------
TEST_F(StlReaderTest, open_valid_file) {

    StlReader stl("test_scenery/LCCone-simple_parab.stl");
    EXPECT_EQ(3692, stl.get_number_of_triangles());
}
//----------------------------------------------------------------------
TEST_F(StlReaderTest, scaling) {

    double scaleing = 42.1337;

    StlReader stl("test_scenery/LCCone-simple_parab.stl");
    EXPECT_EQ(3692, stl.get_number_of_triangles());
    Frame* obj_normal = stl.get_frame();

    StlReader stl2("test_scenery/LCCone-simple_parab.stl", scaleing);
    EXPECT_EQ(3692, stl2.get_number_of_triangles());
    Frame* obj_scaled = stl2.get_frame();

    EXPECT_EQ(
        scaleing*obj_normal->get_radius_of_sphere_enclosing_all_children(),
        obj_scaled->get_radius_of_sphere_enclosing_all_children()
    );
}
//----------------------------------------------------------------------

