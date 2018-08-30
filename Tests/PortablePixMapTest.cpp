// Copyright 2018 Sebastian A. Mueller
#include <iostream>
#include <sstream>
#include "gtest/gtest.h"
#include "Visual/PortablePixMap.h"

class PortablePixMapTest : public ::testing::Test {};

TEST_F(PortablePixMapTest, write_and_read) {
    Visual::Image img(16, 9);

    for (unsigned int c = 0; c < img.number_cols; c++) {
        for (unsigned int r = 0; r < img.number_rows; r++) {
            if (c == 8) {
                img.set_col_row(c, r, Color(255, 0, 0));
            } else {
                if (r == 4) {
                    img.set_col_row(c, r, Color(0, 255, 0));
                } else {
                    img.set_col_row(c, r, Color(0, 0, 255));
                }
            }
        }
    }
    Visual::ppm::write_image_to_path(
        img,
        "InOut/image_16x9_rgb.ppm");

    Visual::Image img_back = Visual::ppm::read_image_from_path(
        "InOut/image_16x9_rgb.ppm");

    ASSERT_EQ(img.number_cols, img_back.number_cols);
    ASSERT_EQ(img.number_rows, img_back.number_rows);

    for (unsigned int c = 0; c < img.number_cols; c++) {
        for (unsigned int r = 0; r < img.number_rows; r++) {
            ASSERT_EQ(
                img.at_col_row(c, r).r,
                img_back.at_col_row(c, r).r);
            ASSERT_EQ(
                img.at_col_row(c, r).g,
                img_back.at_col_row(c, r).g);
            ASSERT_EQ(
                img.at_col_row(c, r).b,
                img_back.at_col_row(c, r).b);
        }
    }
}
