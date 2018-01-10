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
    //  name="zero" 
    //  number="1.337e42" 
    //  color="[128,255,128]" 
    //  pos="[0.0, 1.0, 2.0]" 
    //  rot="[0.1, 2.3, 4.5]"
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
    Visual::Config out;

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
    Visual::Config in = Xml::Configs::get_VisualConfig_from_node(vc_node);

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
#include "Xml/Factory/Function.h"
#include "Scenery/Scenery.h"
TEST_F(XmlTest, functions) {

    const std::string path = "xml/functions.xml";

    Scenery scenery;

    Xml::Document doc(path);
    Xml::Node tree = doc.node();
    
    Xml::Node child = tree.first_child(); 
    ASSERT_TRUE(child);
    ASSERT_EQ("function", child.name());
    ASSERT_EQ("linear_interpolation", child.first_child().name());

    Function::LinInterpol* f1 = 
        scenery.functions.add<Function::LinInterpol>(child.attribute("name"));
    Xml::init_function_with_node(f1, child.first_child());

    child = child.next_child();
    ASSERT_TRUE(child);
    ASSERT_EQ("function", child.name());
    ASSERT_EQ("constant", child.first_child().name());

    Function::Constant* f2 = 
        scenery.functions.add<Function::Constant>(child.attribute("name"));
    Xml::init_function_with_node(f2, child.first_child());

    child = child.next_child();
    ASSERT_TRUE(child);
    ASSERT_EQ("function", child.name());
    ASSERT_EQ("polynom3", child.first_child().name());

    Function::Polynom3* f3 = 
        scenery.functions.add<Function::Polynom3>(child.attribute("name"));
    Xml::init_function_with_node(f3, child.first_child());

    child = child.next_child();
    ASSERT_TRUE(child);
    ASSERT_EQ("function", child.name());
    ASSERT_EQ("concatenation", child.first_child().name());

    Function::Concat* f4 = 
        scenery.functions.add<Function::Concat>(child.attribute("name"));
    Xml::init_function_with_node(f4, child.first_child(), &scenery.functions);  

    child = child.next_child();
    EXPECT_FALSE(child);

    Function::Func1D* func;
    func = scenery.functions.get("my_funny_function");
    EXPECT_EQ(100e-9, func->limits().lower());
    EXPECT_EQ(200e-9, func->limits().upper());
    EXPECT_NEAR(0.8, func->evaluate(160e-9), 1e-6);

    func = scenery.functions.get("constant_function");
    EXPECT_EQ(200e-9, func->limits().lower());
    EXPECT_EQ(400e-9, func->limits().upper());
    EXPECT_NEAR(1.337, func->evaluate(300e-9), 1e-6);

    func = scenery.functions.get("polynom_function");
    EXPECT_EQ(400e-9, func->limits().lower());
    EXPECT_EQ(1200e-9, func->limits().upper());
    double x = 600e-9;
    EXPECT_NEAR(1.0*x*x*x + 0.0*x*x + 1.2*x - 1.7, func->evaluate(x), 1e-6);

    func = scenery.functions.get("concat_function");
    EXPECT_EQ(100e-9, func->limits().lower());
    EXPECT_EQ(1200e-9, func->limits().upper());
}
//------------------------------------------------------------------------------
#include "Xml/Factory/PropagationConfigFab.h"
TEST_F(XmlTest, PropagationConfig) {

    const std::string path = "xml/tracer_settings.xml";

    PropagationConfig out;

    out.max_number_of_interactions_per_photon = 1337;
    out.use_multithread_when_possible = false;

    FileTools::write_text_to_file(Xml::Configs::to_node(out), path);

    Xml::Document doc(path);
    Xml::Node node = doc.node();
    PropagationConfig in = Xml::Configs::get_PropagationConfig_from_node(node.child("settings"));

    EXPECT_EQ(in.max_number_of_interactions_per_photon, out.max_number_of_interactions_per_photon);
    EXPECT_EQ(in.use_multithread_when_possible, out.use_multithread_when_possible);
}
