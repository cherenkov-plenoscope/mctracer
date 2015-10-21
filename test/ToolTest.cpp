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
TEST_F(Tools, StringTools_place_infront_of_each_line) {
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
TEST_F(Tools, strip_leading_and_tailing_whitespaces) {
  std::string no_whitespaces = "no_whitespaces";

  EXPECT_TRUE(
    StringTools::is_equal(
      no_whitespaces,
      StringTools::strip_whitespaces(no_whitespaces)
    )
  );

  std::string whitespaces_in_between = "whitespaces in between";

  EXPECT_TRUE(
    StringTools::is_equal(
      whitespaces_in_between,
      StringTools::strip_whitespaces(whitespaces_in_between)
    )
  );

  std::string leading_whitespaces = "    leading_whitespaces";

  EXPECT_TRUE(
    StringTools::is_equal(
      "leading_whitespaces",
      StringTools::strip_whitespaces(leading_whitespaces)
    )
  );

  std::string trailing_ws = "trailing_ws  ";

  EXPECT_TRUE(
    StringTools::is_equal(
      "trailing_ws",
      StringTools::strip_whitespaces(trailing_ws)
    )
  );
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
//------------------------------------------------------------------------------
TEST_F(Tools, odd_and_even) {
  EXPECT_TRUE(is_even(0));
  EXPECT_TRUE(is_even(2));
  EXPECT_TRUE(is_even(-2));
  EXPECT_TRUE(is_even(1214214));

  EXPECT_TRUE(is_odd(-1));
  EXPECT_TRUE(is_odd(-3));
  EXPECT_TRUE(is_odd(-7));
  EXPECT_TRUE(is_odd(-9));

  EXPECT_FALSE(is_even(3));
  EXPECT_FALSE(is_even(-1));
  EXPECT_FALSE(is_even(1337));
  EXPECT_FALSE(is_even(23));

  EXPECT_FALSE(is_odd(2));
  EXPECT_FALSE(is_odd(0));
  EXPECT_FALSE(is_odd(42));
  EXPECT_FALSE(is_odd(-46));
}
//------------------------------------------------------------------------------
TEST_F(Tools, StrToDouble) {

  EXPECT_NO_THROW(
    EXPECT_EQ(42.1337,StrToDouble("42.1337"));
    EXPECT_EQ(1337,StrToDouble("1337"));
    EXPECT_EQ(1,StrToDouble("1"));
    EXPECT_EQ(.1,StrToDouble(".1"));
    EXPECT_EQ(.1442,StrToDouble(".1442"));  
  );

  EXPECT_THROW(
    EXPECT_NE(42.1337,StrToDouble("42p1337"));
    EXPECT_NE(1337,StrToDouble("1337 "));
    EXPECT_NE(1,StrToDouble(""));
    EXPECT_NE(1.337,StrToDouble(" 1.337"));
    EXPECT_NE(.1,StrToDouble(" .1"));
    EXPECT_NE(.1,StrToDouble(" .1"));
    EXPECT_NE(.1442,StrToDouble(".1 442"));,
    TracerException
  );
}
//------------------------------------------------------------------------------
TEST_F(Tools, StrToBool) {

  EXPECT_NO_THROW(
    EXPECT_EQ(true,StrToBool("true"));
    EXPECT_EQ(true,StrToBool("TRUE"));
    EXPECT_EQ(false,StrToBool("false"));
    EXPECT_EQ(false,StrToBool("FALSE")); 
  );

  EXPECT_THROW(
    EXPECT_NE(true,StrToBool(" true"));
    EXPECT_NE(true,StrToBool("true "));
    EXPECT_NE(true,StrToBool("wahr"));
    EXPECT_NE(true,StrToBool("Troe"));
    EXPECT_NE(true,StrToBool("yes"));
    EXPECT_NE(true,StrToBool("no"));
    EXPECT_NE(true,StrToBool("0"));
    EXPECT_NE(true,StrToBool(""));,
    TracerException
  );
}
//------------------------------------------------------------------------------
TEST_F(Tools, StrToInt) {

  EXPECT_NO_THROW(
    EXPECT_EQ(1,StrToInt("1"));
    EXPECT_EQ(12,StrToInt("12"));
    EXPECT_EQ(1337,StrToInt("1337"));
    EXPECT_EQ(123456789,StrToInt("123456789")); 
  );

  EXPECT_THROW(
    //EXPECT_NE(1,StrToInt(" 1"));
    //EXPECT_NE(1,StrToInt("  1"));
    EXPECT_NE(1,StrToInt("1 "));
    EXPECT_NE(1,StrToInt(""));
    EXPECT_NE(123456,StrToInt("123 456"));
    EXPECT_NE(12.344,StrToInt("12.344"));
    EXPECT_NE(1,StrToInt("no"));
    EXPECT_NE(6.6,StrToInt("6p6"));,
    TracerException
  );
}
//------------------------------------------------------------------------------
#include "Core/Vector3D.h"
#include "Core/Photon.h"
#include <memory>

TEST_F(Tools, memory_test) {
  
  for(uint y=0; y<1e2; y++) {

    std::vector<Photon*> *photon_bunch = new std::vector<Photon*>;

    for(uint i=0; i<1e2; i++) {

      Photon* hans = new Photon(Vector3D::unit_x, Vector3D::unit_z ,133.7);
      photon_bunch->push_back(hans);
      //hans->delete_history();
      //delete hans;
    }

    //delete photon_bunch;
  }
}