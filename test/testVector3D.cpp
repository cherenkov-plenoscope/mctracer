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

	Vector3D RandVec1;
	Vector3D RandVec2;	
	
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
    RandVec1.set((rand()-.5),(rand()-.5),(rand()-.5));
    RandVec2.set((rand()-.5),(rand()-.5),(rand()-.5));
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }

  // Objects declared here can be used by all tests in the test case for Foo.
};
//----------------------------------------------------------------------
TEST_F(Vector3DTest, ConstructorAndGetter) {
  const double x = (rand()-.5);
  const double y = (rand()-.5);
  const double z = (rand()-.5);
  Vector3D v(x,y,z); 
  EXPECT_EQ(x, v.get_x());
  EXPECT_EQ(y, v.get_y());
  EXPECT_EQ(z, v.get_z());
}
//----------------------------------------------------------------------
TEST_F(Vector3DTest, SetterAndGetter) {
  const double x = (rand()-.5);
  const double y = (rand()-.5);
  const double z = (rand()-.5);
  Vector3D v;
  v.set(x,y,z);
  EXPECT_EQ(x, v.get_x());
  EXPECT_EQ(y, v.get_y());
  EXPECT_EQ(z, v.get_z());
}
//----------------------------------------------------------------------
TEST_F(Vector3DTest, EuclideanNorm) {
  const double x = (rand()-.5);
  const double y = (rand()-.5);
  const double z = (rand()-.5);
  Vector3D v(x,y,z);
  EXPECT_EQ(sqrt(x*x+y*y+z*z), v.norm2());
  
  v.set(1.0,0.0,0.0);
  EXPECT_EQ(1.0, v.norm2()); 
   
  v.set(0.0,1.0,0.0);
  EXPECT_EQ(1.0, v.norm2());  
  
  v.set(0.0,0.0,1.0);
  EXPECT_EQ(1.0, v.norm2());  
}
//----------------------------------------------------------------------
TEST_F(Vector3DTest, CrossProduct) {

  Vector3D u(1.0,0.0,0.0);
  Vector3D v(0.0,1.0,0.0);
  Vector3D w = u.cross_product(v);
  
  EXPECT_EQ(1.0, w.get_z());
  
  double x1, y1, z1, x2, y2, z2; 
  x1 = rand();
  y1 = rand();
  z1 = rand();
  x2 = rand();
  y2 = rand();
  z2 = rand();
  
  Vector3D v1(x1,y1,z1);
  Vector3D v2(x2,y2,z2); 
  
  Vector3D v3( y1*z2 - y2*z1,
               z1*x2 - z2*x1,
               x1*y2 - x2*y1);
  
  EXPECT_EQ(v3.get_x(), ( v1.cross_product(v2) ).get_x());  
  EXPECT_EQ(v3.get_y(), ( v1.cross_product(v2) ).get_y());  
  EXPECT_EQ(v3.get_z(), ( v1.cross_product(v2) ).get_z());    
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
  
  double x1, y1, z1, x2, y2, z2; 
  x1 = rand();
  y1 = rand();
  z1 = rand();
  x2 = rand();
  y2 = rand();
  z2 = rand();
  
  Vector3D v1(x1,y1,z1);
  Vector3D v2(x2,y2,z2); 
  
  EXPECT_EQ( x1*x2 + y1*y2 + z1*z2 , v1*v2);  
}
//----------------------------------------------------------------------
TEST_F(Vector3DTest, ScalarMultiplication) {

	double x = (rand()-.5);
	double y = (rand()-.5);
	double z = (rand()-.5);
	Vector3D a(x,y,z); 
	double factor = rand();
  
	EXPECT_EQ(factor*x, (a*factor ).get_x());
	EXPECT_EQ(factor*y, (a*factor ).get_y());
	EXPECT_EQ(factor*z, (a*factor ).get_z());
}
//----------------------------------------------------------------------
TEST_F(Vector3DTest, ScalarDiviation) {
	double x = (rand()-.5);
	double y = (rand()-.5);
	double z = (rand()-.5);
	Vector3D a(x,y,z); 
	double factor = (rand()-.5);
  
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
