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

    std::cout << options;
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