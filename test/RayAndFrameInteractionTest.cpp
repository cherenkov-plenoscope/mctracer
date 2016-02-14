#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "Core/Ray.h"
#include "Core/Intersection.h"
#include "Geometry/Sphere.h"

using namespace std;

class RayAndFrameInteractionTest : public ::testing::Test{
protected:

    TracerSettings settings;    

    RayAndFrameInteractionTest() {
        settings.max_number_of_interactions_per_photon = 5;
    }

    Frame* spheres_r1m_in_a_row(
        const uint number_of_spheres,
        Vector3D start,
        const double distance_in_between_spheres_in_m
    ) {

    Frame *spheres_in_a_row_along_x_r1m;
    spheres_in_a_row_along_x_r1m = new Frame;

    spheres_in_a_row_along_x_r1m->set_name_pos_rot(
        "ten_in_a_row", 
        Vector3D::null,
        Rotation3D::null
    );

    Color* color; 
    color = new Color(200,25,25);

    for(uint i=0; i<number_of_spheres; i++) {
        Sphere* sphere = new Sphere;

        stringstream name; name << "sphere_number_" << i+1;
        sphere->set_name_pos_rot(
        name.str(),
        start + Vector3D(distance_in_between_spheres_in_m*i,0.0,0.0),
        Rotation3D::null
        );

        sphere->set_sphere_radius(1.0);
        sphere->set_outer_color(color);
        sphere->set_inner_color(color);  
        spheres_in_a_row_along_x_r1m->set_mother_and_child(sphere);
    }

    spheres_in_a_row_along_x_r1m->init_tree_based_on_mother_child_relations();

    return spheres_in_a_row_along_x_r1m;
    }

    virtual ~RayAndFrameInteractionTest() {
    }

    virtual void SetUp() {
    }

    virtual void TearDown() {
    }
};
//------------------------------------------------------------------------------
TEST_F(RayAndFrameInteractionTest, frame_has_bounding_sphere) {
    
    Frame* spheres_in_a_row = spheres_r1m_in_a_row(1440, Vector3D::null,2.5);
    Ray ray(Vector3D::null,Vector3D(1.0,0.0,0.0));

    for(uint i=0; i<1e3; i++) {

        std::vector<const Frame*> frames_with_intersection_in_bounding_sphere;
    
        ray.find_intersection_candidates_in_tree_of_frames(
        spheres_in_a_row,
        &frames_with_intersection_in_bounding_sphere
        );

        EXPECT_EQ(1440, frames_with_intersection_in_bounding_sphere.size());
    }
    //FreeOrbitCamera free(spheres_in_a_row, &settings);
}
//------------------------------------------------------------------------------
TEST_F(RayAndFrameInteractionTest, ray_finds_first_interaction) {

    Ray ray(Vector3D::null,Vector3D(1.0,0.0,0.0));

    Intersection i0(
        SurfaceEntity::void_object,
        Vector3D(0.,0.,0.),        // intersection in world
        Vector3D::unit_z,
        1.,                        // <-- distance, !! the only thing we need for this test
        Vector3D::null
    );

    Intersection i1(
        SurfaceEntity::void_object,
        Vector3D(0.,0.,0.), // intersection in world
        Vector3D::unit_z,
        2.,   // <-- distance, !! the only thing we need for this test
        Vector3D::null
    );
    
    Intersection i2(
        SurfaceEntity::void_object,
        Vector3D(0.,0.,0.), // intersection in world
        Vector3D::unit_z,
        3.,   // <-- distance, !! the only thing we need for this test
        Vector3D::null
    );

    vector<const Intersection*> intersections = {&i0, &i1, &i2};

    const Intersection* maybe_closest_i = ray.calculate_closest_intersection(&intersections);

    EXPECT_EQ(&i0, maybe_closest_i);
}