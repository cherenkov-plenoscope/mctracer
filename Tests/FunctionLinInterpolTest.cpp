#include "gtest/gtest.h"
#include <math.h> 
#include "Core/Function/LinInterpolFunction.h"
#include "Tools/AsciiIo.h"
using std::stringstream;
using std::string;
using std::vector;

class LinInterpolTest : public ::testing::Test {
    protected:

    const unsigned int table_size = 100;
    std::vector<std::vector<double>> table;
    std::vector<std::vector<double>> table_with_duplicate_argument;

    LinInterpolTest() {
        init_table();
        init_table_with_duplicate_argument();
    }

    ~LinInterpolTest() {}

    void init_table() {

        double phase = 0.25;
        for(unsigned int i=0; i<table_size; i++) {

            double x = double(i)/double(table_size);
            double y = sin(x*2.0*M_PI + phase);

            std::vector<double> row = {x, y};
            table.push_back(row);
        }
    }

    void init_table_with_duplicate_argument() {

        for(unsigned int i=0; i<table_size; i++) {

            double x = 1.337;
            double y = 1.0;

            std::vector<double> row = {x, y};
            table_with_duplicate_argument.push_back(row);
        }
    }
};
//------------------------------------------------------------------------------
TEST_F(LinInterpolTest, check_setup) {
    
    ASSERT_EQ(table_size, table.size());
    ASSERT_EQ(table_size, table_with_duplicate_argument.size());

    for(unsigned int i=0; i<table_size; i++) {
        ASSERT_EQ(2u, table.at(i).size());
        ASSERT_EQ(2u, table_with_duplicate_argument.at(i).size());
    }
}
//------------------------------------------------------------------------------
TEST_F(LinInterpolTest, construct_using_matrix_of_vectors) {

    Function::LinInterpol f(table);

    for(unsigned int i=0; i<table.size()-1; i++) {
        double argument = table.at(i).at(0);
        double value = table.at(i).at(1);
        EXPECT_NEAR(value, f.evaluate(argument), 1e-9);
    }
}
//------------------------------------------------------------------------------
TEST_F(LinInterpolTest, access_below_lowest_argument) {

    Function::LinInterpol f(table);
    double arg_below_definition = table.at(0).at(0) - 1e-9;
    EXPECT_THROW(f.evaluate(arg_below_definition), std::out_of_range);
}
//------------------------------------------------------------------------------
TEST_F(LinInterpolTest, access_at_highest_argument) {

    Function::LinInterpol f(table);
    double upper_limit = table.back().at(0);
    EXPECT_THROW(f.evaluate(upper_limit), std::out_of_range);
}
//------------------------------------------------------------------------------
TEST_F(LinInterpolTest, generate_from_from_file) {

    std::string path = "numeric_table_IO/tim_sinus.csv";
    AsciiIo::write_table_to_file(table ,path);

    Function::LinInterpol f(AsciiIo::gen_table_from_file(path));

    // precision loss in ascii files, cant access boundarys sharp,
    // start in row 1 and stop one row before end.
    for(unsigned int i=1; i<table.size()-1.; i++) {
        double argument = table[i][0];
        double value = table[i][1];
        EXPECT_NEAR(value, f.evaluate(argument), 1e-9);
    }
}
//------------------------------------------------------------------------------
TEST_F(LinInterpolTest, linear_interpolation) {

    std::vector<std::vector<double>> two_entry_table = { 
        {0.0, 0.0},
        {1.0, 1.0} 
    };

    //  1-|     /   two_entry_table = y = f(x) = x
    //    |    /
    //    |   /
    //    |  /
    //    | /
    //    |/____.___
    //   0      1

    Function::LinInterpol f(two_entry_table);

    for(double x=-0.1; x<1.1; x=x+0.011) {

        if(x >= 0.0 && x < 1.0)
            EXPECT_NEAR(x, f.evaluate(x), 1e-9);
        else
            EXPECT_THROW(f.evaluate(x), std::out_of_range);
    }
}
//------------------------------------------------------------------------------
TEST_F(LinInterpolTest, empty_table_for_LinInterpol) {

    std::vector<std::vector<double>> empty_table;

    EXPECT_EQ(0u, empty_table.size());
    EXPECT_THROW(Function::LinInterpol f(empty_table), std::invalid_argument);
    // It must throw anyhow when it is first used because its limit's range is
    // zero.
}
//------------------------------------------------------------------------------
TEST_F(LinInterpolTest, max_value) {

    for(double amp=0.0; amp<1.337*4.2; amp=amp+1e-2) {
    
        std::vector<std::vector<double>> table;

        for(double x=0; x<1.0; x=x+1e-2) {
            double y = amp*sin(x*2.0*M_PI);
            table.push_back({x, y});
        }

        Function::LinInterpol f(table);
        EXPECT_NEAR(amp, f.get_max_value(), 1e-6);
        EXPECT_NEAR(-amp, f.get_min_value(), 1e-6);
    }
}
//------------------------------------------------------------------------------
TEST_F(LinInterpolTest, all_in_xml) {

    string path = "xml/all_in_xml_function.xml";
}
//------------------------------------------------------------------------------