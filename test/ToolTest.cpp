#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "Tools/StringTools.h"
#include "Tools/FileTools.h"
#include "Tools/StringTools.h"

using namespace std;

// The fixture for testing class Foo.
class Tools : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

	
  Tools() {
    // You can do set-up work for each test here.
  }

  virtual ~Tools() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }

  // Objects declared here can be used by all tests in the test case for Foo.
};
//----------------------------------------------------------------------
TEST_F(Tools, Deg2Rad_Rad2Deg) {
  for(int i=-721; i<721; i++){

    double angle_in_deg = double(i);
    EXPECT_NEAR(angle_in_deg, Rad2Deg(Deg2Rad( angle_in_deg )), 1e-9);
  }
}
//----------------------------------------------------------------------
TEST_F(Tools, Deg2Rad) {
    EXPECT_NEAR(  0.0,      Deg2Rad(  0.0 ), 1e-9);
    EXPECT_NEAR( M_PI/2.0,  Deg2Rad( 90.0 ), 1e-9);
    EXPECT_NEAR( M_PI,      Deg2Rad(180.0 ), 1e-9);
    EXPECT_NEAR(-M_PI/2.0,  Deg2Rad(-90.0 ), 1e-9);
    EXPECT_NEAR(-M_PI,      Deg2Rad(-180.0), 1e-9);
    EXPECT_NEAR(2.0*M_PI,   Deg2Rad( 360.0), 1e-9);
}
//----------------------------------------------------------------------
TEST_F(Tools, Rad2Deg) {
    EXPECT_NEAR(  0.0,      Rad2Deg(  0.0*M_PI   ), 1e-9);
    EXPECT_NEAR(  45.0,     Rad2Deg( 0.25*M_PI   ), 1e-9);
    EXPECT_NEAR(  90.0,     Rad2Deg(  0.5*M_PI   ), 1e-9);
    EXPECT_NEAR( 180.0,     Rad2Deg(  1.0*M_PI   ), 1e-9);
    EXPECT_NEAR( 180.0e3,   Rad2Deg(  1.0e3*M_PI ), 1e-9);
}
//----------------------------------------------------------------------
TEST_F(Tools, is_ending) {
  EXPECT_TRUE ( StringTools::is_ending("hans.xml",".xml") );
  EXPECT_FALSE( StringTools::is_ending("hans.xml",".XML") );
  EXPECT_FALSE( StringTools::is_ending("blabla_text_bla",".xml") );
  EXPECT_FALSE( StringTools::is_ending("text.xmltext",".xml") );
  EXPECT_TRUE ( StringTools::is_ending(".xml",".xml") );
  EXPECT_TRUE ( StringTools::is_ending(".xml.xml",".xml") );
  EXPECT_TRUE ( StringTools::is_ending("\t\n\r.xml",".xml") );
  EXPECT_TRUE ( StringTools::is_ending("text .xml",".xml") );
  EXPECT_FALSE( StringTools::is_ending("text.x ml",".xml") );
  EXPECT_TRUE ( StringTools::is_ending("abc","c") );
  EXPECT_TRUE ( StringTools::is_ending("abc\n","\n") );
}
//----------------------------------------------------------------------
TEST_F(Tools, StringTools_is_equal) {
  EXPECT_TRUE ( StringTools::is_equal("","")   );
  EXPECT_FALSE( StringTools::is_equal("a","b") );
  EXPECT_TRUE ( StringTools::is_equal("a","a") );
  EXPECT_FALSE( StringTools::is_equal("a","A") );
  EXPECT_TRUE ( StringTools::is_equal("Auto","Auto") );
  EXPECT_FALSE( StringTools::is_equal("Auto","auto") );
  EXPECT_TRUE ( StringTools::is_equal("Auto ","Auto ") );
  EXPECT_FALSE( StringTools::is_equal("Auto ","Auto") );
  EXPECT_TRUE ( StringTools::is_equal("\n","\n") );
  EXPECT_FALSE( StringTools::is_equal(" Auto","Auto") );
}
//----------------------------------------------------------------------
TEST_F(Tools, StringTools_remove_if_leading) {
  string Hans = "Hans";
  StringTools::remove_char_from_text_if_leading('H', Hans); 
  EXPECT_EQ("ans", Hans);

  string Klaus = "Klaus";
  StringTools::remove_char_from_text_if_leading('H', Klaus); 
  EXPECT_EQ("Klaus", Klaus);

  string Peter = "Peter";
  StringTools::remove_char_from_text_if_leading('t', Peter); 
  EXPECT_EQ("Peter", Peter); 

  string Willi = " Willi";
  StringTools::remove_char_from_text_if_leading('W', Willi); 
  EXPECT_EQ(" Willi", Willi); 

  string Dieter = " Dieter";
  StringTools::remove_char_from_text_if_leading(' ', Dieter); 
  EXPECT_EQ("Dieter", Dieter); 
}
//------------------------------------------------------------------------------
TEST_F(Tools, StringTools_cut_leading_token) {
  string names = "Hans,Peter,Klaus";

  string first_name = 
    StringTools::cut_leading_token_infront_of_delimiter(names,',');
    
  EXPECT_EQ("Hans", first_name);
  EXPECT_EQ("Peter,Klaus", names);

  names = ",Hans,Peter,Klaus";
  first_name = StringTools::cut_leading_token_infront_of_delimiter(names,','); 
  EXPECT_EQ("", first_name);
  EXPECT_EQ("Hans,Peter,Klaus", names);

  names = ",Hans/Peter/Klaus/";
  first_name = StringTools::cut_leading_token_infront_of_delimiter(names,'/'); 
  EXPECT_EQ(",Hans", first_name);
  EXPECT_EQ("Peter/Klaus/", names);

  names = "Hans ,Peter,Klaus";
  first_name = StringTools::cut_leading_token_infront_of_delimiter(names,','); 
  EXPECT_EQ("Hans ", first_name);
  EXPECT_EQ("Peter,Klaus", names);

  names =  " Hans,Peter,Klaus";
  first_name = StringTools::cut_leading_token_infront_of_delimiter(names,','); 
  EXPECT_EQ(" Hans", first_name);
  EXPECT_EQ("Peter,Klaus", names);
}
//------------------------------------------------------------------------------
TEST_F(Tools, StringTools_contains_char) {
  string text = "Hans Peter is an awesome engineer who build AMS!";
  EXPECT_TRUE(StringTools::string_contains_char(text, 'H'));
  EXPECT_TRUE(StringTools::string_contains_char(text, 'A'));
  EXPECT_TRUE(StringTools::string_contains_char(text, 'w'));
  EXPECT_TRUE(StringTools::string_contains_char(text, '!'));
  EXPECT_FALSE(StringTools::string_contains_char(text, '?'));
  EXPECT_FALSE(StringTools::string_contains_char(text, '*'));
}
//------------------------------------------------------------------------------
TEST_F(Tools, place_item_infront_of_each_line) {
  std::stringstream out;
  out << "Dear Customer,\n";
  out << "\n";
  out << "Please Shut the fuck up!\n";
  out << "\n";
  out << "Kind regards, your customer service\n";

  std::string result =
    StringTools::place_first_infront_of_each_new_line_of_second("->",out.str());

  std::stringstream expected_out;
  expected_out << "->Dear Customer,\n";
  expected_out << "->\n";
  expected_out << "->Please Shut the fuck up!\n";
  expected_out << "->\n";
  expected_out << "->Kind regards, your customer service\n";

  EXPECT_TRUE(StringTools::is_equal(expected_out.str(), result));
}
//------------------------------------------------------------------------------
TEST_F(Tools, file_existance_on_not_existing_file) {
  EXPECT_FALSE(FileTools::can_be_opened("no_such_file"));
}
//------------------------------------------------------------------------------
TEST_F(Tools, file_existance_on_existing_file) {
  EXPECT_TRUE(FileTools::can_be_opened("ToolTest.cpp"));
}
//------------------------------------------------------------------------------
TEST_F(Tools, file_size_of_not_existing_file) {
  EXPECT_THROW(
    {FileTools::size_in_bytes("no_such_file");},
    TracerException
  );
}
//------------------------------------------------------------------------------
TEST_F(Tools, file_size_of_existing_file) {
  EXPECT_NO_THROW(
    {FileTools::size_in_bytes("ToolTest.cpp");}
  );
}
//------------------------------------------------------------------------------
TEST_F(Tools, assert_text_not_empty) {
  EXPECT_NO_THROW(
    AssertionTools::text_with_name_is_not_empty_given_context(
      "a non empty text\n", "my_text", "testing"
    )
  );

  EXPECT_THROW(
    AssertionTools::text_with_name_is_not_empty_given_context(
      "", "my_text", "testing"
    ),
    TracerException
  );
}
//------------------------------------------------------------------------------
TEST_F(Tools, assert_text_has_no_whitespaces) {
  EXPECT_NO_THROW(
    AssertionTools::text_with_name_has_no_whitespaces_given_context(
      "no_whitespaces_in_here", "my_text", "testing"
    )
  );

  EXPECT_THROW(
    AssertionTools::text_with_name_has_no_whitespaces_given_context(
      "this text has whitespaces", "my_text", "testing"
    ),
    TracerException
  );

  EXPECT_THROW(
    AssertionTools::text_with_name_has_no_whitespaces_given_context(
      "this\ntext\nhas\nwhitespaces", "my_text", "testing"
    ),
    TracerException
  );

  EXPECT_THROW(
    AssertionTools::text_with_name_has_no_whitespaces_given_context(
      "this\rtext\thas\nwhitespaces", "my_text", "testing"
    ),
    TracerException
  );
}
//------------------------------------------------------------------------------
TEST_F(Tools, assert_text_has_no_specific_char) {
  EXPECT_NO_THROW(
    AssertionTools::text_with_name_has_no_specific_char_given_context(
      "no plus sign in here", "my_text", '+', "testing"
    )
  );

  EXPECT_THROW(
    AssertionTools::text_with_name_has_no_specific_char_given_context(
      "a + sign in here", "my_text", '+', "testing"
    ),
    TracerException
  );
}