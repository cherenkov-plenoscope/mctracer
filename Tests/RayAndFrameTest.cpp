#include "gtest/gtest.h"
#include "Core/RayAndFrame.h"
#include "Scenery/Primitive/Sphere.h"

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
class RayAndFrameTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(RayAndFrameTest, transform_into_unit_frame) {

    Frame frame("frame", Vec3::null, Rot3::null);
    frame.init_tree_based_on_mother_child_relations();

    Ray ray(Vec3::null, Vec3::unit_z);

    Ray ray_t = RayAndFrame::get_ray_transformed_in_object_system_of_frame(
        &ray, 
        &frame
    );

    EXPECT_EQ(ray_t.get_support(), Vec3::null);
    EXPECT_EQ(ray_t.get_direction(), Vec3::unit_z);
}
//------------------------------------------------------------------------------
TEST_F(RayAndFrameTest, transform_into_translated_frame) {

    Frame frame("frame", Vec3(0.0, 0.0, 13.37), Rot3::null);
    frame.init_tree_based_on_mother_child_relations();

    Ray ray(Vec3(0.0, 4.2, 0.0), Vec3::unit_z);

    Ray ray_t = RayAndFrame::get_ray_transformed_in_object_system_of_frame(
        &ray, 
        &frame
    );

    EXPECT_EQ(ray_t.get_support(), Vec3(0.0, 4.2, -13.37));
    EXPECT_EQ(ray_t.get_direction(), Vec3::unit_z);
}
//------------------------------------------------------------------------------
TEST_F(RayAndFrameTest, transform_ray_in_z_into_frame_rotated_in_z) {

    Frame frame("frame", Vec3::null, Rot3(0.0, 0.0, 0.5*M_PI));
    frame.init_tree_based_on_mother_child_relations();

    Ray ray(Vec3::null, Vec3::unit_z);

    Ray ray_t = RayAndFrame::get_ray_transformed_in_object_system_of_frame(
        &ray, 
        &frame
    );

    EXPECT_EQ(ray_t.get_support(), Vec3::null);
    EXPECT_EQ(ray_t.get_direction(), Vec3::unit_z);
}
//------------------------------------------------------------------------------
TEST_F(RayAndFrameTest, transform_ray_into_rotated_frame) {

    Frame frame("frame", Vec3::null, Rot3(0.0, 0.0, 0.5*M_PI));
    frame.init_tree_based_on_mother_child_relations();

    Ray ray(Vec3::null, Vec3::unit_x);

    Ray ray_t = RayAndFrame::get_ray_transformed_in_object_system_of_frame(
        &ray, 
        &frame
    );

    EXPECT_EQ(ray_t.get_support(), Vec3::null);
    EXPECT_EQ(ray_t.get_direction(), Vec3::unit_y);
}
//------------------------------------------------------------------------------
/*class RayAndFrameTest : public ::testing::Test {

};*/
//------------------------------------------------------------------------------
TEST_F(RayAndFrameTest, causal_intersection) {

    Frame spheres_in_a_row("spheres", Vec3::null, Rot3::null);

    Sphere* s0 = spheres_in_a_row.append<Sphere>();
    s0->set_name_pos_rot("s0", Vec3(0.0, 0.0,0.0), Rot3::null);
    s0->set_radius(1.0);
    Sphere* s1 = spheres_in_a_row.append<Sphere>();
    s1->set_name_pos_rot("s1", Vec3(0.0, 3.0,0.0), Rot3::null);
    s1->set_radius(1.0);
    Sphere* s2 = spheres_in_a_row.append<Sphere>();
    s2->set_name_pos_rot("s2", Vec3(0.0, 6.0,0.0), Rot3::null);
    s2->set_radius(1.0);
    Sphere* s3 = spheres_in_a_row.append<Sphere>();
    s3->set_name_pos_rot("s3", Vec3(0.0, 9.0,0.0), Rot3::null);
    s3->set_radius(1.0);
    Sphere* s4 = spheres_in_a_row.append<Sphere>();
    s4->set_name_pos_rot("s4", Vec3(0.0,12.0,0.0), Rot3::null);
    s4->set_radius(1.0);

    spheres_in_a_row.init_tree_based_on_mother_child_relations();
}