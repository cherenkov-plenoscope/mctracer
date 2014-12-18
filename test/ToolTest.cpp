#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "../Functions.h"

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
  EXPECT_TRUE ( is_ending("hans.xml",".xml") );
  EXPECT_FALSE( is_ending("hans.xml",".XML") );
  EXPECT_FALSE( is_ending("blabla_text_bla",".xml") );
  EXPECT_FALSE( is_ending("text.xmltext",".xml") );
  EXPECT_TRUE ( is_ending(".xml",".xml") );
  EXPECT_TRUE ( is_ending(".xml.xml",".xml") );
  EXPECT_TRUE ( is_ending("\t\n\r.xml",".xml") );
  EXPECT_TRUE ( is_ending("text .xml",".xml") );
  EXPECT_FALSE( is_ending("text.x ml",".xml") );
  EXPECT_TRUE ( is_ending("abc","c") );
  EXPECT_TRUE ( is_ending("abc\n","\n") );
}
//----------------------------------------------------------------------
TEST_F(Tools, pedantic_str_comp) {
  EXPECT_TRUE ( pedantic_str_comp("","")   );
  EXPECT_FALSE( pedantic_str_comp("a","b") );
  EXPECT_TRUE ( pedantic_str_comp("a","a") );
  EXPECT_FALSE( pedantic_str_comp("a","A") );
  EXPECT_TRUE ( pedantic_str_comp("Auto","Auto") );
  EXPECT_FALSE( pedantic_str_comp("Auto","auto") );
  EXPECT_TRUE ( pedantic_str_comp("Auto ","Auto ") );
  EXPECT_FALSE( pedantic_str_comp("Auto ","Auto") );
  EXPECT_TRUE ( pedantic_str_comp("\n","\n") );
  EXPECT_FALSE( pedantic_str_comp(" Auto","Auto") );
}
//----------------------------------------------------------------------
TEST_F(Tools, remove_if_leading) {
  string Hans = "Hans";
  remove_if_leading(Hans,'H'); 
  EXPECT_EQ("ans", Hans);

  string Klaus = "Klaus";
  remove_if_leading(Klaus,'H'); 
  EXPECT_EQ("Klaus", Klaus);

  string Peter = "Peter";
  remove_if_leading(Peter,'t'); 
  EXPECT_EQ("Peter", Peter); 

  string Willi = " Willi";
  remove_if_leading(Willi,'W'); 
  EXPECT_EQ(" Willi", Willi); 

  string Dieter = " Dieter";
  remove_if_leading(Dieter,' '); 
  EXPECT_EQ("Dieter", Dieter); 
}
//------------------------------------------------------------------------------
TEST_F(Tools, cut_leading_token) {
  string names = "Hans,Peter,Klaus";
  string first_name = cut_leading_token(names,','); 
  EXPECT_EQ("Hans", first_name);
  EXPECT_EQ("Peter,Klaus", names);

  names = ",Hans,Peter,Klaus";
  first_name = cut_leading_token(names,','); 
  EXPECT_EQ("", first_name);
  EXPECT_EQ("Hans,Peter,Klaus", names);

  names = ",Hans/Peter/Klaus/";
  first_name = cut_leading_token(names,'/'); 
  EXPECT_EQ(",Hans", first_name);
  EXPECT_EQ("Peter/Klaus/", names);

  names = "Hans ,Peter,Klaus";
  first_name = cut_leading_token(names,','); 
  EXPECT_EQ("Hans ", first_name);
  EXPECT_EQ("Peter,Klaus", names);

  names =  " Hans,Peter,Klaus";
  first_name = cut_leading_token(names,','); 
  EXPECT_EQ(" Hans", first_name);
  EXPECT_EQ("Peter,Klaus", names);
}
//------------------------------------------------------------------------------
TEST_F(Tools, word_size_is_too_large) {

  string h = "Hans Peter";

  bool error_detected = false;
  try{
    float H = ToolBox::str2float_4byte_bin_map(h);
    H=H;
  }catch(TracerException &e){
    //std::cout << e.what();
    error_detected = true;
  }

  EXPECT_TRUE(error_detected);
}
//------------------------------------------------------------------------------
TEST_F(Tools, word_size_is_zero) {

  string h = "";

  bool error_detected = false;
  try{
    float H = ToolBox::str2float_4byte_bin_map(h);
    H=H;
  }catch(TracerException &e){
    //std::cout << e.what();
    error_detected = true;
  }

  EXPECT_TRUE(error_detected);
}
//------------------------------------------------------------------------------
TEST_F(Tools, word_2_float) {

  string h = "hans";

  float H = ToolBox::str2float_4byte_bin_map(h);
  string r = ToolBox::float2str_4byte_bin_map(H);

  EXPECT_EQ(h, r);
}
//------------------------------------------------------------------------------
TEST_F(Tools, float_2_word) {

  float leet = 1.337;

  string leet_word = ToolBox::float2str_4byte_bin_map(leet);
  float r_leet = ToolBox::str2float_4byte_bin_map(leet_word);

  EXPECT_EQ(leet, r_leet);
}
//------------------------------------------------------------------------------
TEST_F(Tools, zero_float_2_word) {

  float n = 0.0;
  string emp = ToolBox::float2str_4byte_bin_map(n);
  EXPECT_EQ(string(4, '\0'), emp);
}
//------------------------------------------------------------------------------
TEST_F(Tools, zero_word_2_float) {

  string emp(4, '\0');
  float n = ToolBox::str2float_4byte_bin_map(emp);
  EXPECT_EQ(0.0, n);
}