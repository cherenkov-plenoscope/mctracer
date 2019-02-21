// Copyright 2015 Sebastian A. Mueller
#include <sstream>
#include <string>
#include "gtest/gtest.h"
#include "Core/Frame.h"
#include "Core/Vec3.h"
#include "Core/Rot3.h"
#include "Core/Ray.h"
#include "Core/Photon.h"
#include "Visual/CameraRay.h"
#include "Core/Color.h"
#include "Core/HomTra3.h"
#include "Core/SurfaceEntity.h"
#include "Scenery/Primitive/Primitive.h"
#include "Core/PropagationEnvironment.h"
#include "Tools/StringTools.h"
#include "PhotonSensor/PhotonSensor.h"
#include "SignalProcessing/PipelinePhoton.h"
using std::cout;
using std::string;
using namespace relleums;

template<typename T>
void print_size(const string &name, T s) {
    cout << StringTools::fill_up_text_with_whitespaces_until_column(name, 36);
    cout << sizeof(s) << '\n';
}

class SizeOfTest : public ::testing::Test {};

TEST_F(SizeOfTest, sizes_of_mct) {
    cout << "Size in bytes\n";
    cout << "--------------------------------------------" << "\n";

    Color col(123, 65, 12);
    print_size("Color", col);

    Vec2 v2;
    print_size("Vec2", v2);

    Vec3 v3;
    print_size("Vec3", v3);

    Rot3 r;
    print_size("Rot3", r);

    HomTra3 h;
    print_size("HomTra3", h);
    cout << "--------------------------------------------" << "\n";

    Ray ray(Vec3::ORIGIN, Vec3::UNIT_Z);
    print_size("Ray", ray);

    visual::CameraRay camray(Vec3::ORIGIN, Vec3::UNIT_Z);
    print_size("CameraRay", camray);

    RayForPropagation rayfp(Vec3::ORIGIN, Vec3::UNIT_Z);
    print_size("RayForPropagation", rayfp);

    Photon ph(Vec3::ORIGIN, Vec3::UNIT_Z, 433e-9);
    print_size("Photon", ph);
    cout << "--------------------------------------------" << "\n";

    Intersection is;
    print_size("Intersection", is);

    PropagationEnvironment environment =
        PropagationEnvironment::VOID_ENVIRONMENT;
    print_size("PropagationEnvironment", environment);
    cout << "--------------------------------------------" << "\n";

    Frame tree;
    tree.set_name_pos_rot("0123456789", Vec3::ORIGIN, Rot3::UNITY);
    print_size("Frame", tree);

    SurfaceEntity surf("0123456789", Vec3::ORIGIN, Rot3::UNITY);
    print_size("SurfaceEntity", surf);

    Annulus annulus;
    print_size("Annulus", annulus);

    BiConvexLens biConvexLens;
    print_size("BiConvexLens", biConvexLens);

    BiConvexLensHexBound biConvexLensHexBound;
    print_size("BiConvexLensHexBound", biConvexLensHexBound);

    Cylinder cylinder;
    print_size("Cylinder", cylinder);

    Disc disc;
    print_size("Disc", disc);

    EllipticalCapWithHexagonalBound ellipticalCapWithHexagonalBound;
    print_size(
        "EllipticalCapWithHexagonalBound", ellipticalCapWithHexagonalBound);

    HexPlane hexPlane;
    print_size("HexPlane", hexPlane);

    Plane plane;
    print_size("Plane", plane);

    PlaneDualSphericalBound planeDualSphericalBound;
    print_size("PlaneDualSphericalBound", planeDualSphericalBound);

    Sphere sphere;
    print_size("Sphere", sphere);

    SphereCapWithCylinderBound sphereCapWithCylinderBound;
    print_size("SphereCapWithCylinderBound", sphereCapWithCylinderBound);

    SphereCapWithHexagonalBound sphereCapWithHexagonalBound;
    print_size("SphereCapWithHexagonalBound", sphereCapWithHexagonalBound);

    Triangle triangle;
    print_size("Triangle", triangle);
    cout << "--------------------------------------------" << "\n";

    PhotonSensor::PhotonArrival arr_info;
    print_size("PhotonArrival", arr_info);

    PhotonSensor::Sensor sensor(0u, &tree);
    print_size("Sensor", sensor);

    std::vector<PhotonSensor::Sensor*> raw_sensors;
    PhotonSensor::Sensors sensors(raw_sensors);
    print_size("Sensors", sensors);

    PhotonSensor::FindSensorByFrame finder(&tree, &sensors.by_frame);
    print_size("FindSensorByFrame", finder);
    cout << "--------------------------------------------" << "\n";

    SignalProcessing::PipelinePhoton pipe_ph;
    print_size("PipelinePhoton", pipe_ph);
}
