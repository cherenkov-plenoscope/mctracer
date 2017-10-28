#include "gtest/gtest.h"
#include "Scenery/ColorMap.h"

class ColorMapTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(ColorMapTest, init_empty) {

    ColorMap my_colors;
    EXPECT_FALSE(my_colors.has("red"));
}
//------------------------------------------------------------------------------
TEST_F(ColorMapTest, get_not_existing_color) {

    ColorMap my_colors;
    EXPECT_THROW(my_colors.get("red"), ColorMap::NoSuchKey);
}
//------------------------------------------------------------------------------
TEST_F(ColorMapTest, add_color_and_check_it_is_in) {

    ColorMap my_colors;
    my_colors.add("red", Color(255,0,0));
    EXPECT_TRUE(my_colors.has("red"));
}
//------------------------------------------------------------------------------
TEST_F(ColorMapTest, add_color_and_add_it_again) {

    ColorMap my_colors;
    my_colors.add("red", Color(255,0,0));
    EXPECT_THROW(
        my_colors.add("red", Color(128,0,0)),
        ColorMap::KeyAlreadyInUse
    );
}
//------------------------------------------------------------------------------
TEST_F(ColorMapTest, add_color_and_get_it_again) {

    ColorMap my_colors;
    Color novel_red =  Color(255,0,0);
    my_colors.add("red", novel_red);
    const Color* my_red = my_colors.get("red");

    EXPECT_EQ(my_red->r, novel_red.r);
    EXPECT_EQ(my_red->g, novel_red.g);
    EXPECT_EQ(my_red->b, novel_red.b);
}
//------------------------------------------------------------------------------
