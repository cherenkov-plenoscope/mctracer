#include "gtest/gtest.h"
#include "../XmlFactory/WorldFactory.h"

class WorldFactoryTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(WorldFactoryTest, DefaultWorld) {

    WorldFactory file2world;
    Frame *Mworld = file2world.world();

    // name of default world
    EXPECT_EQ( Mworld->get_name() , "world");

    // default world has no children
    EXPECT_EQ( Mworld->get_number_of_children() , 0 );

    // position of default world in mother frame is null vector
    EXPECT_EQ( 
        *Mworld->get_position_in_mother(),
        Vector3D::null
    );

    // rotation of default world in mother frame is null
    EXPECT_EQ( 
        *Mworld->get_rotation_in_mother(),
        Rotation3D::null
    );
}
//------------------------------------------------------------------------------
TEST_F(WorldFactoryTest, ReadEmptyXML) {

    WorldFactory file2world;
    Frame *Mworld = NULL;

    ASSERT_NO_THROW(
        file2world.load("./test_scenery/empty.xml");
        Mworld = file2world.world();
    );

    // name of default world
    EXPECT_EQ( Mworld->get_name() , "world");
    // no children in world
    EXPECT_EQ( Mworld->get_number_of_children() , 0 );
    // enclosing radius must be zero
    EXPECT_EQ( 0.0, Mworld->get_radius_of_sphere_enclosing_all_children() );
}
//------------------------------------------------------------------------------
TEST_F(WorldFactoryTest, ReadNotExistingFile) {

    WorldFactory file2world;

    string xml_file = "./not_existing_path/not_existing_file.xml";

    try{

        file2world.load(xml_file);
        Frame *Mworld = file2world.world();

        // the root of a file is always called world also the file does not 
        // exist
        EXPECT_EQ( Mworld->get_name() , "world");
    }catch(XmlIoException& error){

        //check whether the exception knows the bad file path or not
        EXPECT_EQ( error.filename() , xml_file );
    }catch(std::exception& error){

        // only XmlIoException exceptions are acceptable
        cout << error.what() << endl;
        EXPECT_EQ( true , false );
    }
}
//------------------------------------------------------------------------------
TEST_F(WorldFactoryTest, ReadFileWithUnknownObject) {

    WorldFactory file2world;

    string xml_file = 
    "./test_scenery/unkown_object_in_line_17_MarioMuellerWesternhagen.xml";

    try{

        file2world.load(xml_file);
        Frame *Mworld = file2world.world();

        EXPECT_EQ( Mworld->get_name() , "world");
    }catch(UnknownItem& unknown){

        //error.ReportException();
        //check whether the exception knows the bad file path or not
        EXPECT_EQ( unknown.name() , "MarioMuellerWesternhagen" );
    }catch(std::exception& error){

        //only UnknownObject exceptions are acceptable
        cout << error.what();
        EXPECT_EQ( true , false );
    }
}
//------------------------------------------------------------------------------
TEST_F(WorldFactoryTest, IncludeXMLFiles) {
    try{

        WorldFactory file2world;
        string xml_file = "./test_scenery/including_other_xml_files.xml";
        file2world.load(xml_file);
    }catch(std::exception& error){
        cout << error.what();
    }
}
//------------------------------------------------------------------------------
TEST_F(WorldFactoryTest, read_function_linear_interpolation) {
    WorldFactory fab;
    fab.load("./test_scenery/functions_as_variables.xml");

    const Function::Func1D* f_lin = fab.functions->get_function("my_funny_function");
    EXPECT_EQ(100e-9, f_lin->get_limits().get_lower());
    EXPECT_EQ(200e-9, f_lin->get_limits().get_upper());
    EXPECT_NEAR(0.1, (*f_lin)(105e-9), 1e-9);
    EXPECT_NEAR(0.9, (*f_lin)(125e-9), 1e-9);
    EXPECT_NEAR(0.1, (*f_lin)(190e-9), 1e-9);
}
//------------------------------------------------------------------------------
TEST_F(WorldFactoryTest, read_functions_constant) {
    WorldFactory fab;
    fab.load("./test_scenery/functions_as_variables.xml");

    const Function::Func1D* f_con = fab.functions->get_function("constant_function");
    EXPECT_EQ(200e-9, f_con->get_limits().get_lower());
    EXPECT_EQ(400e-9, f_con->get_limits().get_upper());
    EXPECT_EQ(1.337, (*f_con)(200e-9));
    EXPECT_EQ(1.337, (*f_con)(300e-9));
    EXPECT_EQ(1.337, (*f_con)(399e-9));
}
//------------------------------------------------------------------------------
TEST_F(WorldFactoryTest, read_function_polynom) {
    WorldFactory fab;
    fab.load("./test_scenery/functions_as_variables.xml");

    const Function::Func1D* f_pol = fab.functions->get_function("polynom_function");
    EXPECT_EQ( 400e-9, f_pol->get_limits().get_lower());
    EXPECT_EQ(1200e-9, f_pol->get_limits().get_upper());
    double x = 400e-9;
    EXPECT_NEAR(x*x*x*1.0+x*x*0.0+x*1.2-1.7, (*f_pol)(x), 1e-9);
    x = 400e-9;
    EXPECT_NEAR(x*x*x*1.0+x*x*0.0+x*1.2-1.7, (*f_pol)(x), 1e-9);
    x = 500e-9;
    EXPECT_NEAR(x*x*x*1.0+x*x*0.0+x*1.2-1.7, (*f_pol)(x), 1e-9);
    x = 600e-9;
    EXPECT_NEAR(x*x*x*1.0+x*x*0.0+x*1.2-1.7, (*f_pol)(x), 1e-9);
    x = 700e-9;
    EXPECT_NEAR(x*x*x*1.0+x*x*0.0+x*1.2-1.7, (*f_pol)(x), 1e-9);
}
//------------------------------------------------------------------------------
TEST_F(WorldFactoryTest, read_function_concatenation) {
    WorldFactory fab;
    fab.load("./test_scenery/functions_as_variables.xml");

    const Function::Func1D* f_concat = fab.functions->get_function("concat_function");
    EXPECT_EQ(100e-9, f_concat->get_limits().get_lower());
    EXPECT_EQ(1200e-9, f_concat->get_limits().get_upper());

    EXPECT_NEAR(0.1, (*f_concat)(105e-9), 1e-9);
    EXPECT_NEAR(0.9, (*f_concat)(125e-9), 1e-9);
    EXPECT_NEAR(0.1, (*f_concat)(190e-9), 1e-9);

    EXPECT_EQ(1.337, (*f_concat)(200e-9));
    EXPECT_EQ(1.337, (*f_concat)(300e-9));
    EXPECT_EQ(1.337, (*f_concat)(399e-9));

    double x = 400e-9;
    EXPECT_NEAR(x*x*x*1.0+x*x*0.0+x*1.2-1.7, (*f_concat)(x), 1e-9);
    x = 400e-9;
    EXPECT_NEAR(x*x*x*1.0+x*x*0.0+x*1.2-1.7, (*f_concat)(x), 1e-9);
    x = 500e-9;
    EXPECT_NEAR(x*x*x*1.0+x*x*0.0+x*1.2-1.7, (*f_concat)(x), 1e-9);
    x = 600e-9;
    EXPECT_NEAR(x*x*x*1.0+x*x*0.0+x*1.2-1.7, (*f_concat)(x), 1e-9);
    x = 700e-9;
    EXPECT_NEAR(x*x*x*1.0+x*x*0.0+x*1.2-1.7, (*f_concat)(x), 1e-9);
}
//------------------------------------------------------------------------------
TEST_F(WorldFactoryTest, include_stl_file) {
    WorldFactory fab;
    fab.load("./test_scenery/geometry_with_STL_include.xml");
}