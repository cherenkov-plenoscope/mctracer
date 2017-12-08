#include "gtest/gtest.h"
#include "Scenery/Primitive/Primitive.h"
#include "Visual/FlyingCamera.h"
#include <vector>
#include <string>
using std::string;
using std::vector;

class OwnerShipTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(OwnerShipTest, own) {

	Frame tree;
	tree.set_name_pos_rot("tree", Vec3::ORIGIN, Rot3::UNITY);

	Sphere* sphere = tree.append<Sphere>();
	sphere->set_name_pos_rot("leaf_ball", Vec3(0,0,1.8), Rot3::UNITY);
	sphere->set_radius(1);
	sphere->set_outer_color(&Color::GREEN);

	Frame* pole = tree.append<Frame>();
	pole->set_name_pos_rot("pole", Vec3(0,0,0.5), Rot3::UNITY);

	Cylinder* pole1 = pole->append<Cylinder>();
	pole1->set_name_pos_rot("pole1", Vec3(0,0,0.0), Rot3::UNITY);
	pole1->set_radius_and_length(0.25, 1.0);
	pole1->set_outer_color(&Color::RED);

	Cylinder* fork = pole->append<Cylinder>();
	fork->set_name_pos_rot("fork", Vec3(0,0,0), Rot3::UNITY);
	fork->set_cylinder(0.1, Vec3::ORIGIN, Vec3(0,0.5,0.5));
	fork->set_outer_color(&Color::RED);

	Disc* ground = tree.append<Disc>();
	ground->set_name_pos_rot("ground", Vec3(0,0,0), Rot3::UNITY);
	ground->set_radius(3.0);
	ground->set_outer_color(&Color::GRASS_GREEN);

	tree.init_tree_based_on_mother_child_relations();

	//PropagationConfig set;
	//FlyingCamera fly(&tree, &set);
}