// Copyright 2018 Sebastian A. Mueller
#include "catch.hpp"
#include "merlict/merlict.h"

using namespace merlict;



TEST_CASE("ImageTest: striding_order", "[merlict]") {
    visual::Image image(192, 108);
    unsigned int idx = 0;
    for (unsigned int col = 0; col < image.number_cols; col++) {
        for (unsigned int row = 0; row < image.number_rows; row++) {
            CHECK(idx == image._idx(col, row));
            idx++;
        }
    }
}
