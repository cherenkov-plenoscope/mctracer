#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "Tools/StringTools.h"
#include "ProgramOptions.h"

class ProgramOptionsTest : public ::testing::Test {
};
//------------------------------------------------------------------------------
TEST_F(ProgramOptionsTest, has_not_existing_key) {
    KeyValueMap options;
    EXPECT_FALSE(options.has_key("peter"));
}
//------------------------------------------------------------------------------
TEST_F(ProgramOptionsTest, has_existing_key) {
    KeyValueMap options;
    options.insert_key_and_value("peter", "41");
    EXPECT_TRUE(options.has_key("peter"));
}
//------------------------------------------------------------------------------
TEST_F(ProgramOptionsTest, map) {
    KeyValueMap options;

    options.insert_key_and_value("hans", "41");
    options.insert_key_and_value("peter", "42");

    EXPECT_TRUE(options.has_key("peter"));
    EXPECT_TRUE(options.has_key("hans"));
    EXPECT_THROW(options.get_value_for_key("gustav"),TracerException);
    EXPECT_TRUE(StringTools::is_equal(options.get_value_for_key("hans"), "41"));
}
//------------------------------------------------------------------------------
TEST_F(ProgramOptionsTest, assert_key_is_unique) {
    KeyValueMap options;

    options.insert_key_and_value("hans", "41");
    options.insert_key_and_value("peter", "42");

    EXPECT_THROW(    
        options.insert_key_and_value("hans", "42"),
        TracerException
    );
}
//------------------------------------------------------------------------------
/*TEST_F(ProgramOptionsTest, parse_program_options) {
    
    int argc = 7;
    char opt1[] = "../build/mctracer";
    char opt2[] = "-d";
    char opt3[] = "1337";
    char opt4[] = "-w";
    char opt5[] = "42";
    char opt6[] = "--file";
    char opt7[] = "my_funny_file.txt";
    char* argv [] = {opt1, opt2, opt3, opt4, opt5, opt6, opt7};
    
    KeyValueMap options(argc, argv);

    ASSERT_TRUE(options.has_key("call"));
    EXPECT_EQ(options.get_value_for_key("call"),"../build/mctracer");

    ASSERT_TRUE(options.has_key("-d"));
    EXPECT_EQ(options.get_value_for_key("-d"),"1337");

    ASSERT_TRUE(options.has_key("-w"));
    EXPECT_EQ(options.get_value_for_key("-w"),"42");

    ASSERT_TRUE(options.has_key("--file"));
    EXPECT_EQ(options.get_value_for_key("--file"),"my_funny_file.txt");

    EXPECT_FALSE(options.has_key("non_existing_key"));
}*/
//------------------------------------------------------------------------------
/*TEST_F(ProgramOptionsTest, missing_value_for_key) {
    
    int argc = 2;
    char opt1[] = "../build/mctracer";
    char opt2[] = "-d";
    char* argv [] = {opt1, opt2};
    
    EXPECT_THROW(    
        KeyValueMap options(argc, argv),
        TracerException
    );
}*/
//------------------------------------------------------------------------------
TEST_F(ProgramOptionsTest, value_to_double) {
      
    KeyValueMap options;
    options.insert_key_and_value("peter", "41.1337");
    EXPECT_EQ(41.1337, options.get_value_for_key_as_double("peter"));
}
//------------------------------------------------------------------------------
TEST_F(ProgramOptionsTest, value_to_integer) {
      
    KeyValueMap options;
    options.insert_key_and_value("peter", "1337");
    EXPECT_EQ(1337, options.get_value_for_key_as_int("peter"));
}
//------------------------------------------------------------------------------
TEST_F(ProgramOptionsTest, value_to_bool) {
      
    KeyValueMap options;
    options.insert_key_and_value("peter", "true");
    EXPECT_EQ(true, options.get_value_for_key_as_bool("peter"));
}
//------------------------------------------------------------------------------
TEST_F(ProgramOptionsTest, save_and_load) {
        
    const std::string filename = "./config_and_options_IO/robert.cfg";

    KeyValueMap options;
    options.insert_key_and_value("peter", "true");
    options.insert_key_and_value("hans", "1337");
    options.insert_key_and_value("gustav", "/path/to/file");
    options.save(filename);

    KeyValueMap loaded_options(filename);

    ASSERT_TRUE(loaded_options.has_key("peter"));
    EXPECT_EQ(  
        options.get_value_for_key("peter"), 
        loaded_options.get_value_for_key("peter")
    );

    ASSERT_TRUE(loaded_options.has_key("hans"));
    EXPECT_EQ(  
        options.get_value_for_key("hans"), 
        loaded_options.get_value_for_key("hans")
    );
    
    ASSERT_TRUE(loaded_options.has_key("gustav"));
    EXPECT_EQ(  
        options.get_value_for_key("gustav"), 
        loaded_options.get_value_for_key("gustav")
    );
}