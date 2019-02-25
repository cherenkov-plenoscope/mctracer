// Copyright 2014 Sebastian A. Mueller
#include "gtest/gtest.h"
#include "Tools/AsciiIo.h"
using std::stringstream;
using std::string;
using std::vector;
using namespace relleums;

class AsciiIoTest : public ::testing::Test {};

TEST_F(AsciiIoTest, read_non_existing_file) {
    EXPECT_THROW(std::vector<std::vector<double>> table = tsvio::gen_table_from_file("non_existing_path/non_existing_file.csv"), std::runtime_error);
}

TEST_F(AsciiIoTest, read_empty_file) {
    std::vector<std::vector<double>> table =
        tsvio::gen_table_from_file("numeric_table_IO/empty.csv");
    EXPECT_EQ(0u, table.size());
}

TEST_F(AsciiIoTest, read_valid_file_table_only) {
    std::vector<std::vector<double>> table =
        tsvio::gen_table_from_file("numeric_table_IO/matrix_4col_3row.csv");

    // wavelength, mean_free_path, absorbtion_coeff, wuff
    // 123.4, 100.0, 0.99, 133.7
    // 234.5, 101.0, 0.04, 13.37
    // 666.6, 88.8, 0.056, 1.337

    ASSERT_EQ(3u, table.size());
    ASSERT_EQ(4u, table[0].size());
    ASSERT_EQ(4u, table[1].size());
    ASSERT_EQ(4u, table[2].size());

    // 1st row
    EXPECT_EQ(123.4, table[0][0]);
    EXPECT_EQ(100.0, table[0][1]);
    EXPECT_EQ(0.99,  table[0][2]);
    EXPECT_EQ(133.7, table[0][3]);

    // 2nd row
    EXPECT_EQ(234.5, table[1][0]);
    EXPECT_EQ(101.0, table[1][1]);
    EXPECT_EQ(0.04, table[1][2]);
    EXPECT_EQ(13.37, table[1][3]);

    // 3rd row
    EXPECT_EQ(666.6, table[2][0]);
    EXPECT_EQ(88.8,  table[2][1]);
    EXPECT_EQ(0.056, table[2][2]);
    EXPECT_EQ(1.337, table[2][3]);
}

TEST_F(AsciiIoTest, write_table) {
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
    ASSERT_EQ(rows, table_in.size());

    for (unsigned int row = 0; row < rows; row++) {
        ASSERT_EQ(cols, table_in.at(row).size());
        for (unsigned int col = 0; col < cols; col++) {
            EXPECT_NEAR(table[row][col], table_in[row][col], 1e-9);
        }
    }
}

TEST_F(AsciiIoTest, table_from_string_extra_new_line_in_the_end) {
    string text =   "266e-9\t0.03\n"
                    "277e-9\t0.1\n"
                    "283e-9\t0.2\n"
                    "300e-9\t0.254\n";

    vector<vector<double>> table = tsvio::gen_table_from_string(text);

    ASSERT_EQ(table.size(), 4u);
    ASSERT_EQ(table.at(0).size(), 2u);
    ASSERT_EQ(table.at(1).size(), 2u);
    ASSERT_EQ(table.at(2).size(), 2u);
    ASSERT_EQ(table.at(3).size(), 2u);

    EXPECT_EQ(table.at(0).at(0), 266e-9);
    EXPECT_EQ(table.at(0).at(1), 0.03);

    EXPECT_EQ(table.at(1).at(0), 277e-9);
    EXPECT_EQ(table.at(1).at(1), 0.1);

    EXPECT_EQ(table.at(2).at(0), 283e-9);
    EXPECT_EQ(table.at(2).at(1), 0.2);

    EXPECT_EQ(table.at(3).at(0), 300e-9);
    EXPECT_EQ(table.at(3).at(1), 0.254);
}

TEST_F(AsciiIoTest, table_from_string_) {
    string text =   "266e-9\t0.03\n"
                    " 277e-9 0.1\n"
                    "  283e-9  0.2\n"
                    "  \t 300e-9 \t 0.254 ";

    vector<vector<double>> table = tsvio::gen_table_from_string(text);

    ASSERT_EQ(table.size(), 4u);
    ASSERT_EQ(table.at(0).size(), 2u);
    ASSERT_EQ(table.at(1).size(), 2u);
    ASSERT_EQ(table.at(2).size(), 2u);
    ASSERT_EQ(table.at(3).size(), 2u);

    EXPECT_EQ(table.at(0).at(0), 266e-9);
    EXPECT_EQ(table.at(0).at(1), 0.03);

    EXPECT_EQ(table.at(1).at(0), 277e-9);
    EXPECT_EQ(table.at(1).at(1), 0.1);

    EXPECT_EQ(table.at(2).at(0), 283e-9);
    EXPECT_EQ(table.at(2).at(1), 0.2);

    EXPECT_EQ(table.at(3).at(0), 300e-9);
    EXPECT_EQ(table.at(3).at(1), 0.254);
}
