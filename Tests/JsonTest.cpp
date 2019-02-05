// Copyright 2014 Sebastian A. Mueller
#include "gtest/gtest.h"
#include "Scenery/json.hpp"
#include "Scenery/json_to_scenery.h"
#include "Scenery/Scenery.h"
#include "Scenery/Primitive/Primitive.h"
//#include "Visual/Config.h"
//#include "Visual/FlyingCamera.h"
namespace nl = nlohmann;
using std::string;
using StringTools::is_equal;

class JsonTest : public ::testing::Test {};

TEST_F(JsonTest, empty_path) {
    const std::string path = "";
    Scenery s;
    EXPECT_THROW(
        mct::json::append_to_frame_in_scenery(&s.root, &s, path),
        std::runtime_error);
}

TEST_F(JsonTest, mini_scenery_with_stl) {
    const std::string path =  "json/mini_scenery.json";
    Scenery s;
    mct::json::append_to_frame_in_scenery(&s.root, &s, path);
    s.root.init_tree_based_on_mother_child_relations();
    //Visual::Config cfg;
    //Visual::FlyingCamera(&s.root, &cfg);
}

TEST_F(JsonTest, valid_color) {
    nl::json jrgb = {{"red", {255, 0, 0}}};
    Color c = mct::json::to_color(jrgb["red"]);
    EXPECT_EQ(c.r, 255);
    EXPECT_EQ(c.g, 0);
    EXPECT_EQ(c.b, 0);
}

TEST_F(JsonTest, color_rg_no_b) {
    nl::json jrgb = {{"red", {255, 0}}};
    EXPECT_THROW(mct::json::to_color(jrgb["red"]), mct::json::BadColor);
}

TEST_F(JsonTest, color_no_array_but_string) {
    nl::json jrgb = {{"rgb", "woot?"}};
    EXPECT_THROW(mct::json::to_color(jrgb["rgb"]), mct::json::BadColor);
}

TEST_F(JsonTest, fine_colors) {
    auto j = R"(
    {
      "colors": [
        {"name":"red", "rgb":[255, 0, 0]},
        {"name":"green", "rgb":[0, 255, 0]},
        {"name":"blue", "rgb":[0, 0, 255]}
      ]
    }
    )"_json;
    ColorMap cmap;
    mct::json::assert_key(j, "colors");
    mct::json::add_colors(&cmap, j["colors"]);
    EXPECT_TRUE(cmap.has("red"));
    EXPECT_EQ(*cmap.get("red"), Color(255, 0, 0));
    EXPECT_TRUE(cmap.has("green"));
    EXPECT_EQ(*cmap.get("green"), Color(0, 255, 0));
    EXPECT_TRUE(cmap.has("blue"));
    EXPECT_EQ(*cmap.get("blue"), Color(0, 0, 255));
}

TEST_F(JsonTest, empty_colors) {
    nl::json j = R"({"colors": {}})"_json;
    ColorMap cmap;
    mct::json::assert_key(j, "colors");
    mct::json::add_colors(&cmap, j["colors"]);
    EXPECT_EQ(cmap.colors.size(), 0u);
}

TEST_F(JsonTest, parse_mini_scenery) {
    auto jscenery = R"(
    { "functions":[
        { "name":"leaf_reflection",
          "argument_versus_value":[
            [200e-9, 0.5],
            [1200e-9, 0.5]]},
        { "name":"pole_dull",
          "argument_versus_value":[
            [200e-9, 0.0],
            [1200e-9, 0.0]]}
      ],
      "colors":
      [
        {"name":"grass_green", "rgb":[22, 91, 49]},
        {"name":"orange", "rgb":[255, 91, 49]},
        {"name":"leaf_green", "rgb":[22, 200, 49]},
        {"name":"wood_brown", "rgb":[200, 200, 0]}
      ],
      "children": [
        {
          "type": "Frame",
          "name": "tree",
          "pos": [5, 5, -2],
          "rot": [0, 0, 0],
          "children": [
            {
              "type": "Sphere",
              "name": "leaf_ball",
              "pos": [0, 0, 3],
              "rot": [0, 0, 0],
              "radius": 1.5,
              "surface": {
                "outer_color": "leaf_green",
                "inner_color": "leaf_green"},
              "children": []
            },
            {
              "type": "Cylinder",
              "name": "pole",
              "start_pos": [0, 0, 0],
              "end_pos": [0, 0, 1.5],
              "radius": 0.25,
              "surface": {
                "outer_color": "wood_brown",
                "inner_color": "wood_brown"},
              "children": []
            }
          ]
        }
      ]
    }
    )"_json;

    Scenery s;
    mct::json::append_to_frame_in_scenery(&s.root, &s, jscenery);

    EXPECT_EQ(s.root.get_name(), "root");
    EXPECT_TRUE(s.root.has_children());

    EXPECT_TRUE(s.functions.has("leaf_reflection"));
    EXPECT_TRUE(s.functions.has("pole_dull"));

    EXPECT_TRUE(s.colors.has("grass_green"));
    EXPECT_TRUE(s.colors.has("leaf_green"));
    EXPECT_TRUE(s.colors.has("orange"));

    const std::vector<Frame*>* children = s.root.get_children();
    ASSERT_EQ(children->size(), 1u);
    EXPECT_EQ(children->at(0)->get_name(), "tree");
}

