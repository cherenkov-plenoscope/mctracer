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