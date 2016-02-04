#include "Xml/Xml.h"

class XmlTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(XmlTest, open_not_existing_file) {
	
	const std::string path = "test_scenery/nein_nein.xml";	
	EXPECT_THROW(Xml::Document doc(path), Xml::IoError);
}
//------------------------------------------------------------------------------
TEST_F(XmlTest, path_name) {

	const std::string path = "test_scenery/loop_over_children.xml";

	Xml::Document doc(path);

	Xml::Node tree = doc.get_tree();

	for(Xml::Node child = tree.get_first_child(); child; child = child.get_next_child())
		std::cout << attribute_as_double(child, "name") << "\n";
}