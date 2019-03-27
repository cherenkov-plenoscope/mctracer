// Copyright 2018 Sebastian A. Mueller
#include "tests/catch.hpp"
#include "merlict_visual/visual.h"
namespace ml = merlict;


TEST_CASE("ImageTest: striding_order", "[merlict]") {
    ml::visual::Image image(192, 108);
    unsigned int idx = 0;
    for (unsigned int col = 0; col < image.num_cols; col++) {
        for (unsigned int row = 0; row < image.num_rows; row++) {
            CHECK(idx == image._idx(col, row));
            idx++;
        }
    }
}
