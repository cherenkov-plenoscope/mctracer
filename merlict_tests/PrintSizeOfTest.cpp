// Copyright 2015 Sebastian A. Mueller
#include <sstream>
#include <string>
#include "merlict/tests/catch.hpp"
#include "merlict/merlict.h"
#include "merlict_visual/visual.h"
#include "signal_processing/signal_processing.h"
namespace ml = merlict;


template<typename T>
void print_size(const std::string &name, T s) {
    std::cout << ml::txt::fill_up_text_with_whitespaces_until_column(name, 36);
    std::cout << sizeof(s) << '\n';
}


TEST_CASE("SizeOfTest: sizes_of_mct", "[merlict]") {
    std::cout << "Size in bytes\n";
    std::cout << "--------------------------------------------" << "\n";

    ml::Color col(123, 65, 12);
    print_size("Color", col);

    ml::Vec2 v2;
    print_size("Vec2", v2);

    ml::Vec3 v3;
    print_size("Vec3", v3);

    ml::Rot3 r;
    print_size("Rot3", r);

    ml::HomTra3 h;
    print_size("HomTra3", h);
    std::cout << "--------------------------------------------" << "\n";

    ml::Ray ray(ml::VEC3_ORIGIN, ml::VEC3_UNIT_Z);
    print_size("Ray", ray);

    ml::visual::CameraRay camray(ml::VEC3_ORIGIN, ml::VEC3_UNIT_Z);
    print_size("CameraRay", camray);

    ml::RayForPropagation rayfp(ml::VEC3_ORIGIN, ml::VEC3_UNIT_Z);
    print_size("RayForPropagation", rayfp);

    ml::Photon ph(ml::VEC3_ORIGIN, ml::VEC3_UNIT_Z, 433e-9);
    print_size("Photon", ph);
    std::cout << "--------------------------------------------" << "\n";

    ml::Intersection is;
    print_size("Intersection", is);

    ml::PropagationEnvironment environment;
    print_size("PropagationEnvironment", environment);
    std::cout << "--------------------------------------------" << "\n";

    ml::Frame tree;
    tree.set_name_pos_rot("0123456789", ml::VEC3_ORIGIN, ml::ROT3_UNITY);
    print_size("Frame", tree);

    ml::SurfaceEntity surf("0123456789", ml::VEC3_ORIGIN, ml::ROT3_UNITY);
    print_size("SurfaceEntity", surf);

    ml::Annulus annulus;
    print_size("Annulus", annulus);

    ml::BiConvexLens biConvexLens;
    print_size("BiConvexLens", biConvexLens);

    ml::BiConvexLensHexBound biConvexLensHexBound;
    print_size("BiConvexLensHexBound", biConvexLensHexBound);

    ml::Cylinder cylinder;
    print_size("Cylinder", cylinder);

    ml::Disc disc;
    print_size("Disc", disc);

    ml::EllipticalCapWithHexagonalBound ellipticalCapWithHexagonalBound;
    print_size(
        "EllipticalCapWithHexagonalBound", ellipticalCapWithHexagonalBound);

    ml::HexPlane hexPlane;
    print_size("HexPlane", hexPlane);

    ml::Plane plane;
    print_size("Plane", plane);

    ml::PlaneDualSphericalBound planeDualSphericalBound;
    print_size("PlaneDualSphericalBound", planeDualSphericalBound);

    ml::Sphere sphere;
    print_size("Sphere", sphere);

    ml::SphereCapWithCylinderBound sphereCapWithCylinderBound;
    print_size("SphereCapWithCylinderBound", sphereCapWithCylinderBound);

    ml::SphereCapWithHexagonalBound sphereCapWithHexagonalBound;
    print_size("SphereCapWithHexagonalBound", sphereCapWithHexagonalBound);

    ml::Triangle triangle;
    print_size("Triangle", triangle);
    std::cout << "--------------------------------------------" << "\n";

    ml::sensor::PhotonArrival arr_info;
    print_size("PhotonArrival", arr_info);

    ml::sensor::Sensor sensor(0u, &tree);
    print_size("Sensor", sensor);

    std::vector<ml::sensor::Sensor*> raw_sensors;
    ml::sensor::Sensors sensors(raw_sensors);
    print_size("Sensors", sensors);

    ml::sensor::FindSensorByFrame finder(&tree, &sensors.by_frame);
    print_size("FindSensorByFrame", finder);
    std::cout << "--------------------------------------------" << "\n";

    signal_processing::PipelinePhoton pipe_ph;
    print_size("PipelinePhoton", pipe_ph);
}
