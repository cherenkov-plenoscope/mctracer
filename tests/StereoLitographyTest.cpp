// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "merlict/Vec3.h"
#include "merlict/scenery/stereo_litography.h"

using namespace relleums;
namespace stl = stereo_litography;



TEST_CASE("StereoLitographyTest: open_non_existing_file", "[mctracer]") {
    CHECK_THROWS_AS(stl::BinaryReader("not_existing_file"), stl::BinaryReader::CanNotReadFile);
}

TEST_CASE("StereoLitographyTest: ascii_format", "[mctracer]") {
    CHECK_THROWS_AS(stl::BinaryReader("test_scenery/ascii_format.stl"), stl::BinaryReader::CanNotReadAscii);
}

TEST_CASE("StereoLitographyTest: open_valid_file", "[mctracer]") {
    stl::BinaryReader stl("test_scenery/LCCone-simple_parab.stl");
    CHECK(stl.get_facets().size() == 3692u);
}

TEST_CASE("StereoLitographyTest: write_and_read_stl_with_zero_triangles", "[mctracer]") {
    std::string filename = "test_scenery/single_triangle.stl";
    stl::BinaryWriter stlwr;
    stlwr.write_to_file(filename);
    stl::BinaryReader stl(filename);
    CHECK(stl.get_facets().size() == 0u);
}

TEST_CASE("StereoLitographyTest: write_and_read_1000_triangle", "[mctracer]") {
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
    REQUIRE(facets_from_file.size() == facets.size());

    for (unsigned int i = 0; i < facets.size(); i++) {
        CHECK(facets_from_file.at(i).n == facets.at(i).n);
        CHECK(facets_from_file.at(i).a == facets.at(i).a);
        CHECK(facets_from_file.at(i).b == facets.at(i).b);
        CHECK(facets_from_file.at(i).c == facets.at(i).c);
    }
}

TEST_CASE("StereoLitographyTest: scaling", "[mctracer]") {
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

    CHECK(obj_scaled.get_bounding_sphere_radius() == scaleing*obj_normal.get_bounding_sphere_radius());
}

TEST_CASE("StereoLitographyTest: write_and_read_simple_stl", "[mctracer]") {
    std::string filename = "test_scenery/single_triangle.stl";
    stl::BinaryWriter stlwr;

    Vec3 n(0.0, 0.0, 1.0);
    Vec3 a(0.0, 0.0, 0.0);
    Vec3 b(1.0, 0.0, 0.0);
    Vec3 c(0.0, 1.0, 1.0);

    stlwr.add_facet_normal_and_three_vertices(n, a, b, c);
    stlwr.write_to_file(filename);

    stl::BinaryReader stl(filename);
    CHECK(stl.get_facets().size() == 1u);
}
