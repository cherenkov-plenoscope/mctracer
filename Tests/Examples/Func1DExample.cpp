#include "gtest/gtest.h"
#include "Core/Function/Function.h"
#include "Tools/AsciiIo.h"
using namespace Function;

class FunctionExample : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(FunctionExample, function_limits) {

    //--func_limits--
    Limits limits(0.0, 1.0);
    //--func_limits_assert--
    EXPECT_THROW( limits.assert_contains(-0.1), Limits::OutOfRange );
    EXPECT_NO_THROW( limits.assert_contains(0.0) );
    EXPECT_NO_THROW( limits.assert_contains(0.5) );
    EXPECT_THROW( limits.assert_contains(1.0), Limits::OutOfRange );
    //--func_limits_constant--
    Constant con(1.337, limits);
    //--func_limits_const_call--
    EXPECT_THROW( con(-0.1), Limits::OutOfRange );
    EXPECT_NO_THROW( con(0.0) );
    EXPECT_NO_THROW( con(0.5) );
    EXPECT_THROW( con(1.0), Limits::OutOfRange );
    //--func_limits_call_end--
}
//------------------------------------------------------------------------------
TEST_F(FunctionExample, function_constant) {

    //--func_const--
    Constant c(1.337, Limits(0.0, 1.0));
    //--func_const_call--
    EXPECT_EQ( 1.337, c(0.0) );
    EXPECT_EQ( 1.337, c(0.2) );
    EXPECT_EQ( 1.337, c(0.3) );
    EXPECT_EQ( 1.337, c(0.43657657) );
    EXPECT_EQ( 1.337, c(0.78) );
    EXPECT_EQ( 1.337, c(0.9999) );
    //--func_const_call_end--

    AsciiIo::write_table_to_file(c.get_samples(1000),"Examples/Out/function_const.func");
}
//------------------------------------------------------------------------------
TEST_F(FunctionExample, function_polynom_1) {

    //--func_poly3--
    Polynom3 p3(0.0, 0.0, 1.0, 0.0, Limits(0.0, 1.0));
    //--func_poly3_call--
    EXPECT_EQ( 0.43657657, p3(0.43657657) );
    EXPECT_EQ( 0.78, p3(0.78) );
    EXPECT_EQ( 0.9999, p3(0.9999) );
    //--func_poly3_call_end--
    AsciiIo::write_table_to_file(p3.get_samples(1000),"Examples/Out/function_polynom1.func");
}
//------------------------------------------------------------------------------
TEST_F(FunctionExample, function_polynom_2) {

    //--func_poly3_quad--
    Polynom3 p3(0.0, 1.0, 0.0, 0.0, Limits(0.0, 1.0));
   
    EXPECT_NEAR( 0.0, p3(0.0) ,1e-9);
    EXPECT_NEAR( 0.25, p3(0.5) ,1e-9);
    EXPECT_NEAR( 0.04, p3(0.2) ,1e-9);
    //--func_poly3_quad_end--
    AsciiIo::write_table_to_file(p3.get_samples(1000),"Examples/Out/unction_polynom2.func");
}
//------------------------------------------------------------------------------
TEST_F(FunctionExample, function_polynom_3) {

    //--func_poly3_tri--
    Polynom3 p3(1.0, -3.0, -1.0, -3.0, Limits(-2.0, 4.0));
    //--func_poly3_tri_end--
    AsciiIo::write_table_to_file(p3.get_samples(1000),"Examples/Out/function_polynom3.func");
}
//------------------------------------------------------------------------------
TEST_F(FunctionExample, interpol) {

    //--look_up--
    std::vector<std::vector<double>> table = {
        {-0.5, 1.0},
        { 0.0, 0.5},
        { 0.5, 0.5},
        { 0.50001, 2.0},
        { 0.75, 2.3},
        { 0.75001, 0.5},
        { 1.0, 0.6},
        { 2.0, 0.3}
    };

    LinInterpol ip(table);
    //--look_up_end--
    AsciiIo::write_table_to_file(ip.get_samples(1000),"Examples/Out/function_interpol.func");
}
//------------------------------------------------------------------------------
TEST_F(FunctionExample, function_concat) {

    //--func_concat--
    Polynom3 f1(0.0, 1.0, 0.0, 0.0, Limits(-1.5, -0.5));
    Polynom3 f2(0.0, 0.0, 2.0, 0.0, Limits(-0.5, 0.5));
    Polynom3 f3(0.0, -1.0, 0.0, 0.0, Limits(0.5, 1.5));

    std::vector<const Func1D*> funcs = {&f1, &f2, &f3};
    Concat concat(funcs);
    //--func_concat_end--
    AsciiIo::write_table_to_file(concat.get_samples(1000),"Examples/Out/function_concat.func");
}
//------------------------------------------------------------------------------
TEST_F(FunctionExample, access) {

    //--func_access--
    Polynom3 p3(1.0, -3.0, -1.0, -3.0, Limits(-2.0, 4.0));

    double value1 = p3(-1.0);
    double value2 = p3(-0.0);
    //--func_access_end--
    value1 = value2;
    value2 = value1;
    //--func_access_sampling--
    std::vector<std::vector<double>> table = p3.get_samples(1000);
    EXPECT_EQ(1000, table.size());
    //--func_access_sampling_end--
    //--func_access_sampling_export--
    AsciiIo::write_table_to_file(p3.get_samples(7), "Examples/Out/my_p3.txt");
    //--func_access_sampling_export_end--
}
//------------------------------------------------------------------------------