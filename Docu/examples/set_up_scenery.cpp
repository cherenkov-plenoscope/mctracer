#include "gtest/gtest.h"

#include "Triangle.h"
#include "Plane.h"
#include "Geometry/Sphere.h"
#include "Cylinder.h"
#include "Disc.h"
#include "SegmetedReflectorGenerator.h"
#include "Core/Vector3D.h"
#include "Core/Rotation3D.h"
#include "Core/Color.h"
#include "XmlIO/XmlFileIo.h"
#include "TracerException.h"
#include "PhotonSensor.h"
#include "TelescopeArrayControl.h"
#include "SphereCapWithHexagonalBound.h"
#include "SphereCapWithCylinderBound.h"
#include "FreeOrbitCamera.h"
#include "Core/Function/Func1DFunction.h"
#include "Core/Function/ConstantFunction.h"
#include "Core/Function/Polynom3Function.h"
#include "Core/Function/ConcatFunction.h"
#include "Tools/AsciiIo.h"

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

    Color leaf_green(0, 128, 0);
    Sphere leaf_ball;
    leaf_ball.set_name_pos_rot("leaf_ball", Vector3D(0.0, 0.0, 2.0), Rotation3D::null);
    leaf_ball.set_outer_color(&leaf_green);
    leaf_ball.set_sphere_radius(0.5);

    Color wood_brown(64, 64, 0);
    Cylinder tree_pole;
    tree_pole.set_name_pos_rot("tree_pole", Vector3D(0.0, 0.0, 0.5), Rotation3D::null);
    tree_pole.set_outer_color(&wood_brown);
    tree_pole.set_radius_and_length(0.1, 1.0);

    tree.set_mother_and_child(&leaf_ball);
    tree.set_mother_and_child(&tree_pole); 
    //--house--
    Vector3D house_pos(0.0, -5.0, 0.0);
    Frame house;
    house.set_name_pos_rot("house", house_pos, Rotation3D::null);

    Color wall_white(225, 225, 225);
    Color roof_red(225, 64, 64);
    Color foundation_gray(32, 32, 32);

    Plane foundation;
    foundation.set_name_pos_rot("foundation", Vector3D(0.0, 0.0, 0.01), Rotation3D::null);
    foundation.set_outer_color(&foundation_gray);
    foundation.set_inner_color(&foundation_gray);
    foundation.set_x_y_width(4.5, 4.5);

    Plane front_wall;
    front_wall.set_name_pos_rot("wall1", Vector3D(0.0, 0.0, 0.0), Rotation3D::null);

    world.set_mother_and_child(&tree);

    /*@Post initializing@*/
    world.init_tree_based_on_mother_child_relations();

    /*@//free like up to $\infty$@*/

    TracerSettings propagation_settings;
    FreeOrbitCamera cam(&world, &propagation_settings);
    //--end_set_up_scene_in_source--
}
//--using_namespace--
using namespace Function;
//--using_namespace_end--
//------------------------------------------------------------------------------
TEST_F(SetUpScenery, function_limits) {

    //--func_limits--
    Limits our_limits(0.0, 1.0);
    //--func_limits_assert--
    EXPECT_THROW( our_limits.assert_contains(-0.1), Limits::OutOfRange );
    EXPECT_NO_THROW( our_limits.assert_contains(0.0) );
    EXPECT_NO_THROW( our_limits.assert_contains(0.5) );
    EXPECT_THROW( our_limits.assert_contains(1.0), Limits::OutOfRange );
    //--func_limits_constant--
    Constant our_const(1.337, our_limits);
    //--func_limits_const_call--
    EXPECT_THROW( our_const(-0.1), Limits::OutOfRange );
    EXPECT_NO_THROW( our_const(0.0) );
    EXPECT_NO_THROW( our_const(0.5) );
    EXPECT_THROW( our_const(1.0), Limits::OutOfRange );
    //--func_limits_call_end--
}
//------------------------------------------------------------------------------
TEST_F(SetUpScenery, function_constant) {

    //--func_const--
    Constant c(1.337, Limits(0.0, 1.0));
    //--func_const_call--
    EXPECT_EQ( 1.337, c(0.0) );
    EXPECT_EQ( 1.337, c(0.2) );
    EXPECT_EQ( 1.337, c(0.3) );
    EXPECT_EQ( 1.337, c(0.43657657) );
    EXPECT_EQ( 1.337, c(0.78) );
    EXPECT_EQ( 1.337, c(0.9999) );
    //--func_const_call_end--

    AsciiIo::write_table_to_file(c.get_samples(1000),"function_const.func");
}
//------------------------------------------------------------------------------
TEST_F(SetUpScenery, function_polynom_1) {

    //--func_poly3--
    Polynom3 p3(0.0, 0.0, 1.0, 0.0, Limits(0.0, 1.0));
    //--func_poly3_call--
    EXPECT_EQ( 0.43657657, p3(0.43657657) );
    EXPECT_EQ( 0.78, p3(0.78) );
    EXPECT_EQ( 0.9999, p3(0.9999) );
    //--func_poly3_call_end--
    AsciiIo::write_table_to_file(p3.get_samples(1000),"function_polynom1.func");
}
//------------------------------------------------------------------------------
TEST_F(SetUpScenery, function_polynom_2) {

    //--func_poly3_quad--
    Polynom3 p3(0.0, 1.0, 0.0, 0.0, Limits(0.0, 1.0));
   
    EXPECT_NEAR( 0.0, p3(0.0) ,1e-9);
    EXPECT_NEAR( 0.25, p3(0.5) ,1e-9);
    EXPECT_NEAR( 0.04, p3(0.2) ,1e-9);
    //--func_poly3_quad_end--
    AsciiIo::write_table_to_file(p3.get_samples(1000),"function_polynom2.func");
}
//------------------------------------------------------------------------------
TEST_F(SetUpScenery, function_concat) {

    //--func_concat--
    Polynom3 f1(0.0, 1.0, 0.0, 0.0, Limits(-1.5, -0.5));
    Polynom3 f2(0.0, 0.0, 2.0, 0.0, Limits(-0.5, 0.5));
    Polynom3 f3(0.0, -1.0, 0.0, 0.0, Limits(0.5, 1.5));

    std::vector<const Func1D*> funcs = {&f1, &f2, &f3};
    Concat concat(funcs);
    //--func_concat_end--
    AsciiIo::write_table_to_file(concat.get_samples(1000),"function_concat.func");
}