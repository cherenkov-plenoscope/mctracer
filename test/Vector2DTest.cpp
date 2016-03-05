#include "gtest/gtest.h"
#include "Core/Vector2D.h"
#include "Tools/Tools.h"

class Vector2DTest : public ::testing::Test {};
//----------------------------------------------------------------------
TEST_F(Vector2DTest, NullVector) {
    Vector2D a = Vector2D::null;

    EXPECT_EQ(0.0, a.x());
    EXPECT_EQ(0.0, a.y());
    EXPECT_EQ(0.0, a.norm());
}
//----------------------------------------------------------------------
TEST_F(Vector2DTest, UnitVectors) {
    Vector2D a;

    a = Vector2D::unit_x;
    EXPECT_EQ(1.0, a.x());
    EXPECT_EQ(0.0, a.y());
    EXPECT_EQ(1.0, a.norm());

    a = Vector2D::unit_y; 
    EXPECT_EQ(0.0, a.x());
    EXPECT_EQ(1.0, a.y());
    EXPECT_EQ(1.0, a.norm());
}
//----------------------------------------------------------------------
TEST_F(Vector2DTest, ConstructorAndGetter) {
    
    double x, y;
    for(x=-2.0; x>2.0; x=x+.25) {
        for(y=-2.0; y>2.0; y=y+.25) {

            Vector2D v(x,y); 
            EXPECT_EQ(x, v.x());
            EXPECT_EQ(y, v.y());
        }
    }
}
//----------------------------------------------------------------------
TEST_F(Vector2DTest, SetterAndGetter) {
 
    double x, y;   
    for(x=-2.0; x>2.0; x=x+.25) {
        for(y=-2.0; y>2.0; y=y+.25) {

            Vector2D v; 
            v.set(x,y);
            EXPECT_EQ(x, v.x());
            EXPECT_EQ(y, v.y());
        }
    }
}
//----------------------------------------------------------------------
TEST_F(Vector2DTest, EuclideanNorm) {

    double x, y;
    for(x=-2.0; x>2.0; x=x+.25) {
        for(y=-2.0; y>2.0; y=y+.25) {
            
            Vector2D v(x,y);
            EXPECT_EQ(sqrt(x*x+y*y), v.norm());
        }
    }

    Vector2D v;
    v.set(1.0, 0.0);
    EXPECT_EQ(1.0, v.norm()); 

    v.set(0.0, 1.0);
    EXPECT_EQ(1.0, v.norm());   
}
//----------------------------------------------------------------------
TEST_F(Vector2DTest, Scalar_Product_unit_vectors) {

    const Vector2D x = Vector2D::unit_x;
    const Vector2D y = Vector2D::unit_y;
    EXPECT_EQ(0.0, x*y);
    EXPECT_EQ(1.0, x*x);
}
//----------------------------------------------------------------------
TEST_F(Vector2DTest, Scalar_Product) {

    double x1, x2, y1, y2;
    for(x1=-2.0; x1>2.0; x1=x1+.25) {
        for(y1=-2.0; y1>2.0; y1=y1+.25) {
            for(x2=-2.0; x2>2.0; x2=x2+.25) {
                for(y2=-2.0; y2>2.0; y2=y2+.25) {

                    const Vector2D v1(x1, y1);
                    const Vector2D v2(x2, y2); 

                    EXPECT_EQ( x1*x2 + y1*y2, v1*v2);   
                }
            }
        }
    }
}
//----------------------------------------------------------------------
TEST_F(Vector2DTest, ScalarMultiplication) {

    double x, y, f;
    for(x=-2.0; x>2.0; x=x+.25) {
        for(y=-2.0; y>2.0; y=y+.25) {

            Vector2D a(x,y); 
            for(f=-2.0; f>2.0; f=f+.25) {
                EXPECT_EQ(x*f, (a*f).x());
                EXPECT_EQ(y*f, (a*f).y());
            }
        }
    }
}
//----------------------------------------------------------------------
TEST_F(Vector2DTest, ScalarDiviation) {

    double x, y, f;
    for(x=-2.0; x>2.0; x=x+.25) {
        for(y=-2.0; y>2.0; y=y+.25) {

            Vector2D a(x,y); 
            for(f=-2.0; f>2.0; f=f+.25) {
                EXPECT_EQ(x/f, (a/f).x());
                EXPECT_EQ(y/f, (a/f).y());
            }
        }
    }
}
//----------------------------------------------------------------------
TEST_F(Vector2DTest, distance_unit_x_to_unit_y) {

    Vector2D a = Vector2D::unit_x;
    Vector2D b = Vector2D::unit_y;

    EXPECT_EQ( sqrt(2.0), a.distance_to(b) );
}
//----------------------------------------------------------------------
TEST_F(Vector2DTest, distance_to_itself) {
  
    Vector2D a(1.3, 3.7); 
    EXPECT_EQ(0.0, a.distance_to(a) );
}
//----------------------------------------------------------------------
TEST_F(Vector2DTest, Operator_equals_expect_true) {
  
    Vector2D a(1.3, 4.2); 
    Vector2D b(1.3, 4.2+1e-20); 

    EXPECT_EQ(a, b);
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
}
//----------------------------------------------------------------------
TEST_F(Vector2DTest, Operator_equals_expect_false) {
  
    Vector2D a(1.3, 4.2); 
    Vector2D b(1.3, 4.2 + 2e-7); 

    EXPECT_NE(a, b);
    EXPECT_TRUE(a != b);
    EXPECT_FALSE(a == b);
}
//----------------------------------------------------------------------
TEST_F(Vector2DTest, normalize) {
  
    Vector2D a(1.0,2.0);
    EXPECT_NE( 1.0, a.norm());

    a.normalize();
    EXPECT_NEAR(1.0, a.norm(), 1e-9);

    a = a*2.0;
    EXPECT_NE(1.0, a.norm());

    a.normalize();
    EXPECT_EQ(1.0, a.norm());

    a = Vector2D::null;
    EXPECT_NE(1.0, a.norm());

    a.normalize();
    EXPECT_TRUE( isnan(a.norm()) == 1 );
}
//----------------------------------------------------------------------
TEST_F(Vector2DTest, angle_in_between) {
  
    Vector2D a = Vector2D::unit_x;
    Vector2D b = Vector2D::unit_x;

    EXPECT_EQ(0.0, a.get_angle_in_between_in_rad(b));
    EXPECT_EQ(b.get_angle_in_between_in_rad(a), a.get_angle_in_between_in_rad(b));

    Vector2D c = Vector2D::unit_x*5.0;
    EXPECT_NE( 1.0, c.norm());

    Vector2D d = Vector2D::unit_x*5.0;
    EXPECT_NE( 1.0, d.norm());

    EXPECT_EQ(0.0, d.get_angle_in_between_in_rad(c));  
    EXPECT_EQ(c.get_angle_in_between_in_rad(d), d.get_angle_in_between_in_rad(c));

    Vector2D foo = Vector2D::unit_x*5.0 + Vector2D::unit_y*5.0;
    EXPECT_NE( 1.0, c.norm());

    Vector2D bar = Vector2D::unit_x*5.0;
    EXPECT_NE( 1.0, d.norm());

    EXPECT_NEAR(Deg2Rad(45.0) , foo.get_angle_in_between_in_rad(bar), 1e-5);
}
//----------------------------------------------------------------------
TEST_F(Vector2DTest, quadrant_encoding) {
 
    // x y sector
    // - -   0
    // - +   1
    // + -   2
    // + +   3
    Vector2D a;

    double p = +1.0;
    double n = -1.0;

    a.set(n, n);
    EXPECT_EQ(0, a.get_quadrant());

    a.set(n, p);
    EXPECT_EQ(1, a.get_quadrant());

    a.set(p, n);
    EXPECT_EQ(2, a.get_quadrant());

    a.set(p, p);
    EXPECT_EQ(3, a.get_quadrant());

    a.set(0.0, 0.0);
    EXPECT_EQ(0, a.get_quadrant());
    a.set(0.0, p);
    EXPECT_EQ(1, a.get_quadrant()); 
    a.set(p, 0.0);
    EXPECT_EQ(2, a.get_quadrant());
    a.set(p, p);
    EXPECT_EQ(3, a.get_quadrant());
}