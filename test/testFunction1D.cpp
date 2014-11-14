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
  
    bool problem_detected = FALSE;
    try{
        Function1D f("ThisFileDoesNotExist.xml");
    }catch(XmlIoException &error){
        problem_detected = TRUE;
    } 
    EXPECT_EQ( TRUE , problem_detected );

    problem_detected = FALSE;
    try{
        Function1D f("./test_scenery/Function1D/function1D_complete.xml");
        EXPECT_EQ( "my_assignment" , f.name() ); 
        //f.disp();

    }catch(XmlIoException &error){
        problem_detected = TRUE;
        cout << error.what() << endl;
    } 
    EXPECT_EQ( FALSE , problem_detected ); 
}
//------------------------------------------------------------------------------
TEST_F(Function1DTest, MissingIdentifiers) {

    bool problem_detected = FALSE;
    try{
        Function1D f("./test_scenery/Function1D/function1D_missing_name.xml");
    }catch(XmlIoException &error){
        problem_detected = TRUE;
        //error.ReportException();
    } 
    EXPECT_EQ( TRUE , problem_detected );
}
//------------------------------------------------------------------------------
TEST_F(Function1DTest, InvalidFloatingNumber){
     bool problem_detected = FALSE;
    try{
        Function1D f
        ("./test_scenery/Function1D/function1D_invalid_floating_number.xml");
    }catch(XmlIoException &error){
        problem_detected = TRUE;
        //error.ReportException();
    } 
    EXPECT_EQ( TRUE , problem_detected );
}
//------------------------------------------------------------------------------
TEST_F(Function1DTest, SameArgumentTwice){
    bool problem_detected = FALSE;
    try{
        Function1D f
        ("./test_scenery/Function1D/function1D_same_argument_twice.xml");
    }catch(XmlIoException &error){
        problem_detected = TRUE;
        //error.ReportException();
    } 
    EXPECT_EQ( TRUE , problem_detected );
}
//------------------------------------------------------------------------------
TEST_F(Function1DTest, At){

    try{
        Function1D f("./test_scenery/Function1D/function1D_complete.xml");
        // f.disp();
        double arg = 0.0;
        for(uint i=0; i<150; i++){
          arg = arg + 6.666;
          //cout << arg << "," << f.at(arg) << "\n";
        }
    }catch(TracerException &error){
        cout << error.what() << endl;
    } 
}
//------------------------------------------------------------------------------
TEST_F(Function1DTest, Integral){

    try{
        Function1D f("./test_scenery/Function1D/function1D_integral_42.xml");
        
        EXPECT_EQ( 42.0 , f.mean_value() );     
    }catch(TracerException &error){
        cout << error.what() << endl;
    } 
}
//------------------------------------------------------------------------------