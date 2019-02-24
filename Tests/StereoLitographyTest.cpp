// Copyright 2014 Sebastian A. Mueller
#include "gtest/gtest.h"
#include "Core/Vec3.h"
#include "scenery/stereo_litography.h"

using namespace relleums;
namespace stl = stereo_litography;

class StereoLitographyTest : public ::testing::Test {};

TEST_F(StereoLitographyTest, open_non_existing_file) {
    EXPECT_THROW(
        stl::BinaryReader hans("not_existing_file"),
        stl::BinaryReader::CanNotReadFile);
}

TEST_F(StereoLitographyTest, ascii_format) {
    EXPECT_THROW(
        stl::BinaryReader stl("test_scenery/ascii_format.stl"),
        stl::BinaryReader::CanNotReadAscii);
}

TEST_F(StereoLitographyTest, open_valid_file) {
    stl::BinaryReader stl("test_scenery/LCCone-simple_parab.stl");
    EXPECT_EQ(3692u, stl.get_facets().size());
}

TEST_F(StereoLitographyTest, write_and_read_stl_with_zero_triangles) {
    std::string filename = "test_scenery/single_triangle.stl";
    stl::BinaryWriter stlwr;
    stlwr.write_to_file(filename);
    stl::BinaryReader stl(filename);
    EXPECT_EQ(0u,  stl.get_facets().size());
}

TEST_F(StereoLitographyTest, write_and_read_1000_triangle) {
    unsigned int number_of_triangles = 1000;
    std::vector<stl::Facet> facets;

    for (unsigned int i = 0; i < number_of_triangles; i++) {
        double offset = static_cast<double>(i)/
            static_cast<double>(number_of_triangles);
        stl::Facet facet = {
            Vec3(0.0, 0.0, 1.0),
            Vec3(0.0, 0.0, 0.0),
            Vec3(offset+1.0, 0.0, 0.0),
            Vec3(0.0, offset+1.0, offset+1.0)
        };
        facets.push_back(facet);
    }

    std::string filename = "test_scenery/single_triangle.stl";
    stl::BinaryWriter writer;
    writer.add_facets(facets);
    writer.write_to_file(filename);

    stl::BinaryReader reder(filename);

    std::vector<stl::Facet> facets_from_file = reder.get_facets();
    ASSERT_EQ(facets.size(), facets_from_file.size());

    for (unsigned int i = 0; i < facets.size(); i++) {
        EXPECT_EQ(facets.at(i).n, facets_from_file.at(i).n);
        EXPECT_EQ(facets.at(i).a, facets_from_file.at(i).a);
        EXPECT_EQ(facets.at(i).b, facets_from_file.at(i).b);
        EXPECT_EQ(facets.at(i).c, facets_from_file.at(i).c);
    }
}

TEST_F(StereoLitographyTest, scaling) {
    const double scaleing = 42.1337;
    Frame obj_normal;
    obj_normal.set_name_pos_rot("normal", VEC3_ORIGIN, ROT3_UNITY);
    stl::add_stl_to_frame("test_scenery/LCCone-simple_parab.stl", &obj_normal);

    Frame obj_scaled;
    obj_scaled.set_name_pos_rot("scaled", VEC3_ORIGIN, ROT3_UNITY);
    stl::add_stl_to_frame(
        "test_scenery/LCCone-simple_parab.stl",
        &obj_scaled,
        scaleing);

    EXPECT_EQ(
        scaleing*obj_normal.get_bounding_sphere_radius(),
        obj_scaled.get_bounding_sphere_radius());
}

TEST_F(StereoLitographyTest, write_and_read_simple_stl) {
    std::string filename = "test_scenery/single_triangle.stl";
    stl::BinaryWriter stlwr;

    Vec3 n(0.0, 0.0, 1.0);
    Vec3 a(0.0, 0.0, 0.0);
    Vec3 b(1.0, 0.0, 0.0);
    Vec3 c(0.0, 1.0, 1.0);

    stlwr.add_facet_normal_and_three_vertices(n, a, b, c);
    stlwr.write_to_file(filename);

    stl::BinaryReader stl(filename);
    EXPECT_EQ(1u, stl.get_facets().size());
}
