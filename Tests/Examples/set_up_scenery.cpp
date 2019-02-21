#include "Visual/FlyingCamera.h"
#include "Core/Color.h"
#include "Core/Function/Function.h"
#include "Core/Rot3.h"
#include "Core/Vec3.h"
#include "Scenery/Primitive/Primitive.h"
#include "gtest/gtest.h"
#include "Tools/AsciiIo.h"

using namespace relleums;

class SetUpScenery : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(SetUpScenery, create_scenery) {
    //--set_up_scene_in_source--
    /*@Declare the geometry@*/

    //--world--
    Frame world;
    world.set_name_pos_rot("World", VEC3_ORIGIN, Rot3::UNITY);
    //--tree--
    Vec3 tree_pos(5.0, 0.0, 0.0);
    Frame* tree = world.append<Frame>();
    tree->set_name_pos_rot("My_Tree", tree_pos, Rot3::UNITY);

    Color leaf_green(0, 128, 0);
    Sphere* leaf_ball = tree->append<Sphere>();
    leaf_ball->set_name_pos_rot("leaf_ball", Vec3(0.0, 0.0, 2.0), Rot3::UNITY);
    leaf_ball->set_outer_color(&leaf_green);
    leaf_ball->set_radius(0.5);

    Color wood_brown(64, 64, 0);
    Cylinder* tree_pole = tree->append<Cylinder>();
    tree_pole->set_name_pos_rot("tree_pole", Vec3(0.0, 0.0, 0.5), Rot3::UNITY);
    tree_pole->set_outer_color(&wood_brown);
    tree_pole->set_radius_and_length(0.1, 1.0);

    //--house--
    Vec3 house_pos(0.0, -5.0, 0.0);
    Frame* house = world.append<Frame>();
    house->set_name_pos_rot("house", house_pos, Rot3::UNITY);

    Color wall_white(225, 225, 225);
    Color roof_red(225, 64, 64);
    Color foundation_gray(32, 32, 32);

    Plane* foundation = house->append<Plane>();
    foundation->set_name_pos_rot("foundation", Vec3(0.0, 0.0, 0.01), Rot3::UNITY);
    foundation->set_outer_color(&foundation_gray);
    foundation->set_inner_color(&foundation_gray);
    foundation->set_x_y_width(4.5, 4.5);

    Plane* front_wall = house->append<Plane>();
    front_wall->set_name_pos_rot("wall1", Vec3(0.0, 0.0, 0.0), Rot3::UNITY);

    /*@Post initializing@*/
    world.init_tree_based_on_mother_child_relations();

    /*@//free like up to $\infty$@*/

    visual::Config visual_config;
    //visual::FlyingCamera cam(&world, &visual_config);
    //--end_set_up_scene_in_source--
}
//--using_namespace--
using namespace Function;
//--using_namespace_end--
//------------------------------------------------------------------------------