#include <iostream> 
#include <string>
#include <math.h>

#include "SmartImage.h"

#include "gtest/gtest.h"

// The fixture for testing class Foo.
class SmartImageTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.
	
  SmartImageTest() {
    // You can do set-up work for each test here.
  }

  virtual ~SmartImageTest() {
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
TEST_F(SmartImageTest, ConstructorAndGetter) {
  
 	SmartImage MyImage;
  
  MyImage.Set(MCT_VGA);

 	// final image width should be 640
	EXPECT_EQ( 640 , MyImage.Width() );

 	// final image width should be 480 
	EXPECT_EQ( 480 , MyImage.Hight() );
}
//----------------------------------------------------------------------
