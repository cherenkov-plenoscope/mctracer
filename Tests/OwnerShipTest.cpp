#include "gtest/gtest.h"
#include "Scenery/Primitive/Primitive.h"
#include "Cameras/FlyingCamera.h"
#include <vector>
#include <string>
using std::string;
using std::vector;

class OwnerShipTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(OwnerShipTest, own) {

	Frame tree("tree", Vec3::null, Rot3::null);

	Sphere* sphere = tree.append<Sphere>();
	sphere->set_name_pos_rot("leaf_ball", Vec3(0,0,1.8), Rot3::null);
	sphere->set_radius(1);
	sphere->set_outer_color(&Color::green);

	Frame* pole = tree.append<Frame>();
	pole->set_name_pos_rot("pole", Vec3(0,0,0.5), Rot3::null);

	Cylinder* pole1 = pole->append<Cylinder>();
	pole1->set_name_pos_rot("pole1", Vec3(0,0,0.0), Rot3::null);
	pole1->set_radius_and_length(0.25, 1.0);
	pole1->set_outer_color(&Color::red);

	Cylinder* fork = pole->append<Cylinder>();
	fork->set_name_pos_rot("fork", Vec3(0,0,0), Rot3::null);
	fork->set_cylinder(0.1, Vec3::null, Vec3(0,0.5,0.5));
	fork->set_outer_color(&Color::red);

	Disc* ground = tree.append<Disc>();
	ground->set_name_pos_rot("ground", Vec3(0,0,0), Rot3::null);
	ground->set_radius(3.0);
	ground->set_outer_color(&Color::grass_green);

	tree.init_tree_based_on_mother_child_relations();

	TracerSettings set;
	FlyingCamera fly(&tree, &set);
}