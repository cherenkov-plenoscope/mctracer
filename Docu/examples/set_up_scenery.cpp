#include "gtest/gtest.h"

#include "Triangle.h"
#include "Plane.h"
#include "Geometry/Sphere.h"
#include "Cylinder.h"
#include "Disc.h"
#include "SegmetedReflectorGenerator.h"
#include "Core/Vector3D.h"
#include "Core/Rotation3D.h"
#include "Core/ReflectionProperties.h"
#include "Core/Color.h"
#include "XmlIO/XmlFileIo.h"
#include "TracerException.h"
#include "PhotonSensor.h"
#include "TelescopeArrayControl.h"
#include "SphereCapWithHexagonalBound.h"
#include "SphereCapWithCylinderBound.h"
#include "FreeOrbitCamera.h"

class SetUpScenery : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(SetUpScenery, create_scenery) {
    //--set_up_scene_in_source--
    /*@Declare the geometry@*/

    //--world--
    Frame world;
    world.set_name_pos_rot("World", Vector3D::null, Rotation3D::null);
    //--tree--
    Vector3D tree_pos(5.0, 0.0, 0.0);
    Frame tree;
    tree.set_name_pos_rot("My_Tree", tree_pos, Rotation3D::null);
    //--leaf_ball--
    Color leaf_green(0, 128, 0);
    Sphere leaf_ball;
    leaf_ball.set_name_pos_rot("leaf_ball", Vector3D(0.0, 0.0, 2.0), Rotation3D::null);
    leaf_ball.set_outer_color(&leaf_green);
    leaf_ball.set_sphere_radius(1.0);
    //--tree_pole--
    Color wood_brown(64, 64, 0);
    Cylinder tree_pole;
    tree_pole.set_name_pos_rot("tree_pole", Vector3D(0.0, 0.0, 0.5), Rotation3D::null);
    tree_pole.set_outer_color(&wood_brown);

    /*@Declare relations between individual geometry@*/
    tree.set_mother_and_child(&leaf_ball);
    tree.set_mother_and_child(&tree_pole);

    world.set_mother_and_child(&tree);

    /*@Post initializing@*/
    world.init_tree_based_on_mother_child_relations();

    /*@free like up to $\infty$@*/

    TracerSettings propagation_settings;
    FreeOrbitCamera cam(&world, &propagation_settings);
    //--end_set_up_scene_in_source--
}
//------------------------------------------------------------------------------