#include "gtest/gtest.h"
#include "Core/Frame.h"
#include "Core/Vec3.h"
#include "Core/Rot3.h"
#include "Core/Ray.h"
#include "Core/Photon.h"
#include "Cameras/CameraRay.h"
#include "Core/Color.h"
#include "Core/HomTra3.h"
#include "Core/SurfaceEntity.h"
#include "Geometry/Geometry.h"
#include "Core/PropagationEnvironment.h"
#include "Tools/StringTools.h"
using std::cout;
using StringTools::fill_up_text_with_whitespaces_until_column;

class SizeOfTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(SizeOfTest, sizes_of_mct) {

    cout << "Size in bytes\n";

    const uint c = 36;

    Vec3 v;
    cout << fill_up_text_with_whitespaces_until_column("Vec3", c) << sizeof(v) << "\n";

    Rot3 r;
    cout << fill_up_text_with_whitespaces_until_column("Rot3", c) << sizeof(r) << "\n";

    HomTra3 h;
    cout << fill_up_text_with_whitespaces_until_column("HomTra3", c) << sizeof(h) << "\n";

    Ray ray(Vec3::null, Vec3::unit_z);
    cout << fill_up_text_with_whitespaces_until_column("Ray", c) << sizeof(ray) << "\n";

    CameraRay camray(Vec3::null, Vec3::unit_z);
    cout << fill_up_text_with_whitespaces_until_column("CameraRay", c) << sizeof(camray) << "\n";

    RayForPropagation rayfp(Vec3::null, Vec3::unit_z);
    cout << fill_up_text_with_whitespaces_until_column("RayForPropagation", c) << sizeof(rayfp) << "\n";

    Photon ph(Vec3::null, Vec3::unit_z, 433e-9);
    cout << fill_up_text_with_whitespaces_until_column("Photon", c) << sizeof(ph) << "\n";

    Intersection is = Intersection::void_intersection;
    cout << fill_up_text_with_whitespaces_until_column("Intersection", c) << sizeof(is) << "\n";

    Frame tree("0123456789", Vec3::null, Rot3::null);
    cout << fill_up_text_with_whitespaces_until_column("Frame", c) << sizeof(tree) << "\n";

    SurfaceEntity surf("0123456789", Vec3::null, Rot3::null);
    cout << fill_up_text_with_whitespaces_until_column("SurfaceEntity", c) << sizeof(surf) << "\n";

    Annulus annulus;
    cout << fill_up_text_with_whitespaces_until_column("Annulus", c) << sizeof(annulus) << "\n";

    BiConvexLens biConvexLens;
    cout << fill_up_text_with_whitespaces_until_column("BiConvexLens", c) << sizeof(biConvexLens) << "\n";

    BiConvexLensHexBound biConvexLensHexBound;
    cout << fill_up_text_with_whitespaces_until_column("BiConvexLensHexBound", c) << sizeof(biConvexLensHexBound) << "\n";

    Cylinder cylinder;
    cout << fill_up_text_with_whitespaces_until_column("Cylinder", c) << sizeof(cylinder) << "\n";

    Disc disc;
    cout << fill_up_text_with_whitespaces_until_column("Disc", c) << sizeof(disc) << "\n";

    EllipticalCapWithHexagonalBound ellipticalCapWithHexagonalBound;
    cout << fill_up_text_with_whitespaces_until_column("EllipticalCapWithHexagonalBound   ", c) << sizeof(ellipticalCapWithHexagonalBound) << "\n";

    HexPlane hexPlane;
    cout << fill_up_text_with_whitespaces_until_column("HexPlane", c) << sizeof(hexPlane) << "\n";

    Plane plane;
    cout << fill_up_text_with_whitespaces_until_column("Plane", c) << sizeof(plane) << "\n";

    Sphere sphere;
    cout << fill_up_text_with_whitespaces_until_column("Sphere", c) << sizeof(sphere) << "\n";

    SphereCapWithHexagonalBound sphereCapWithHexagonalBound;
    cout << fill_up_text_with_whitespaces_until_column("SphereCapWithHexagonalBound", c) << sizeof(sphereCapWithHexagonalBound) << "\n";

    Triangle triangle;
    cout << fill_up_text_with_whitespaces_until_column("Triangle", c) << sizeof(triangle) << "\n";

    PropagationEnvironment environment = PropagationEnvironment::default_environment;
    cout << fill_up_text_with_whitespaces_until_column("PropagationEnvironment", c) << sizeof(environment) << "\n";

    Color col(123, 65, 12);
    cout << fill_up_text_with_whitespaces_until_column("Color", c) << sizeof(col) << "\n";     
}
//------------------------------------------------------------------------------