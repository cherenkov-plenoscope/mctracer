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
#include "Scenery/Primitive/Primitive.h"
#include "Core/PropagationEnvironment.h"
#include "Tools/StringTools.h"
#include "PhotonSensor/PhotonSensor.h"

#include "SignalProcessing/PipelinePhoton.h"
using std::cout;
using StringTools::fill_up_text_with_whitespaces_until_column;

template<typename T>
void print_size(const string &name, T s) {
    cout << fill_up_text_with_whitespaces_until_column(name, 36) << sizeof(s) << '\n';
}

class SizeOfTest : public ::testing::Test {};
//------------------------------------------------------------------------------
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

    Ray ray(Vec3::null, Vec3::unit_z); 
    print_size("Ray", ray);

    CameraRay camray(Vec3::null, Vec3::unit_z); 
    print_size("CameraRay", camray);

    RayForPropagation rayfp(Vec3::null, Vec3::unit_z); 
    print_size("RayForPropagation", rayfp);

    Photon ph(Vec3::null, Vec3::unit_z, 433e-9); 
    print_size("Photon", ph);
    cout << "--------------------------------------------" << "\n";

    Intersection is = Intersection::void_intersection; 
    print_size("Intersection", is);

    PropagationEnvironment environment = PropagationEnvironment::default_environment; 
    print_size("PropagationEnvironment", environment);
    cout << "--------------------------------------------" << "\n";

    Frame tree;
    tree.set_name_pos_rot("0123456789", Vec3::null, Rot3::null); 
    print_size("Frame", tree);

    SurfaceEntity surf("0123456789", Vec3::null, Rot3::null); 
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
    print_size("EllipticalCapWithHexagonalBound", ellipticalCapWithHexagonalBound);

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

    PhotonSensor::ArrivalInformation arr_info;
    print_size("ArrivalInformation", arr_info);
    cout << "--------------------------------------------" << "\n";

    SignalProcessing::PipelinePhoton pipe_ph; 
    print_size("PipelinePhoton", pipe_ph);
}
//------------------------------------------------------------------------------