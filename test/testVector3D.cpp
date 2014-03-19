#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "../Vector3D.h"

using namespace std;

// The fixture for testing class Foo.
class Vector3DTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  Vector3DTest() {
    // You can do set-up work for each test here.
  }

  virtual ~Vector3DTest() {
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
TEST_F(Vector3DTest, ConstructorAndGetter) {
  const double x = 1.0;
  const double y = 2.0;
  const double z = 3.0;
  Vector3D v(x,y,z); 
  EXPECT_EQ(x, v.get_x());
  EXPECT_EQ(y, v.get_y());
  EXPECT_EQ(z, v.get_z());
}
//----------------------------------------------------------------------
TEST_F(Vector3DTest, SetterAndGetter) {
  const double x = 1.0;
  const double y = 2.0;
  const double z = 3.0;
  Vector3D v;
  v.set(x,y,z);
  EXPECT_EQ(x, v.get_x());
  EXPECT_EQ(y, v.get_y());
  EXPECT_EQ(z, v.get_z());
}

//----------------------------------------------------------------------
TEST_F(Vector3DTest, EuclideanNorm) {
  const double x = 1.0;
  const double y = 2.0;
  const double z = 3.0;
  Vector3D v(x,y,z);
  EXPECT_EQ(sqrt(x*x+y*y+z*z), v.norm2());
  
  v.set(1.0,0.0,0.0);
  EXPECT_EQ(1.0, v.norm2());  
}
//----------------------------------------------------------------------
TEST_F(Vector3DTest, CrossProduct) {

  Vector3D u(1.0,0.0,0.0);
  Vector3D v(0.0,1.0,0.0);
  Vector3D w = u.cross_product(v);
  
  EXPECT_EQ(1.0, w.get_z());
}
//----------------------------------------------------------------------
TEST_F(Vector3DTest, ScalarProduct) {

  Vector3D a(1.0,0.0,0.0);
  Vector3D b(0.0,1.0,0.0);
  EXPECT_EQ(0.0, a*b);
  
  Vector3D c(0.0,1.0,0.0);
  Vector3D d(0.0,0.0,1.0);  
  EXPECT_EQ(0.0, c*d);
  
  Vector3D e(1.0,0.0,0.0);
  Vector3D f(1.0,0.0,0.0);  
  EXPECT_EQ(1.0, e*f);
}
//----------------------------------------------------------------------
TEST_F(Vector3DTest, ScalarMultiplication) {

	double x = 1.123;
	double y = 42.1415;
	double z = 1337;
	Vector3D a(x,y,z); 
	double factor = -134;
  
	EXPECT_EQ(factor*x, (a*factor ).get_x());
	EXPECT_EQ(factor*y, (a*factor ).get_y());
	EXPECT_EQ(factor*z, (a*factor ).get_z());
}
//----------------------------------------------------------------------
TEST_F(Vector3DTest, ScalarDiviation) {
	double x = 1.123;
	double y = 42.1415;
	double z = 1337;
	Vector3D a(x,y,z); 
	double factor = -134;
  
	EXPECT_EQ(x/factor, (a/factor ).get_x());
	EXPECT_EQ(y/factor, (a/factor ).get_y());
	EXPECT_EQ(z/factor, (a/factor ).get_z());
}
//----------------------------------------------------------------------
TEST_F(Vector3DTest, NullVector) {
	Vector3D a;
	a.set_null_vector(); 

	EXPECT_EQ(0.0, a.get_x());
	EXPECT_EQ(0.0, a.get_y());
	EXPECT_EQ(0.0, a.get_z());
	EXPECT_EQ(0.0, a.norm2());
}
//----------------------------------------------------------------------
TEST_F(Vector3DTest, UnitVectors) {
	Vector3D a;
	
	a.set_unit_vector_x(); 
	EXPECT_EQ(1.0, a.get_x());
	EXPECT_EQ(0.0, a.get_y());
	EXPECT_EQ(0.0, a.get_z());
	EXPECT_EQ(1.0, a.norm2());
	
	a.set_unit_vector_y(); 
	EXPECT_EQ(0.0, a.get_x());
	EXPECT_EQ(1.0, a.get_y());
	EXPECT_EQ(0.0, a.get_z());
	EXPECT_EQ(1.0, a.norm2());
	
	a.set_unit_vector_z(); 
	EXPECT_EQ(0.0, a.get_x());
	EXPECT_EQ(0.0, a.get_y());
	EXPECT_EQ(1.0, a.get_z());
	EXPECT_EQ(1.0, a.norm2());
}
