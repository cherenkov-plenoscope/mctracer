// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "merlict/Vec3.h"
#include "merlict/scenery/stereo_litography.h"
namespace ml = merlict;


TEST_CASE("StereoLitographyTest: open_non_existing_file", "[merlict]") {
    CHECK_THROWS_AS(ml::stereo_litography::BinaryReader("not_existing_file"), ml::stereo_litography::BinaryReader::CanNotReadFile);
}

TEST_CASE("StereoLitographyTest: ascii_format", "[merlict]") {
    CHECK_THROWS_AS(ml::stereo_litography::BinaryReader("test_scenery/ascii_format.stl"), ml::stereo_litography::BinaryReader::CanNotReadAscii);
}

TEST_CASE("StereoLitographyTest: open_valid_file", "[merlict]") {
    ml::stereo_litography::BinaryReader stl_reader("test_scenery/LCCone-simple_parab.stl");
    CHECK(stl_reader.get_facets().size() == 3692u);
}

TEST_CASE("StereoLitographyTest: write_and_read_stl_with_zero_triangles", "[merlict]") {
    std::string filename = "test_scenery/single_triangle.stl";
    ml::stereo_litography::BinaryWriter stlwr;
    stlwr.write_to_file(filename);
    ml::stereo_litography::BinaryReader stl_reader(filename);
    CHECK(stl_reader.get_facets().size() == 0u);
}

TEST_CASE("StereoLitographyTest: write_and_read_1000_triangle", "[merlict]") {
    unsigned int number_of_triangles = 1000;
    std::vector<ml::stereo_litography::Facet> facets;

    for (unsigned int i = 0; i < number_of_triangles; i++) {
        double offset = static_cast<double>(i)/
            static_cast<double>(number_of_triangles);
        ml::stereo_litography::Facet facet = {
            ml::Vec3(0.0, 0.0, 1.0),
            ml::Vec3(0.0, 0.0, 0.0),
            ml::Vec3(offset+1.0, 0.0, 0.0),
            ml::Vec3(0.0, offset+1.0, offset+1.0)
        };
        facets.push_back(facet);
    }

    std::string filename = "test_scenery/single_triangle.stl";
    ml::stereo_litography::BinaryWriter writer;
    writer.add_facets(facets);
    writer.write_to_file(filename);

    ml::stereo_litography::BinaryReader reder(filename);

    std::vector<ml::stereo_litography::Facet> facets_from_file = reder.get_facets();
    REQUIRE(facets_from_file.size() == facets.size());

    for (unsigned int i = 0; i < facets.size(); i++) {
        CHECK(facets_from_file.at(i).n == facets.at(i).n);
        CHECK(facets_from_file.at(i).a == facets.at(i).a);
        CHECK(facets_from_file.at(i).b == facets.at(i).b);
        CHECK(facets_from_file.at(i).c == facets.at(i).c);
    }
}

TEST_CASE("StereoLitographyTest: scaling", "[merlict]") {
    const double scaleing = 42.1337;
    ml::Frame obj_normal;
    obj_normal.set_name_pos_rot("normal", ml::VEC3_ORIGIN, ml::ROT3_UNITY);
    ml::stereo_litography::add_stl_to_frame("test_scenery/LCCone-simple_parab.stl", &obj_normal);

    ml::Frame obj_scaled;
    obj_scaled.set_name_pos_rot("scaled", ml::VEC3_ORIGIN, ml::ROT3_UNITY);
    ml::stereo_litography::add_stl_to_frame(
        "test_scenery/LCCone-simple_parab.stl",
        &obj_scaled,
        scaleing);

    CHECK(obj_scaled.get_bounding_sphere_radius() == scaleing*obj_normal.get_bounding_sphere_radius());
}

TEST_CASE("StereoLitographyTest: write_and_read_simple_stl", "[merlict]") {
    std::string filename = "test_scenery/single_triangle.stl";
    ml::stereo_litography::BinaryWriter stlwr;

    ml::Vec3 n(0.0, 0.0, 1.0);
    ml::Vec3 a(0.0, 0.0, 0.0);
    ml::Vec3 b(1.0, 0.0, 0.0);
    ml::Vec3 c(0.0, 1.0, 1.0);

    stlwr.add_facet_normal_and_three_vertices(n, a, b, c);
    stlwr.write_to_file(filename);

    ml::stereo_litography::BinaryReader stl_reader(filename);
    CHECK(stl_reader.get_facets().size() == 1u);
}
