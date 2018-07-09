// Copyright 2018 Sebastian A. Mueller
#include <iostream>
#include <sstream>
#include "gtest/gtest.h"
#include "Visual/PortablePixMap.h"

class PortablePixMapTest : public ::testing::Test {};

TEST_F(PortablePixMapTest, write_and_read) {
    Visual::Image img(16, 9);

    for (unsigned int c = 0; c < img.get_number_of_cols(); c++) {
        for (unsigned int r = 0; r < img.get_number_of_rows(); r++) {
            if (c == 8) {
                img.set_row_col_to_color(r, c, Color(255, 0, 0));
            } else {
                if (r == 4) {
                    img.set_row_col_to_color(r, c, Color(0, 255, 0));
                } else {
                    img.set_row_col_to_color(r, c, Color(0, 0, 255));
                }
            }
        }
    }

    std::ofstream fout;
    fout.open("test_image_16x9_rgb.ppm", std::ios::out | std::ios::binary);
    Visual::append_picture_to_file(img, fout);
    fout.close();

    std::ifstream fin;
    fin.open("test_image_16x9_rgb.ppm", std::ios::in | std::ios::binary);
    Visual::Image img_back = Visual::read_picture_from_file(fin);
    fin.close();

    ASSERT_EQ(img.get_number_of_cols(), img_back.get_number_of_cols());
    ASSERT_EQ(img.get_number_of_rows(), img_back.get_number_of_rows());

    for (unsigned int c = 0; c < img.get_number_of_cols(); c++) {
        for (unsigned int r = 0; r < img.get_number_of_rows(); r++) {
            ASSERT_EQ(
                img.get_color_row_col(r, c).r,
                img_back.get_color_row_col(r, c).r);
            ASSERT_EQ(
                img.get_color_row_col(r, c).g,
                img_back.get_color_row_col(r, c).g);
            ASSERT_EQ(
                img.get_color_row_col(r, c).b,
                img_back.get_color_row_col(r, c).b);
        }
    }
}
