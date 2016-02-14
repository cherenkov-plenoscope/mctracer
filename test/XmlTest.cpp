#include "Xml/Xml.h"

class XmlTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(XmlTest, open_not_existing_file) {
	
	const std::string path = "xml/nein_nein.xml";	
	EXPECT_THROW(Xml::Document doc(path), Xml::IoError);
}
//------------------------------------------------------------------------------
TEST_F(XmlTest, loop_over_children) {

	const std::string path = "xml/loop_over_children.xml";

	Xml::Document doc(path);
	Xml::Node tree = doc.get_tree();

	std::vector<std::string> children_names = {
		"child1", "child2", "child3", "dude"
	};

	int i=0;
	for(
		Xml::Node child = tree.get_first_child(); 
		child; 
		child = child.get_next_child()
	){
		EXPECT_EQ(children_names.at(i++), child.get_name());
	}
}
//------------------------------------------------------------------------------
TEST_F(XmlTest, minimal_node) {
	
	const std::string path = "xml/minimal_node_name_attribute.xml";	

	Xml::Document doc(path);
	Xml::Node tree = doc.get_tree();

	Xml::Node function = tree.get_child("function");

	EXPECT_EQ("function", function.get_name());
	EXPECT_EQ("zero", function.get_attribute("name"));
}
//------------------------------------------------------------------------------
TEST_F(XmlTest, valid_attributes) {
	
	const std::string path = "xml/valid_attributes.xml";	

	Xml::Document doc(path);
	Xml::Node tree = doc.get_tree();

	Xml::Node simon = tree.get_child("simon");

	// <simon 
	// 	name="zero" 
	// 	number="1.337e42" 
	// 	color="[128,255,128]" 
	// 	pos="[0.0, 1.0, 2.0]" 
	// 	rot="[0.1, 2.3, 4.5]"
	// />
	EXPECT_EQ("simon", simon.get_name());
	EXPECT_EQ(1.337e42, Xml::attribute_to_double(simon, "number"));
	EXPECT_EQ(Vector3D(0.0, 1.0, 2.0), Xml::attribute_to_Vector3D(simon, "pos"));
	EXPECT_EQ(Rotation3D(0.1, 2.3, 4.5), Xml::attribute_to_Rotation3D(simon, "rot"));
	EXPECT_EQ(Color(128,255,128), Xml::attribute_to_Color(simon, "color"));
}
//------------------------------------------------------------------------------
TEST_F(XmlTest, invalid_attribute_double) {
	
	const std::string path = "xml/invalid_attribute_double.xml";	
	Xml::Document doc(path);
	Xml::Node tree = doc.get_tree();
	Xml::Node simon = tree.get_child("simon");
	EXPECT_THROW(
		Xml::attribute_to_double(simon, "number"), 
		Xml::AttributeIsNoDouble
	);
}
//------------------------------------------------------------------------------
TEST_F(XmlTest, invalid_attribute_Tuple3) {
	
	const std::string path = "xml/invalid_attribute_Tuple3.xml";	
	Xml::Document doc(path);
	Xml::Node tree = doc.get_tree();
	Xml::Node my_node = tree.get_child("MyNode");
	
	EXPECT_THROW(
		Xml::attribute_to_Vector3D(my_node, "myTuple3"), 
		Xml::AttributeIsNoVector3D
	);

	EXPECT_THROW(
		Xml::attribute_to_Rotation3D(my_node, "myTuple3"), 
		Xml::AttributeIsNoRotation3D
	);

	EXPECT_THROW(
		Xml::attribute_to_Color(my_node, "myTuple3"), 
		Xml::AttributeIsNoColor
	);
}
//------------------------------------------------------------------------------
#include "XmlFactory/VisualConfig.h"
TEST_F(XmlTest, visual_config) {

	const std::string path = "xml/visual_config.xml";
	VisualConfig out;

	out.preview.rows = 1337;
	out.preview.cols = 42;

	out.snapshot.rows = 137;
	out.snapshot.cols = 42;
	out.snapshot.rays_per_pixel = 55;
	out.snapshot.focal_length_over_aperture_diameter = 2.8;
	out.snapshot.image_sensor_size_along_a_row = 0.060;

	out.global_illumination.on = false;
	out.global_illumination.incoming_direction = Vector3D(4.2, 1.3, 3.7);

	out.photon_trajectories.radius = 99.9;

	FileTools::write_text_to_file(Xml::Configs::to_node(out), path);

	Xml::Document doc(path);
	Xml::Node node = doc.get_tree();
	Xml::Node vc_node = node.get_child("visual");
	VisualConfig in = Xml::Configs::get_VisualConfig_from_node(vc_node);

	EXPECT_EQ(out.preview.cols, in.preview.cols);
	EXPECT_EQ(out.preview.rows, in.preview.rows);
	
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
