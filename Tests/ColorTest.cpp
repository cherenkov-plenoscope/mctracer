// Copyright 2014 Sebastian A. Mueller
#include "gtest/gtest.h"
#include "Core/Color.h"

class ColorTest : public ::testing::Test {};

TEST_F(ColorTest, assert_8bit_range) {
    EXPECT_NO_THROW(
        Color col(0, 0, 0));
    EXPECT_NO_THROW(
        Color col(128, 128, 128));
    EXPECT_NO_THROW(
        Color col(255, 255, 255));
    EXPECT_THROW(
        Color col(256, 0, 0),
        std::out_of_range);
    EXPECT_THROW(
        Color col(0, 256, 0),
        std::out_of_range);
    EXPECT_THROW(
        Color col(0, 0, 256),
        std::out_of_range);
    EXPECT_THROW(
        Color col(-1, 0, 0),
        std::out_of_range);
    EXPECT_THROW(
        Color col(0, -1, 0),
        std::out_of_range);
    EXPECT_THROW(
        Color col(0, 0, -1),
        std::out_of_range);
}

TEST_F(ColorTest, default_ctor) {
    Color c;
    EXPECT_EQ(c.r, 128);
    EXPECT_EQ(c.g, 128);
    EXPECT_EQ(c.b, 128);
}