TEST_F(JsonTest, linear_interpolation_function) {
    auto j = R"(
    [
      {
        "name":"foo",
        "argument_versus_value":[
          [0, 5],
          [1, 4],
          [2, 3],
          [3, 2],
          [4, 1],
          [5, 0]
        ]
      }
    ]
    )"_json;

    FunctionMap functions;
    mct::json::add_functions(&functions, j);
    EXPECT_TRUE(functions.has("foo"));
    EXPECT_TRUE(functions.get("foo")->limits().upper() == 5);
    EXPECT_TRUE(functions.get("foo")->limits().lower() == 0);
    EXPECT_NEAR(functions.get("foo")->evaluate(0.0), 5.0, 1e-9);
    EXPECT_NEAR(functions.get("foo")->evaluate(0.5), 4.5, 1e-9);
    EXPECT_NEAR(functions.get("foo")->evaluate(1.0), 4.0, 1e-9);
    EXPECT_NEAR(functions.get("foo")->evaluate(4.999), 0.0, 2e-3);
}

TEST_F(JsonTest, Annulus) {
    auto j = R"(
    {
      "type": "Annulus",
      "name": "ring",
      "pos": [0, 0, 3],
      "rot": [0, 1, 0],
      "outer_radius": 1.5,
      "inner_radius": 1.0,
      "surface": {},
      "children": []
    }
    )"_json;
    Scenery s;
    Annulus* a = mct::json::add_Annulus(&s.root, &s, j);
    EXPECT_EQ(a->get_name(), "ring");
    EXPECT_EQ(a->get_position_in_mother(), Vec3(0, 0, 3));
    EXPECT_EQ(a->get_rotation_in_mother(), Rot3(0, 1, 0));
    EXPECT_EQ(a->get_children()->size(), 0u);
}

TEST_F(JsonTest, Cylinder_with_rot_and_pos) {
    auto j = R"(
    {
      "type": "Cylinder",
      "name": "cyl",
      "pos": [0, 0, 3],
      "rot": [0, 1, 0],
      "radius": 1.5,
      "length": 1.0,
      "surface": {},
      "children": []
    }
    )"_json;
    Scenery s;
    Cylinder* a = mct::json::add_Cylinder(&s.root, &s, j);
    EXPECT_EQ(a->get_name(), "cyl");
    EXPECT_EQ(a->get_position_in_mother(), Vec3(0, 0, 3));
    EXPECT_EQ(a->get_rotation_in_mother(), Rot3(0, 1, 0));
    EXPECT_EQ(a->get_children()->size(), 0u);
}

TEST_F(JsonTest, Cylinder_with_start_pos_and_end_pos) {
    auto j = R"(
    {
      "type": "Cylinder",
      "name": "cyl",
      "start_pos": [0, 0, 0],
      "end_pos": [0, 0, 1],
      "radius": 1.5,
      "surface": {},
      "children": []
    }
    )"_json;
    Scenery s;
    Cylinder* a = mct::json::add_Cylinder(&s.root, &s, j);
    EXPECT_EQ(a->get_name(), "cyl");
    EXPECT_EQ(a->get_children()->size(), 0u);
}

TEST_F(JsonTest, Triangle) {
    auto j = R"(
    {
      "type": "Triangle",
      "name": "tri",
      "pos": [0, 0, 1.5],
      "rot": [-1.57, 0.0, -1.57],
      "Ax": -1, "Ay": 0, "Bx": 1, "By": 0, "Cx": 0, "Cy": 1,
      "surface": {},
      "children": []
    }
    )"_json;
    Scenery s;
    Triangle* a = mct::json::add_Triangle(&s.root, &s, j);
    EXPECT_EQ(a->get_name(), "tri");
    EXPECT_EQ(a->get_children()->size(), 0u);
}

TEST_F(JsonTest, Disc) {
    auto j = R"(
    {
      "type": "Disc",
      "name": "didi",
      "pos": [0, 0, 1.5],
      "rot": [-1.57, 0.0, -1.57],
      "radius": 1,
      "surface": {},
      "children": []
    }
    )"_json;
    Scenery s;
    Disc* a = mct::json::add_Disc(&s.root, &s, j);
    EXPECT_EQ(a->get_name(), "didi");
    EXPECT_EQ(a->get_children()->size(), 0u);
}

TEST_F(JsonTest, What_is_key) {
    auto j = R"(
    {
      "key": {
        "val1": 1,
        "val2": 2
      }
    }
    )"_json;
    j["key"]["val1"].get<double>();
    EXPECT_EQ(j["key"]["val1"], 1);
}
