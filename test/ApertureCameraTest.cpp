#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "ApertureCamera.h"

using namespace std;

// The fixture for testing class Foo.
class ApertureCameraTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.
	
  ApertureCameraTest() {
    // You can do set-up work for each test here.
  }

  virtual ~ApertureCameraTest() {
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
TEST_F(ApertureCameraTest, set_up) {
  ApertureCamera Mamiya645("Mamiya645", 640, 480);

  double Mamiya_F_stop_number = 2.3;
  double Mamiya_sensor_width_in_m = 6e-2;
  uint   Mamiya_number_of_rays_emitted_per_pixel = 5;
  
  Mamiya645.set_aperture_cam(
    Mamiya_F_stop_number,
    Mamiya_sensor_width_in_m,
    Mamiya_number_of_rays_emitted_per_pixel
  );

  Mamiya645.set_FoV_in_rad(Deg2Rad(45.0));

  Vector3D pos(0.0,0.0,-5.0);
  Rotation3D rot(0.0,0.0,0.0);
  Mamiya645.update_position_and_orientation(pos, rot); 
}
//----------------------------------------------------------------------
/*TEST_F(Vector3DTest, ConstructorAndGetter) {
  const double x = (rand()-.5);
  const double y = (rand()-.5);
  const double z = (rand()-.5);
  Vector3D v(x,y,z); 
  EXPECT_EQ(x, v.x());
  EXPECT_EQ(y, v.y());
  EXPECT_EQ(z, v.z());
}
//----------------------------------------------------------------------
TEST_F(Vector3DTest, SetterAndGetter) {
  const double x = (rand()-.5);
  const double y = (rand()-.5);
  const double z = (rand()-.5);
  Vector3D v;
  v.set(x,y,z);
  EXPECT_EQ(x, v.x());
  EXPECT_EQ(y, v.y());
  EXPECT_EQ(z, v.z());
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
  Vector3D w = u.CrossProduct(v);
  
  EXPECT_EQ(1.0, w.z());
  
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
  
  EXPECT_EQ(v3.x(), ( v1.CrossProduct(v2) ).x());  
  EXPECT_EQ(v3.y(), ( v1.CrossProduct(v2) ).y());  
  EXPECT_EQ(v3.z(), ( v1.CrossProduct(v2) ).z());    
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
  
	EXPECT_EQ(factor*x, (a*factor ).x());
	EXPECT_EQ(factor*y, (a*factor ).y());
	EXPECT_EQ(factor*z, (a*factor ).z());
}
//----------------------------------------------------------------------
TEST_F(Vector3DTest, ScalarDiviation) {
	double x = (rand()-.5);
	double y = (rand()-.5);
	double z = (rand()-.5);
	Vector3D a(x,y,z); 
	double factor = (rand()-.5);
  
	EXPECT_EQ(x/factor, (a/factor ).x());
	EXPECT_EQ(y/factor, (a/factor ).y());
	EXPECT_EQ(z/factor, (a/factor ).z());
}
//----------------------------------------------------------------------
TEST_F(Vector3DTest, NullVector) {
	Vector3D a;
	a.set_null_vector(); 

	EXPECT_EQ(0.0, a.x());
	EXPECT_EQ(0.0, a.y());
	EXPECT_EQ(0.0, a.z());
	EXPECT_EQ(0.0, a.norm2());
}
//----------------------------------------------------------------------
TEST_F(Vector3DTest, UnitVectors) {
	Vector3D a;
	
	a.set_unit_vector_x(); 
	EXPECT_EQ(1.0, a.x());
	EXPECT_EQ(0.0, a.y());
	EXPECT_EQ(0.0, a.z());
	EXPECT_EQ(1.0, a.norm2());
	
	a.set_unit_vector_y(); 
	EXPECT_EQ(0.0, a.x());
	EXPECT_EQ(1.0, a.y());
	EXPECT_EQ(0.0, a.z());
	EXPECT_EQ(1.0, a.norm2());
	
	a.set_unit_vector_z(); 
	EXPECT_EQ(0.0, a.x());
	EXPECT_EQ(0.0, a.y());
	EXPECT_EQ(1.0, a.z());
	EXPECT_EQ(1.0, a.norm2());
}
//----------------------------------------------------------------------
TEST_F(Vector3DTest, distance_to) {
  Vector3D a;
  a.set_unit_vector_x(); 
  
  Vector3D b;
  b.set_unit_vector_y(); 

  EXPECT_EQ( sqrt(2.0), a.distance_to(b) );
}
//----------------------------------------------------------------------
TEST_F(Vector3DTest, distance_to_itself) {
  
  Vector3D a(1.3,3.7,4.2); 
  EXPECT_EQ( 0.0, a.distance_to(a) );
}
//----------------------------------------------------------------------
TEST_F(Vector3DTest, Operator_equals_expect_true) {
  
  Vector3D a(1.3,3.7,4.2); 
  Vector3D b(1.3,3.7,4.2 + 1e-20); 

  EXPECT_EQ(a, b);
  EXPECT_TRUE(a == b);
  EXPECT_FALSE(a != b);
}
//----------------------------------------------------------------------
TEST_F(Vector3DTest, Operator_equals_expect_false) {
  
  Vector3D a(1.3,3.7,4.2); 
  Vector3D b(1.3,3.7,4.2 + 1e-9); 

  EXPECT_NE(a, b);
  EXPECT_TRUE(a != b);
  EXPECT_FALSE(a == b);
}
//----------------------------------------------------------------------
TEST_F(Vector3DTest, parallel_to_x_y_plane) {
  
  Vector3D a(1.0,2.0,0.0);
  EXPECT_TRUE( a.is_parallel_to_x_y_plane() );

  Vector3D b(1.3,3.7,4.2 + 1e-9); 
  EXPECT_FALSE( b.is_parallel_to_x_y_plane() );

  Vector3D c(0.0,2.0,5.5); 
  EXPECT_FALSE( c.is_parallel_to_x_y_plane() );

}
//----------------------------------------------------------------------
TEST_F(Vector3DTest, normalize) {
  
  Vector3D a(1.0,2.0,3.0);
  EXPECT_NE( 1.0, a.norm2());

  a.normalize();
  EXPECT_EQ( 1.0, a.norm2());

  a = a*2.0;
  EXPECT_NE( 1.0, a.norm2());

  a.normalize();
  EXPECT_EQ( 1.0, a.norm2());

  a.set(0.0,0.0,0.0);
  EXPECT_NE( 1.0, a.norm2());

  a.normalize();
  EXPECT_TRUE( std::isnan(a.norm2()) == 1 );
}*/