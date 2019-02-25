// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "Tools/AsciiIo.h"
using std::stringstream;
using std::string;
using std::vector;
using namespace relleums;



TEST_CASE("AsciiIoTest: read_non_existing_file", "[mctracer]") {
    CHECK_THROWS_AS(tsvio::gen_table_from_file("non_existing_path/non_existing_file.csv"), std::runtime_error);
}

TEST_CASE("AsciiIoTest: read_empty_file", "[mctracer]") {
    std::vector<std::vector<double>> table =
        tsvio::gen_table_from_file("numeric_table_IO/empty.csv");
    CHECK(table.size() == 0u);
}

TEST_CASE("AsciiIoTest: read_valid_file_table_only", "[mctracer]") {
    std::vector<std::vector<double>> table =
        tsvio::gen_table_from_file("numeric_table_IO/matrix_4col_3row.csv");

    // wavelength, mean_free_path, absorbtion_coeff, wuff
    // 123.4, 100.0, 0.99, 133.7
    // 234.5, 101.0, 0.04, 13.37
    // 666.6, 88.8, 0.056, 1.337

    REQUIRE(table.size() == 3u);
    REQUIRE(table[0].size() == 4u);
    REQUIRE(table[1].size() == 4u);
    REQUIRE(table[2].size() == 4u);

    // 1st row
    CHECK(table[0][0] == 123.4);
    CHECK(table[0][1] == 100.0);
    CHECK(table[0][2] == 0.99);
    CHECK(table[0][3] == 133.7);

    // 2nd row
    CHECK(table[1][0] == 234.5);
    CHECK(table[1][1] == 101.0);
    CHECK(table[1][2] == 0.04);
    CHECK(table[1][3] == 13.37);

    // 3rd row
    CHECK(table[2][0] == 666.6);
    CHECK(table[2][1] == 88.8);
    CHECK(table[2][2] == 0.056);
    CHECK(table[2][3] == 1.337);
}

TEST_CASE("AsciiIoTest: write_table", "[mctracer]") {
    const unsigned int rows = 100;
    const unsigned int cols = 10;
    std::string path = "numeric_table_IO/written_table.csv";

    std::vector<std::vector<double>> table(rows);

    double value;
    for (unsigned int row = 0; row < rows; row++) {
        std::vector<double> single_row(cols);
        for (unsigned int col = 0; col < cols; col++) {
            value = static_cast<double>(col)*static_cast<double>(row)/
                (static_cast<double>(rows)*static_cast<double>(cols));
            single_row[col] = value;
        }
        table[row] = single_row;
    }

    tsvio::write_table_to_file(table, path);

    //----------------
    // read in again
    //----------------

    std::vector<std::vector<double>> table_in =
        tsvio::gen_table_from_file(path);

    // check column names
    REQUIRE(table_in.size() == rows);

    for (unsigned int row = 0; row < rows; row++) {
        REQUIRE(table_in.at(row).size() == cols);
        for (unsigned int col = 0; col < cols; col++) {
            CHECK(table[row][col] == Approx(table_in[row][col]).margin(1e-9));
        }
    }
}

TEST_CASE("AsciiIoTest: table_from_string_extra_new_line_in_the_end", "[mctracer]") {
    string text =   "266e-9\t0.03\n"
                    "277e-9\t0.1\n"
                    "283e-9\t0.2\n"
                    "300e-9\t0.254\n";

    vector<vector<double>> table = tsvio::gen_table_from_string(text);

    REQUIRE(4u == table.size());
    REQUIRE(2u == table.at(0).size());
    REQUIRE(2u == table.at(1).size());
    REQUIRE(2u == table.at(2).size());
    REQUIRE(2u == table.at(3).size());

    CHECK(266e-9 == table.at(0).at(0));
    CHECK(0.03 == table.at(0).at(1));

    CHECK(277e-9 == table.at(1).at(0));
    CHECK(0.1 == table.at(1).at(1));

    CHECK(283e-9 == table.at(2).at(0));
    CHECK(0.2 == table.at(2).at(1));

    CHECK(300e-9 == table.at(3).at(0));
    CHECK(0.254 == table.at(3).at(1));
}

TEST_CASE("AsciiIoTest: table_from_string_", "[mctracer]") {
    string text =   "266e-9\t0.03\n"
                    " 277e-9 0.1\n"
                    "  283e-9  0.2\n"
                    "  \t 300e-9 \t 0.254 ";

    vector<vector<double>> table = tsvio::gen_table_from_string(text);

    REQUIRE(4u == table.size());
    REQUIRE(2u == table.at(0).size());
    REQUIRE(2u == table.at(1).size());
    REQUIRE(2u == table.at(2).size());
    REQUIRE(2u == table.at(3).size());

    CHECK(266e-9 == table.at(0).at(0));
    CHECK(0.03 == table.at(0).at(1));

    CHECK(277e-9 == table.at(1).at(0));
    CHECK(0.1 == table.at(1).at(1));

    CHECK(283e-9 == table.at(2).at(0));
    CHECK(0.2 == table.at(2).at(1));

    CHECK(300e-9 == table.at(3).at(0));
    CHECK(0.254 == table.at(3).at(1));
}
