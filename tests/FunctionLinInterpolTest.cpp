// Copyright 2014 Sebastian A. Mueller
#include <math.h>
#include "gtest/gtest.h"
#include "Core/function/function.h"
#include "Tools/AsciiIo.h"
using std::stringstream;
using std::string;
using std::vector;
using namespace relleums;

class Func1Test : public ::testing::Test {
 protected:
    const unsigned int table_size = 100;
    std::vector<std::vector<double>> table;
    std::vector<std::vector<double>> table_with_duplicate_argument;

    Func1Test() {
        init_table();
        init_table_with_duplicate_argument();
    }

    ~Func1Test() {}

    void init_table() {
        double phase = 0.25;
        for (unsigned int i = 0; i < table_size; i++) {
            double x = static_cast<double>(i)/static_cast<double>(table_size);
            double y = sin(x*2.0*M_PI + phase);
            std::vector<double> row = {x, y};
            table.push_back(row);
        }
    }

    void init_table_with_duplicate_argument() {
        for (unsigned int i = 0; i < table_size; i++) {
            double x = 1.337;
            double y = 1.0;
            std::vector<double> row = {x, y};
            table_with_duplicate_argument.push_back(row);
        }
    }
};

TEST_F(Func1Test, check_setup) {
    ASSERT_EQ(table_size, table.size());
    ASSERT_EQ(table_size, table_with_duplicate_argument.size());
    for (unsigned int i = 0; i < table_size; i++) {
        ASSERT_EQ(2u, table.at(i).size());
        ASSERT_EQ(2u, table_with_duplicate_argument.at(i).size());
    }
}

TEST_F(Func1Test, construct_using_matrix_of_vectors) {
    function::Func1 f(table);
    for (unsigned int i = 0; i < table.size()-1; i++) {
        double argument = table.at(i).at(0);
        double value = table.at(i).at(1);
        EXPECT_NEAR(value, f.evaluate(argument), 1e-9);
    }
}

TEST_F(Func1Test, access_below_lowest_argument) {
    function::Func1 f(table);
    double arg_below_definition = table.at(0).at(0) - 1e-9;
    EXPECT_THROW(f.evaluate(arg_below_definition), std::out_of_range);
}

TEST_F(Func1Test, access_at_highest_argument) {
    function::Func1 f(table);
    double upper_limit = table.back().at(0);
    EXPECT_THROW(f.evaluate(upper_limit), std::out_of_range);
}

TEST_F(Func1Test, generate_from_from_file) {
    std::string path = "numeric_table_IO/tim_sinus.csv";
    tsvio::write_table_to_file(table, path);

    function::Func1 f(tsvio::gen_table_from_file(path));

    // precision loss in ascii files, cant access boundarys sharp,
    // start in row 1 and stop one row before end.
    for (unsigned int i = 1; i < table.size()-1.; i++) {
        double argument = table[i][0];
        double value = table[i][1];
        EXPECT_NEAR(value, f.evaluate(argument), 1e-9);
    }
}

TEST_F(Func1Test, linear_interpolation) {
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

    function::Func1 f(two_entry_table);

    for (double x = -0.1; x < 1.1; x = x+0.011) {
        if (x >= 0.0 && x < 1.0)
            EXPECT_NEAR(x, f.evaluate(x), 1e-9);
        else
            EXPECT_THROW(f.evaluate(x), std::out_of_range);
    }
}

TEST_F(Func1Test, empty_table_for_Func1) {
    std::vector<std::vector<double>> empty_table;
    EXPECT_EQ(0u, empty_table.size());
    EXPECT_THROW(function::Func1 f(empty_table), std::invalid_argument);
    // It must throw anyhow when it is first used because its limit's range is
    // zero.
}

TEST_F(Func1Test, max_value) {
    for (double amp = 0.0; amp < 1.337*4.2; amp = amp+1e-2) {
        std::vector<std::vector<double>> table;
        for (double x = 0; x < 1.0; x = x+1e-2) {
            double y = amp*sin(x*2.0*M_PI);
            table.push_back({x, y});
        }
        function::Func1 f(table);
        EXPECT_NEAR(amp, f.max(), 1e-6);
        EXPECT_NEAR(-amp, f.min(), 1e-6);
    }
}