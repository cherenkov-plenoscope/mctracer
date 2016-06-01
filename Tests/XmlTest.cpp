#include "gtest/gtest.h"
#include "Xml/Xml.h"
#include "Tools/FileTools.h"

class XmlTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(XmlTest, empty_xml_path) {
	
	const std::string path = "";	
	EXPECT_THROW(Xml::Document doc(path), Xml::IoError);
}
//------------------------------------------------------------------------------
TEST_F(XmlTest, open_not_existing_file) {
	
	const std::string path = "xml/nein_nein.xml";	
	EXPECT_THROW(Xml::Document doc(path), Xml::IoError);
}
//------------------------------------------------------------------------------
TEST_F(XmlTest, loop_over_children) {

	const std::string path = "xml/loop_over_children.xml";

	Xml::Document doc(path);
	Xml::Node tree = doc.node();

	std::vector<std::string> children_names = {
		"child1", "child2", "child3", "dude"
	};

	int i=0;
	for(Xml::Node child=tree.first_child(); child; child=child.next_child())
		EXPECT_EQ(children_names.at(i++), child.name());
}
//------------------------------------------------------------------------------
TEST_F(XmlTest, minimal_node) {
	
	const std::string path = "xml/minimal_node_name_attribute.xml";	

	Xml::Document doc(path);
	Xml::Node tree = doc.node();

	Xml::Node function = tree.child("function");

	EXPECT_EQ("function", function.name());
	EXPECT_EQ("zero", function.attribute("name"));
}
//------------------------------------------------------------------------------
TEST_F(XmlTest, valid_attributes) {
	
	const std::string path = "xml/valid_attributes.xml";	

	Xml::Document doc(path);
	Xml::Node tree = doc.node();

	Xml::Node simon = tree.child("simon");

	// <simon 
	// 	name="zero" 
	// 	number="1.337e42" 
	// 	color="[128,255,128]" 
	// 	pos="[0.0, 1.0, 2.0]" 
	// 	rot="[0.1, 2.3, 4.5]"
	// />
	EXPECT_EQ("simon", simon.name());
	EXPECT_EQ(1.337e42, simon.attribute2double("number"));
	EXPECT_EQ(Vec3(0.0, 1.0, 2.0), simon.attribute2Vec3("pos"));
	EXPECT_EQ(Rot3(0.1, 2.3, 4.5), simon.attribute2Rot3("rot"));
	EXPECT_EQ(Color(128,255,128), simon.attribute2Color("color"));
}
//------------------------------------------------------------------------------
TEST_F(XmlTest, invalid_attribute_double) {
	
	const std::string path = "xml/invalid_attribute_double.xml";	
	Xml::Document doc(path);
	Xml::Node tree = doc.node();
	Xml::Node simon = tree.child("simon");
	EXPECT_THROW(
		simon.attribute2double("number"), Xml::Node::AttributeIsNoDouble
	);
}
//------------------------------------------------------------------------------
TEST_F(XmlTest, invalid_attribute_Tuple3) {
	
	const std::string path = "xml/invalid_attribute_Tuple3.xml";	
	Xml::Document doc(path);
	Xml::Node tree = doc.node();
	Xml::Node my_node = tree.child("MyNode");
	
	EXPECT_THROW(
		my_node.attribute2Vec3("myTuple3"), Xml::Node::AttributeIsNoVec3
	);

	EXPECT_THROW(
		my_node.attribute2Rot3("myTuple3"), Xml::Node::AttributeIsNoRot3
	);

	EXPECT_THROW(
		my_node.attribute2Color("myTuple3"), Xml::Node::AttributeIsNoColor
	);
}
//------------------------------------------------------------------------------
#include "Xml/Factory/VisualConfigFab.h"
TEST_F(XmlTest, visual_config) {

	const std::string path = "xml/visual_config.xml";
	VisualConfig out;

	out.preview.rows = 1337;
	out.preview.cols = 42;
	out.preview.scale = 6;

	out.snapshot.rows = 137;
	out.snapshot.cols = 42;
	out.snapshot.rays_per_pixel = 55;
	out.snapshot.focal_length_over_aperture_diameter = 2.8;
	out.snapshot.image_sensor_size_along_a_row = 0.060;

	out.global_illumination.on = false;
	out.global_illumination.incoming_direction = Vec3(4.2, 1.3, 3.7);

	out.photon_trajectories.radius = 99.9;

	FileTools::write_text_to_file(Xml::Configs::to_node(out), path);

	Xml::Document doc(path);
	Xml::Node node = doc.node();
	Xml::Node vc_node = node.child("visual");
	VisualConfig in = Xml::Configs::get_VisualConfig_from_node(vc_node);

	EXPECT_EQ(out.preview.cols, in.preview.cols);
	EXPECT_EQ(out.preview.rows, in.preview.rows);
	EXPECT_EQ(out.preview.scale, in.preview.scale);
	
	EXPECT_EQ(out.snapshot.cols, in.snapshot.cols);
	EXPECT_EQ(out.snapshot.rows, in.snapshot.rows);
	EXPECT_EQ(out.snapshot.rays_per_pixel, in.snapshot.rays_per_pixel);
	EXPECT_EQ(
		out.snapshot.focal_length_over_aperture_diameter, 
		in.snapshot.focal_length_over_aperture_diameter);
	EXPECT_EQ(
		out.snapshot.image_sensor_size_along_a_row, 
		in.snapshot.image_sensor_size_along_a_row);

	EXPECT_EQ(out.global_illumination.on, in.global_illumination.on);
	EXPECT_EQ(
		out.global_illumination.incoming_direction, 
		in.global_illumination.incoming_direction);
}
//------------------------------------------------------------------------------
#include "Xml/Factory/FunctionFab.h"
TEST_F(XmlTest, functions) {

	const std::string path = "xml/functions.xml";

	FunctionFab fab;

	Xml::Document doc(path);
	Xml::Node tree = doc.node();
	
	Xml::Node child = tree.first_child(); 
	ASSERT_TRUE(child);
	EXPECT_EQ("function", child.name());
	fab.add(child);

	child = child.next_child();
	ASSERT_TRUE(child);
	EXPECT_EQ("function", child.name());
	fab.add(child);

	child = child.next_child();
	ASSERT_TRUE(child);
	EXPECT_EQ("function", child.name());
	fab.add(child);

	child = child.next_child();
	ASSERT_TRUE(child);
	EXPECT_EQ("function", child.name());
	fab.add(child);

	child = child.next_child();
	EXPECT_FALSE(child);

	/*
	<function name="my_funny_function">
		<linear_interpolation file="leaf_reflection_vs_wavelength.txt"/>
	</function>

	100e-9	0.1
	110e-9	0.1
	120e-9	0.9
	160e-9	0.9
	180e-9	0.1
	200e-9	0.1

	<function name="constant_function">
		<constant value="1.337" upper_limit="400e-9" lower_limit="200e-9"/>
	</function>

	<function name="polynom_function">
		<polynom3 x3="1.0" x2="0.0" x1="1.2" x0="-1.7" upper_limit="1200e-9" lower_limit="400e-9"/>
	</function>

	<function name="concat_function">
		<concatenation f0="my_funny_function" f1="constant_function" f2="polynom_function" />
		<export samples="1000" file="concat_sample.txt"/>
	</function>
	*/

	const Function::Func1D* func;
	func = fab.by_name("my_funny_function");
	EXPECT_EQ(100e-9, func->get_limits().get_lower());
	EXPECT_EQ(200e-9, func->get_limits().get_upper());
	EXPECT_NEAR(0.9, (*func)(160e-9), 1e-6);

	func = fab.by_name("constant_function");
	EXPECT_EQ(200e-9, func->get_limits().get_lower());
	EXPECT_EQ(400e-9, func->get_limits().get_upper());
	EXPECT_NEAR(1.337, (*func)(300e-9), 1e-6);

	func = fab.by_name("polynom_function");
	EXPECT_EQ(400e-9, func->get_limits().get_lower());
	EXPECT_EQ(1200e-9, func->get_limits().get_upper());
	double x = 600e-9;
	EXPECT_NEAR(1.0*x*x*x + 0.0*x*x + 1.2*x - 1.7, (*func)(x), 1e-6);

	func = fab.by_name("concat_function");
	EXPECT_EQ(100e-9, func->get_limits().get_lower());
	EXPECT_EQ(1200e-9, func->get_limits().get_upper());
}
#include "Xml/Factory/TracerSettingsFab.h"
TEST_F(XmlTest, TracerSettings) {

	const std::string path = "xml/tracer_settings.xml";

	TracerSettings out;

	out.max_number_of_interactions_per_photon = 1337;
	out.pseudo_random_number_seed = 42;
	out.use_multithread_when_possible = false;

	FileTools::write_text_to_file(Xml::Configs::to_node(out), path);

	Xml::Document doc(path);
	Xml::Node node = doc.node();
	TracerSettings in = Xml::Configs::get_TracerSettings_from_node(node.child("settings"));

	EXPECT_EQ(in.max_number_of_interactions_per_photon, out.max_number_of_interactions_per_photon);
	EXPECT_EQ(in.pseudo_random_number_seed, out.pseudo_random_number_seed);
	EXPECT_EQ(in.use_multithread_when_possible, out.use_multithread_when_possible);
}
