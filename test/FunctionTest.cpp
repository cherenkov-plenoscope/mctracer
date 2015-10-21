#include "gtest/gtest.h"
#include "Core/Function/Func1DFunction.h"
#include "Core/Function/ConstantFunction.h"
#include "Core/Function/Polynom3Function.h"
#include "Core/Function/LinInterpolFunction.h"
#include "Core/Function/ConcatFunction.h"

class FunctionTest : public ::testing::Test {};
//------------------------------------------------------------------------------
// Limits
//------------------------------------------------------------------------------
TEST_F(FunctionTest, causal_limits_correct) {
    
    EXPECT_NO_THROW(Function::Limits l(0.0, 1.0));
}
//------------------------------------------------------------------------------
TEST_F(FunctionTest, causal_limits_false) {
    
    EXPECT_THROW(Function::Limits l(0.0, -1.0);, TracerException);
}
//------------------------------------------------------------------------------
TEST_F(FunctionTest, causal_limits_lower_included) {
    
    Function::Limits l(0.0, 1.0);
    EXPECT_NO_THROW(l.assert_contains(0.0));
}
//------------------------------------------------------------------------------
TEST_F(FunctionTest, causal_limits_upper_excluded) {
    
    Function::Limits l(0.0, 1.0);
    EXPECT_THROW(l.assert_contains(1.0), TracerException);
}
//------------------------------------------------------------------------------
TEST_F(FunctionTest, causal_limits_above_upper) {
    
    Function::Limits l(0.0, 1.0);
    EXPECT_THROW(l.assert_contains(1.1), TracerException);
}
//------------------------------------------------------------------------------
TEST_F(FunctionTest, causal_limits_below_lower) {
    
    Function::Limits l(0.0, 1.0);
    EXPECT_THROW(l.assert_contains(-0.1), TracerException);
}
//------------------------------------------------------------------------------
TEST_F(FunctionTest, check_limits) {
    
    Function::Limits l(4.2, 133.7);
    EXPECT_EQ(4.2, l.get_lower());
    EXPECT_EQ(133.7, l.get_upper());
}
//------------------------------------------------------------------------------
TEST_F(FunctionTest, default_limits) {
    
    Function::Limits l;

    EXPECT_EQ(0.0, l.get_lower());
    EXPECT_EQ(0.0, l.get_upper());

    EXPECT_THROW(l.assert_contains(-0.1), TracerException);
    EXPECT_THROW(l.assert_contains( 0.0), TracerException);
    EXPECT_THROW(l.assert_contains(+0.1), TracerException);
}
//------------------------------------------------------------------------------
TEST_F(FunctionTest, limit_default_range) {
    
    Function::Limits l;
    EXPECT_EQ(0.0, l.get_range());
}
//------------------------------------------------------------------------------
TEST_F(FunctionTest, limit_range) {
    
    Function::Limits l(0.0, 1.0);
    EXPECT_EQ(1.0, l.get_range());
}
//------------------------------------------------------------------------------
// Constant
//------------------------------------------------------------------------------
TEST_F(FunctionTest, ConstantFunction) {
  
    Function::Limits l(-100.0, 100.0);

    double y = 133.7;
    Function::Constant f(y, l);

    for(double x=-1e2; x<1e2; x=x+0.55)
        EXPECT_EQ(y, f(x));
}
//------------------------------------------------------------------------------
TEST_F(FunctionTest, constant_function_limits) {
  
    Function::Limits l(0.0, 1.0);

    double y = 133.7;
    Function::Constant f(y, l);

    EXPECT_THROW(f(-0.5), TracerException);
    EXPECT_NO_THROW(f(0.0));
    EXPECT_NO_THROW(f(0.5));
    EXPECT_THROW(f(1.0), TracerException);
    EXPECT_THROW(f(1.5), TracerException);
}
//------------------------------------------------------------------------------
// Polynom3
//------------------------------------------------------------------------------
TEST_F(FunctionTest, Polynom3) {

    Function::Limits l(0.0, 1.0);
  
    Function::Polynom3 f(1.0, 1.0, 1.0, 1.0, l);

    for(double x=-0.0; x<1.0; x=x+0.11)
        EXPECT_EQ(x*x*x + x*x + x + 1.0, f(x));
}
//------------------------------------------------------------------------------
TEST_F(FunctionTest, Polynom3_limits) {

    Function::Limits l(0.0, 1.0);
  
    Function::Polynom3 f(1.0, 1.0, 1.0, 1.0, l);

    EXPECT_THROW(f(-0.5), TracerException);
    EXPECT_NO_THROW(f(0.0));
    EXPECT_NO_THROW(f(0.5));
    EXPECT_THROW(f(1.0), TracerException);
    EXPECT_THROW(f(1.5), TracerException);
}
//------------------------------------------------------------------------------
// Sampling
//------------------------------------------------------------------------------
TEST_F(FunctionTest, sampling_table_size) {

    Function::Limits l(-4.2, 1.337);

    Function::Polynom3 f(0.0, 0.0, 1.0, 0.0, l); // f(x) = x
    std::vector<std::vector<double>> sample_xy = f.get_samples(1000);

    ASSERT_EQ(1000, sample_xy.size());
    for(uint i=0; i<sample_xy.size(); i++)
        ASSERT_EQ(2, sample_xy.at(i).size());
}
//------------------------------------------------------------------------------
TEST_F(FunctionTest, sampling_table_x_values) {

    const uint samples = 10;
    Function::Limits l(-4.2, 1.337);

    Function::Polynom3 f(0.0, 1.0, 0.0, 0.0, l); // f(x) = x^2
    std::vector<std::vector<double>> sample_xy = f.get_samples(samples);

    //std::cout << get_print(sample_xy);

    ASSERT_EQ(samples, sample_xy.size());
    for(uint i=0; i<sample_xy.size(); i++) {

        ASSERT_EQ(2, sample_xy.at(i).size());
        double x = -4.2 + double(i)*(1.337+4.2)/double(samples);

        EXPECT_NEAR(x, sample_xy[i][0], 1e-9);
    }
}
//------------------------------------------------------------------------------
TEST_F(FunctionTest, sampling_table_y_values) {

    const uint samples = 10;
    Function::Limits l(-4.2, 1.337);

    Function::Polynom3 f(0.0, 1.0, 0.0, 0.0, l); // f(x) = x^2
    std::vector<std::vector<double>> sample_xy = f.get_samples(samples);

    //std::cout << get_print(sample_xy);

    ASSERT_EQ(samples, sample_xy.size());
    for(uint i=0; i<sample_xy.size(); i++) {

        ASSERT_EQ(2, sample_xy.at(i).size());
        double x = sample_xy[i][0];

        EXPECT_NEAR(x*x, f(x), 1e-9);
        EXPECT_NEAR(x*x, sample_xy[i][1], 1e-9);
    }
}
//------------------------------------------------------------------------------
// Concat
//------------------------------------------------------------------------------
TEST_F(FunctionTest, Concat_valid_limits) {

    Function::Limits l1(0.0, 1.0);
    Function::Constant f1(1.0, l1);

    Function::Limits l2(1.0, 2.0);
    Function::Constant f2(4.2, l2);

    Function::Limits l3(2.0, 3.0);
    Function::Constant f3(1.0, l3);

    std::vector<Function::Func1D*> f = {&f1, &f2, &f3};

    Function::Concat con(f);

    EXPECT_EQ(0.0, con.get_limits().get_lower());
    EXPECT_EQ(3.0, con.get_limits().get_upper());
}
//------------------------------------------------------------------------------
TEST_F(FunctionTest, Concat_bad_limits) {

    Function::Limits l1(0.0, 0.9);
    Function::Polynom3 f1(1.0, 1.0, 1.0, 1.0, l1);

    Function::Limits l2(1.1, 2.0);
    Function::Constant f2(4.2, l2);

    std::vector<Function::Func1D*> f = {&f1, &f2};

    EXPECT_THROW(Function::Concat con(f), TracerException);
}
//------------------------------------------------------------------------------
TEST_F(FunctionTest, Concat_bad_access) {

    Function::Limits l1(0.0, 1.0);
    Function::Polynom3 f1(1.0, 1.0, 1.0, 1.0, l1);

    Function::Limits l2(1.0, 2.0);
    Function::Constant f2(4.2, l2);

    std::vector<Function::Func1D*> f = {&f1, &f2};
    Function::Concat con(f);

    EXPECT_THROW(con(-0.5), TracerException);
    EXPECT_NO_THROW(con(0.0));
    EXPECT_NO_THROW(con(0.5));
    EXPECT_NO_THROW(con(1.0));
    EXPECT_NO_THROW(con(1.5));
    EXPECT_THROW(con(2.0), TracerException);
    EXPECT_THROW(con(2.5), TracerException);
}
//------------------------------------------------------------------------------
TEST_F(FunctionTest, Concat_y_in_different_sub_functions) {

    Function::Limits l1(0.0, 1.0);
    Function::Constant f1(1.337, l1);

    Function::Limits l2(1.0, 2.0);
    Function::Constant f2(4.2, l2);

    Function::Limits l3(2.0, 3.0);
    Function::Constant f3(3.141, l3);

    std::vector<Function::Func1D*> f = {&f1, &f2, &f3};

    Function::Concat con(f);

    for(double x=-0.5; x<3.5; x=x+0.011) {

        if(x >= 0.0 && x < 1.0)
            EXPECT_EQ(1.337, con(x));
        else if(x >= 1.0 && x < 2.0)
            EXPECT_EQ(4.2, con(x));
        else if(x >= 2.0 && x < 3.0)
            EXPECT_EQ(3.141, con(x));
        else
            EXPECT_THROW(con(x), TracerException);
    }
}
//------------------------------------------------------------------------------