#include "gtest/gtest.h"
#include "Core/Color.h"

class ColorTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(ColorTest, 8bit_range) {

    EXPECT_NO_THROW(
        Color col(0,0,0);
    );

    EXPECT_NO_THROW(
        Color col(128,128,128);
    );

    EXPECT_NO_THROW(
        Color col(255,255,255);
    );

    EXPECT_THROW(
        Color col(256,0,0);,
        Color::NotIn8bitRange
    );

    EXPECT_THROW(
        Color col(0,256,0);,
        Color::NotIn8bitRange
    );

    EXPECT_THROW(
        Color col(0,0,256);,
        Color::NotIn8bitRange
    );

    EXPECT_THROW(
        Color col(-1,0,0);,
        Color::NotIn8bitRange
    );

    EXPECT_THROW(
        Color col(0,-1,0);,
        Color::NotIn8bitRange
    );

    EXPECT_THROW(
        Color col(0,0,-1);,
        Color::NotIn8bitRange
    );
}
//------------------------------------------------------------------------------