// Copyright 2018 Sebastian A. Mueller
#include <iostream>
#include <sstream>
#include "catch.hpp"
#include "merlict/merlict.h"
#include "merlict_visual/visual.h"
namespace ml = merlict;


TEST_CASE("PortablePixMapTest: write_and_read", "[merlict]") {
    ml::visual::Image img(16, 9);

    for (unsigned int c = 0; c < img.num_cols; c++) {
        for (unsigned int r = 0; r < img.num_rows; r++) {
            if (c == 8) {
                img.set_col_row(c, r, ml::Color(255, 0, 0));
            } else {
                if (r == 4) {
                    img.set_col_row(c, r, ml::Color(0, 255, 0));
                } else {
                    img.set_col_row(c, r, ml::Color(0, 0, 255));
                }
            }
        }
    }
    ml::visual::write_image_to_path(
        img,
        "resources/image_16x9_rgb.ppm");

    ml::visual::Image img_back = ml::visual::read_image_from_path(
        "resources/image_16x9_rgb.ppm");

    REQUIRE(img_back.num_cols == img.num_cols);
    REQUIRE(img_back.num_rows == img.num_rows);

    for (unsigned int c = 0; c < img.num_cols; c++) {
        for (unsigned int r = 0; r < img.num_rows; r++) {
            REQUIRE(img.at_col_row(c, r).r == img_back.at_col_row(c, r).r);
            REQUIRE(img.at_col_row(c, r).g == img_back.at_col_row(c, r).g);
            REQUIRE(img.at_col_row(c, r).b == img_back.at_col_row(c, r).b);
        }
    }
}
