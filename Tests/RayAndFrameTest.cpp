#include "gtest/gtest.h"
#include "Core/RayAndFrame.h"
#include "Geometry/Sphere.h"

using namespace RayAndFrame;

class RayAndFrameBoundingSphereTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(RayAndFrameBoundingSphereTest, frame_has_bounding_sphere) {

    Sphere sphere("sphere", Vec3::null, Rot3::null);
    sphere.set_radius(1.0);
    EXPECT_EQ(1.0, sphere.contour_radius());
}
//------------------------------------------------------------------------------
TEST_F(RayAndFrameBoundingSphereTest, support_inside_bounding_sphere) {
    /*                                                                                
    //                 __-----__                                 
    //                /         \                          
    //              /             \                          
    //              |      ______\|                                 
    //              |    (0,0,0) /|                                
    //              \             /                          
    //                \__     __/                          
    //                   -----                          
    */ 
    Sphere sphere("sphere", Vec3::null, Rot3::null);
    sphere.set_radius(1.0);

    Ray ray(Vec3::null, Vec3::unit_x);
    EXPECT_TRUE(ray_support_inside_frames_bounding_sphere(&ray, &sphere));
}
//------------------------------------------------------------------------------
TEST_F(RayAndFrameBoundingSphereTest, support_outside_bounding_sphere) {
    /*                                                                                
    //                 __-----__                                 
    //                /         \                          
    //              /             \                          
    //              |    (0,0,0)  |         _________\                          
    //              |    r=1      |       (+2,0,0)   /                          
    //              \             /                          
    //                \__     __/                          
    //                   -----                          
    */     
    Sphere sphere("sphere", Vec3::null, Rot3::null);
    sphere.set_radius(1.0);

    Ray ray(Vec3(2.0, 0.0, 0.0), Vec3::unit_x);
    EXPECT_FALSE(ray_support_inside_frames_bounding_sphere(&ray, &sphere));
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
TEST_F(RayAndFrameBoundingSphereTest, frontal_hit) {
    /*
    //                 __-----__
    //                /         \
    //              /             \
    //    _________\|    (0,0,0)  |
    //  (-2,0,0)   /|    r=1      |
    //              \             /
    //                \__     __/
    //                   -----
    */                      
    Sphere sphere("sphere", Vec3::null, Rot3::null);
    sphere.set_radius(1.0);

    Ray ray(Vec3(-2.0, 0.0, 0.0), Vec3::unit_x);
    EXPECT_TRUE(ray_has_intersection_with_bounding_sphere_of(&ray, &sphere));
}
//------------------------------------------------------------------------------
TEST_F(RayAndFrameBoundingSphereTest, no_hit) {
    /*                                                                                
    //                 __-----__                                 
    //                /         \                          
    //              /             \                          
    //              |    (0,0,0)  |         _________\                          
    //              |    r=1      |       (+2,0,0)   /                          
    //              \             /                          
    //                \__     __/                          
    //                   -----                          
    */                                                
    Sphere sphere("sphere", Vec3::null, Rot3::null);
    sphere.set_radius(1.0);

    Ray ray(Vec3(+2.0, 0.0, 0.0), Vec3::unit_x);
    EXPECT_FALSE(ray_has_intersection_with_bounding_sphere_of(&ray, &sphere));
}
//------------------------------------------------------------------------------
TEST_F(RayAndFrameBoundingSphereTest, hit_inside_bounding_sphere) {
    /*                                                                                
    //                 __-----__                                 
    //                /         \                          
    //              /             \                          
    //              |      ______\|                                 
    //              |    (0,0,0) /|                                
    //              \             /                          
    //                \__     __/                          
    //                   -----                          
    */                                                
    Sphere sphere("sphere", Vec3::null, Rot3::null);
    sphere.set_radius(1.0);

    Ray ray(Vec3::null, Vec3::unit_x);
    EXPECT_TRUE(ray_has_intersection_with_bounding_sphere_of(&ray, &sphere));
}
//------------------------------------------------------------------------------
TEST_F(RayAndFrameBoundingSphereTest, frontal_hits) {
    /*
    //    _________\
    // (-5,+2.0,0) /
    //    .           __-----__
    //    .          /         \
    //    .        /             \
    //    ________\|    (0,0,0)  |
    // (-5,0,0)   /|    r=1      |
    //    .        \             /
    //    .          \__     __/
    //    .             -----
    //    _________\
    // (-5,-2.0,0) /
    */
    
    const double sphere_radius = 1.0;
    Sphere sphere("sphere", Vec3::null, Rot3::null);
    sphere.set_radius(sphere_radius);

    for(double y=-2.0; y<2.0; y+=0.002) {
    
        Ray ray(Vec3(-5.0, y, 0.0), Vec3::unit_x);

        if(fabs(y) > sphere_radius)
            EXPECT_FALSE(ray_has_intersection_with_bounding_sphere_of(&ray, &sphere));
        else
            EXPECT_TRUE(ray_has_intersection_with_bounding_sphere_of(&ray, &sphere));
    }
}
//------------------------------------------------------------------------------