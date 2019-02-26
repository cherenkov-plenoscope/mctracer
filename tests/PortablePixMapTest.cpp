// Copyright 2018 Sebastian A. Mueller
#include <iostream>
#include <sstream>
#include "catch.hpp"
#include "merlict/merlict.h"

using namespace merlict;



TEST_CASE("PortablePixMapTest: write_and_read", "[merlict]") {
    visual::Image img(16, 9);

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
    visual::ppm::write_image_to_path(
        img,
        "InOut/image_16x9_rgb.ppm");

    visual::Image img_back = visual::ppm::read_image_from_path(
        "InOut/image_16x9_rgb.ppm");

    REQUIRE(img_back.number_cols == img.number_cols);
    REQUIRE(img_back.number_rows == img.number_rows);

    for (unsigned int c = 0; c < img.number_cols; c++) {
        for (unsigned int r = 0; r < img.number_rows; r++) {
            REQUIRE(img.at_col_row(c, r).r == img_back.at_col_row(c, r).r);
            REQUIRE(img.at_col_row(c, r).g == img_back.at_col_row(c, r).g);
            REQUIRE(img.at_col_row(c, r).b == img_back.at_col_row(c, r).b);
        }
    }
}
