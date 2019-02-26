// Copyright 2014 Sebastian A. Mueller
#include <math.h>
#include "catch.hpp"
#include "merlict/function/function.h"
#include "Tools/AsciiIo.h"
using std::stringstream;
using std::string;
using std::vector;
using namespace relleums;

struct Func1Test {
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

TEST_CASE("Func1Test: check_setup", "[mctracer]") {
    Func1Test ft;
    REQUIRE(ft.table.size() == ft.table_size);
    REQUIRE(ft.table_with_duplicate_argument.size() == ft.table_size);
    for (unsigned int i = 0; i < ft.table_size; i++) {
        REQUIRE(ft.table.at(i).size() == 2u);
        REQUIRE(ft.table_with_duplicate_argument.at(i).size() == 2u);
    }
}

TEST_CASE("Func1Test: construct_using_matrix_of_vectors", "[mctracer]") {
    Func1Test ft;
    function::Func1 f(ft.table);
    for (unsigned int i = 0; i < ft.table.size()-1; i++) {
        double argument = ft.table.at(i).at(0);
        double value = ft.table.at(i).at(1);
        CHECK(value == Approx(f.evaluate(argument)).margin(1e-9));
    }
}

TEST_CASE("Func1Test: access_below_lowest_argument", "[mctracer]") {
    Func1Test ft;
    function::Func1 f(ft.table);
    double arg_below_definition = ft.table.at(0).at(0) - 1e-9;
    CHECK_THROWS_AS(f.evaluate(arg_below_definition), std::out_of_range);
}

TEST_CASE("Func1Test: access_at_highest_argument", "[mctracer]") {
    Func1Test ft;
    function::Func1 f(ft.table);
    double upper_limit = ft.table.back().at(0);
    CHECK_THROWS_AS(f.evaluate(upper_limit), std::out_of_range);
}

TEST_CASE("Func1Test: generate_from_from_file", "[mctracer]") {
    Func1Test ft;
    std::string path = "tsvio/tim_sinus.csv";
    tsvio::write_table_to_file(ft.table, path);

    function::Func1 f(tsvio::gen_table_from_file(path));

    // precision loss in ascii files, cant access boundarys sharp,
    // start in row 1 and stop one row before end.
    for (unsigned int i = 1; i < ft.table.size()-1.; i++) {
        double argument = ft.table[i][0];
        double value = ft.table[i][1];
        CHECK(value == Approx(f.evaluate(argument)).margin(1e-9));
    }
}

TEST_CASE("Func1Test: linear_interpolation", "[mctracer]") {
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
            CHECK(x == Approx(f.evaluate(x)).margin(1e-9));
        else
            CHECK_THROWS_AS(f.evaluate(x), std::out_of_range);
    }
}

TEST_CASE("Func1Test: empty_table_for_Func1", "[mctracer]") {
    std::vector<std::vector<double>> empty_table;
    CHECK(empty_table.size() == 0u);
    CHECK_THROWS_AS(function::Func1(empty_table), std::invalid_argument);
    // It must throw anyhow when it is first used because its limit's range is
    // zero.
}

TEST_CASE("Func1Test: max_value", "[mctracer]") {
    for (double amp = 0.0; amp < 1.337*4.2; amp = amp+1e-2) {
        std::vector<std::vector<double>> table;
        for (double x = 0; x < 1.0; x = x+1e-2) {
            double y = amp*sin(x*2.0*M_PI);
            table.push_back({x, y});
        }
        function::Func1 f(table);
        CHECK(amp == Approx(f.max()).margin(1e-6));
        CHECK(-amp == Approx(f.min()).margin(1e-6));
    }
}
