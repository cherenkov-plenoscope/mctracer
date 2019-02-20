// Copyright 2018 Sebastian A. Mueller
#include "gtest/gtest.h"
#include "Visual/Image.h"

class ImageTest : public ::testing::Test {};

TEST_F(ImageTest, striding_order) {
    visual::Image image(192, 108);
    unsigned int idx = 0;
    for (unsigned int col = 0; col < image.number_cols; col++) {
        for (unsigned int row = 0; row < image.number_rows; row++) {
            EXPECT_EQ(image._idx(col, row), idx);
            idx++;
        }
    }
}
