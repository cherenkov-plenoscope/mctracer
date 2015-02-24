#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "../Function1D.h"

using namespace std;

// The fixture for testing class Foo.
class Function1DTest : public ::testing::Test {
    protected:
    // You can remove any or all of the following functions if its body
    // is empty.
	
    Function1DTest() {
        // You can do set-up work for each test here.
    }

    virtual ~Function1DTest() {
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
TEST_F(Function1DTest, ConstructorAndGetter) {
  
    EXPECT_THROW({
        Function1D f("ThisFileDoesNotExist.xml");
    }, XmlIoException);

    EXPECT_NO_THROW({
        Function1D f("./test_scenery/Function1D/function1D_complete.xml");
        EXPECT_EQ( "my_assignment" , f.name() ); 
    }); 
}
//------------------------------------------------------------------------------
TEST_F(Function1DTest, MissingIdentifiers) {

    EXPECT_THROW({
        Function1D f("./test_scenery/Function1D/function1D_missing_name.xml");
    }, XmlIoException);
}
//------------------------------------------------------------------------------
TEST_F(Function1DTest, InvalidFloatingNumber){

    EXPECT_THROW({
        Function1D f(
            "./test_scenery/Function1D/function1D_invalid_floating_number.xml"
        );
    }, XmlIoException);
}
//------------------------------------------------------------------------------
TEST_F(Function1DTest, SameArgumentTwice){
    
    EXPECT_THROW({
        Function1D f(
            "./test_scenery/Function1D/function1D_same_argument_twice.xml"
        );
    }, XmlIoException);
}
//------------------------------------------------------------------------------
TEST_F(Function1DTest, At){

    EXPECT_NO_THROW({
        Function1D f("./test_scenery/Function1D/function1D_complete.xml");
        
        double arg = 0.0;
        double integral = 0.0;

        for(uint i=0; i<150; i++) {
          arg = arg + 6.666;
          integral = integral + f.at(arg);
        }
    }); 
}
//------------------------------------------------------------------------------
TEST_F(Function1DTest, Integral){

    EXPECT_NO_THROW({ 
        Function1D f("./test_scenery/Function1D/function1D_integral_42.xml");
        EXPECT_EQ( 42.0 , f.get_weighted_mean_of_value() );  
    }); 
}
//------------------------------------------------------------------------------