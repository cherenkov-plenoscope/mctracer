#include "gtest/gtest.h"
#include "Core/Vector3D.h"
#include "StereoLitographyIo.h"

using namespace StereoLitographyIo;

class StereoLitographyIoTest : public ::testing::Test {};
//----------------------------------------------------------------------
TEST_F(StereoLitographyIoTest, open_non_existing_file) {

    EXPECT_THROW( 
        BinaryReader hans("not_existing_file"),
        BinaryReader::CanNotReadFile
    );
}
//----------------------------------------------------------------------
TEST_F(StereoLitographyIoTest, ascii_format) {

    EXPECT_THROW( 
        BinaryReader stl("test_scenery/ascii_format.stl"),
        BinaryReader::CanNotReadAscii
    );
}
//----------------------------------------------------------------------
TEST_F(StereoLitographyIoTest, open_valid_file) {

    BinaryReader stl("test_scenery/LCCone-simple_parab.stl");
    EXPECT_EQ(3692, stl.get_facets().size());
}
//----------------------------------------------------------------------
TEST_F(StereoLitographyIoTest, write_and_read_stl_with_zero_triangles) {

    std::string filename = "test_scenery/single_triangle.stl";

    BinaryWriter stlwr;
    stlwr.write_to_file(filename);

    BinaryReader stl(filename);
    EXPECT_EQ(0,  stl.get_facets().size());
}
//----------------------------------------------------------------------
TEST_F(StereoLitographyIoTest, write_and_read_1000_triangle) {

    uint number_of_triangles = 1000;
    std::vector<Facet> facets;

    for(uint i=0; i<number_of_triangles; i++) {

        double offset = double(i)/double(number_of_triangles);

        Facet facet = {
            Vector3D(0.0, 0.0, 1.0),
            Vector3D(0.0, 0.0, 0.0),
            Vector3D(offset+1.0, 0.0, 0.0),
            Vector3D(0.0, offset+1.0, offset+1.0)
        };

        facets.push_back(facet);
    }

    std::string filename = "test_scenery/single_triangle.stl";
    
    BinaryWriter writer;
    writer.add_facets(facets);
    writer.write_to_file(filename);

    BinaryReader reder(filename);

    std::vector<Facet> facets_from_file = reder.get_facets();
    
    ASSERT_EQ(facets.size(), facets_from_file.size());

    for(uint i=0; i<facets.size(); i++) {
        EXPECT_EQ(facets.at(i).n, facets_from_file.at(i).n);
        EXPECT_EQ(facets.at(i).a, facets_from_file.at(i).a);
        EXPECT_EQ(facets.at(i).b, facets_from_file.at(i).b);
        EXPECT_EQ(facets.at(i).c, facets_from_file.at(i).c);
    }
}
//----------------------------------------------------------------------
TEST_F(StereoLitographyIoTest, scaling) {

    double scaleing = 42.1337;

    Frame* obj_normal = read("test_scenery/LCCone-simple_parab.stl");
    Frame* obj_scaled = read("test_scenery/LCCone-simple_parab.stl", scaleing);

    EXPECT_EQ(
        scaleing*obj_normal->get_radius_of_sphere_enclosing_all_children(),
        obj_scaled->get_radius_of_sphere_enclosing_all_children()
    );
}
//----------------------------------------------------------------------
TEST_F(StereoLitographyIoTest, write_and_read_simple_stl) {

    std::string filename = "test_scenery/single_triangle.stl";

    BinaryWriter stlwr;

    Vector3D n(0.0, 0.0, 1.0);
    Vector3D a(0.0, 0.0, 0.0);
    Vector3D b(1.0, 0.0, 0.0);
    Vector3D c(0.0, 1.0, 1.0);   

    stlwr.add_facet_normal_and_three_vertices(n, a, b, c);
    stlwr.write_to_file(filename);

    BinaryReader stl(filename);
    EXPECT_EQ(1, stl.get_facets().size());
}