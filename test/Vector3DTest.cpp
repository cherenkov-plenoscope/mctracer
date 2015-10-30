#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "Core/Vector3D.h"
#include "Tools/Tools.h"

class Vector3DTest : public ::testing::Test {};
//----------------------------------------------------------------------
TEST_F(Vector3DTest, NullVector) {
    Vector3D a = Vector3D::null;

    EXPECT_EQ(0.0, a.x());
    EXPECT_EQ(0.0, a.y());
    EXPECT_EQ(0.0, a.z());
    EXPECT_EQ(0.0, a.norm());
}
//----------------------------------------------------------------------
TEST_F(Vector3DTest, UnitVectors) {
    Vector3D a;

    a = Vector3D::unit_x;
    EXPECT_EQ(1.0, a.x());
    EXPECT_EQ(0.0, a.y());
    EXPECT_EQ(0.0, a.z());
    EXPECT_EQ(1.0, a.norm());

    a = Vector3D::unit_y; 
    EXPECT_EQ(0.0, a.x());
    EXPECT_EQ(1.0, a.y());
    EXPECT_EQ(0.0, a.z());
    EXPECT_EQ(1.0, a.norm());

    a = Vector3D::unit_z; 
    EXPECT_EQ(0.0, a.x());
    EXPECT_EQ(0.0, a.y());
    EXPECT_EQ(1.0, a.z());
    EXPECT_EQ(1.0, a.norm());
}
//----------------------------------------------------------------------
TEST_F(Vector3DTest, ConstructorAndGetter) {
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
    EXPECT_EQ(sqrt(x*x+y*y+z*z), v.norm());

    v.set(1.0,0.0,0.0);
    EXPECT_EQ(1.0, v.norm()); 

    v.set(0.0,1.0,0.0);
    EXPECT_EQ(1.0, v.norm());  

    v.set(0.0,0.0,1.0);
    EXPECT_EQ(1.0, v.norm());  
}
//----------------------------------------------------------------------
TEST_F(Vector3DTest, crossUnitVectors) {

    Vector3D u = Vector3D::unit_x;
    Vector3D v = Vector3D::unit_y;
    Vector3D w = u.cross(v);
    EXPECT_EQ(1.0, w.z());
    EXPECT_EQ(Vector3D::unit_z, w);
}
//----------------------------------------------------------------------
TEST_F(Vector3DTest, cross) {

    double x1, y1, z1, x2, y2, z2;
    for(x1=-2.0; x1>2.0; x1=x1+.25) {
        for(y1=-2.0; y1>2.0; y1=y1+.25) {
            for(z1=-2.0; z1>2.0; z1=z1+.25) {
                for(x2=-2.0; x2>2.0; x2=x2+.25) {
                    for(y2=-2.0; y2>2.0; y2=y2+.25) {
                        for(z2=-2.0; z2>2.0; z2=z2+.25) {

                          Vector3D v1(x1,y1,z1);
                          Vector3D v2(x2,y2,z2); 
                          
                          Vector3D v3( y1*z2 - y2*z1,
                                       z1*x2 - z2*x1,
                                       x1*y2 - x2*y1);
                          
                          EXPECT_EQ(v3, v1.cross(v2));  
                        }
                    }
                }
            }
        }
    } 
}
//----------------------------------------------------------------------
TEST_F(Vector3DTest, Scalar_Product_unit_vectors) {

    const Vector3D x = Vector3D::unit_x;
    const Vector3D y = Vector3D::unit_y;
    EXPECT_EQ(0.0, x*y);

    const Vector3D z = Vector3D::unit_z; 
    EXPECT_EQ(0.0, y*z);

    EXPECT_EQ(1.0, x*x);
}
//----------------------------------------------------------------------
TEST_F(Vector3DTest, Scalar_Product) {

    double x1, y1, z1, x2, y2, z2;
    for(x1=-2.0; x1>2.0; x1=x1+.25) {
        for(y1=-2.0; y1>2.0; y1=y1+.25) {
            for(z1=-2.0; z1>2.0; z1=z1+.25) {
                for(x2=-2.0; x2>2.0; x2=x2+.25) {
                    for(y2=-2.0; y2>2.0; y2=y2+.25) {
                        for(z2=-2.0; z2>2.0; z2=z2+.25) {

                        const Vector3D v1(x1,y1,z1);
                        const Vector3D v2(x2,y2,z2); 

                        EXPECT_EQ( x1*x2 + y1*y2 + z1*z2 , v1*v2);  
                        }
                    }
                }
            }
        }
    }
}
//----------------------------------------------------------------------
TEST_F(Vector3DTest, ScalarMultiplication) {

    double x, y, z, f;
    for(x=-2.0; x>2.0; x=x+.25) {
        for(y=-2.0; y>2.0; y=y+.25) {
            for(z=-2.0; z>2.0; z=z+.25) {

                Vector3D a(x,y,z); 
                for(f=-2.0; f>2.0; f=f+.25) {
                    EXPECT_EQ(x*f, (a*f).x());
                    EXPECT_EQ(y*f, (a*f).y());
                    EXPECT_EQ(z*f, (a*f).z());
                }
            }
        }
    }
}
//----------------------------------------------------------------------
TEST_F(Vector3DTest, ScalarDiviation) {

    double x, y, z, f;
    for(x=-2.0; x>2.0; x=x+.25) {
        for(y=-2.0; y>2.0; y=y+.25) {
            for(z=-2.0; z>2.0; z=z+.25) {

                Vector3D a(x,y,z); 
                for(f=-2.0; f>2.0; f=f+.25) {
                    EXPECT_EQ(x/f, (a/f).x());
                    EXPECT_EQ(y/f, (a/f).y());
                    EXPECT_EQ(z/f, (a/f).z());
                }
            }
        }
    }
}
//----------------------------------------------------------------------
TEST_F(Vector3DTest, distance_unit_x_to_unit_y) {

    Vector3D a = Vector3D::unit_x;
    Vector3D b = Vector3D::unit_y;

    EXPECT_EQ( sqrt(2.0), a.distance_to(b) );
}
//----------------------------------------------------------------------
TEST_F(Vector3DTest, distance_to_itself) {
  
    Vector3D a(1.3, 3.7, 4.2); 
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
    Vector3D b(1.3,3.7,4.2 + 2e-7); 

    EXPECT_NE(a, b);
    EXPECT_TRUE(a != b);
    EXPECT_FALSE(a == b);
}
//----------------------------------------------------------------------
TEST_F(Vector3DTest, parallel_to_x_y_plane) {
  
    Vector3D a(1.0, 2.0, 0.0);
    EXPECT_TRUE( a.is_parallel_to_x_y_plane() );

    Vector3D b(1.3, 3.7, 4.2 + 1e-9); 
    EXPECT_FALSE( b.is_parallel_to_x_y_plane() );

    Vector3D c(0.0, 2.0, 5.5); 
    EXPECT_FALSE( c.is_parallel_to_x_y_plane() );

}
//----------------------------------------------------------------------
TEST_F(Vector3DTest, normalize) {
  
    Vector3D a(1.0,2.0,3.0);
    EXPECT_NE( 1.0, a.norm());

    a.normalize();
    EXPECT_EQ( 1.0, a.norm());

    a = a*2.0;
    EXPECT_NE( 1.0, a.norm());

    a.normalize();
    EXPECT_EQ( 1.0, a.norm());

    a = Vector3D::null;
    EXPECT_NE( 1.0, a.norm());

    a.normalize();
    EXPECT_TRUE( isnan(a.norm()) == 1 );
}
//----------------------------------------------------------------------
TEST_F(Vector3DTest, angle_in_between) {
  
    Vector3D a = Vector3D::unit_x;
    Vector3D b = Vector3D::unit_x;

    EXPECT_EQ(0.0, a.get_angle_in_between_in_rad(b));
    EXPECT_EQ(b.get_angle_in_between_in_rad(a), a.get_angle_in_between_in_rad(b));

    Vector3D c = Vector3D::unit_x*5.0;
    EXPECT_NE( 1.0, c.norm());

    Vector3D d = Vector3D::unit_x*5.0;
    EXPECT_NE( 1.0, d.norm());

    EXPECT_EQ(0.0, d.get_angle_in_between_in_rad(c));  
    EXPECT_EQ(c.get_angle_in_between_in_rad(d), d.get_angle_in_between_in_rad(c));

    Vector3D foo = Vector3D::unit_x*5.0 + Vector3D::unit_y*5.0;
    EXPECT_NE( 1.0, c.norm());

    Vector3D bar = Vector3D::unit_x*5.0;
    EXPECT_NE( 1.0, d.norm());

    EXPECT_NEAR(Deg2Rad(45.0) , foo.get_angle_in_between_in_rad(bar), 1e-5);
}
//----------------------------------------------------------------------
TEST_F(Vector3DTest, octant_encoding) {
 
    // x y z sector
    // - - -   0
    // - - +   1
    // - + -   2
    // - + +   3
    // + - -   4
    // + - +   5
    // + + -   6
    // + + +   7
    Vector3D a;

    double p = +1.0;
    double n = -1.0;

    a.set( n, n, n);
    EXPECT_EQ(0, a.get_octant());

    a.set( n, n, p);
    EXPECT_EQ(1, a.get_octant());

    a.set( n, p, n);
    EXPECT_EQ(2, a.get_octant());

    a.set( n, p, p);
    EXPECT_EQ(3, a.get_octant());

    a.set( p, n, n);
    EXPECT_EQ(4, a.get_octant());

    a.set( p, n, p);
    EXPECT_EQ(5, a.get_octant());

    a.set( p, p, n);
    EXPECT_EQ(6, a.get_octant());

    a.set( p, p, p);
    EXPECT_EQ(7, a.get_octant());

    a.set( 0.0, 0.0, 0.0);
    EXPECT_EQ(7, a.get_octant());
    a.set( 0.0, 0.0, p);
    EXPECT_EQ(7, a.get_octant());
    a.set( 0.0, p, 0.0);
    EXPECT_EQ(7, a.get_octant());
    a.set( p, 0.0, 0.0);
    EXPECT_EQ(7, a.get_octant());
    a.set( 0.0, p, p);
    EXPECT_EQ(7, a.get_octant()); 
    a.set( p, p, 0.0);
    EXPECT_EQ(7, a.get_octant());
    a.set( p, 0.0, p);
    EXPECT_EQ(7, a.get_octant());
}
//------------------------------------------------------------------------------
TEST_F(Vector3DTest, projection_on_basic_xyz_planes) {

    Vector3D v(42.0, 13.37, 3.141);

    EXPECT_EQ(Vector3D(42.0, 0.0, 3.141), v.get_projection_on_XZ_plane());
    EXPECT_EQ(Vector3D(0.0, 13.37, 3.141), v.get_projection_on_YZ_plane());
}