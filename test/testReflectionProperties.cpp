#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "Core/ReflectionProperties.h"

using namespace std;

// The fixture for testing class Foo.
class ReflectionPropertiesTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.
	
  ReflectionPropertiesTest() {
    // You can do set-up work for each test here.
  }

  virtual ~ReflectionPropertiesTest() {
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
//------------------------------------------------------------------------------
TEST_F(ReflectionPropertiesTest, ConstructorAndGetter) {

  const double coef = 0.1337;
  ReflectionProperties refl(coef);
  EXPECT_EQ(coef, refl.ReflectionCoefficient() );

/*
  // check input values below 0
  bool problem_detected = FALSE;
  try{
    refl.SetReflectionCoefficient(-42.1337);
  }catch(BadValue &error){
    problem_detected = TRUE;
  }
  EXPECT_EQ( TRUE , problem_detected );

  // check input values above 1
  problem_detected = FALSE;
  try{
    refl.SetReflectionCoefficient(1.337);
  }catch(BadValue &error){
    problem_detected = TRUE;
  }
  EXPECT_EQ( TRUE , problem_detected );

  // check input values min and max
  // the minimum of 0 is still valid as it the maximum of 1
  problem_detected = FALSE;
  try{
    refl.SetReflectionCoefficient(1.0);
    refl.SetReflectionCoefficient(0.0);
  }catch(BadValue &error){
    problem_detected = TRUE;
  }
  EXPECT_EQ( FALSE , problem_detected );*/
}
//------------------------------------------------------------------------------