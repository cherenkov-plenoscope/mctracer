// Copyright 2014 Sebastian A. Mueller
#include "gtest/gtest.h"
#include "Tools/PathTools.h"
using std::string;
using namespace relleums;

class PathToolsTest : public ::testing::Test {};

TEST_F(PathToolsTest, empty) {
    path::Path path;
    EXPECT_EQ("", path.path);
    EXPECT_EQ("", path.dirname);
    EXPECT_EQ("", path.basename_wo_extension);
    EXPECT_EQ("", path.extension);
    EXPECT_EQ("", path.basename);
}

TEST_F(PathToolsTest, complete_path) {
    const string full_path = "la/li/lu/blub.xml";
    path::Path path(full_path);
    EXPECT_EQ("la/li/lu/blub.xml", path.path);
    EXPECT_EQ("la/li/lu", path.dirname);
    EXPECT_EQ("blub", path.basename_wo_extension);
    EXPECT_EQ("xml", path.extension);
    EXPECT_EQ("blub.xml", path.basename);
}

TEST_F(PathToolsTest, only_filename_and_extension) {
    const string full_path = "blub.xml";
    path::Path path(full_path);
    EXPECT_EQ("blub.xml", path.path);
    EXPECT_EQ("", path.dirname);
    EXPECT_EQ("blub", path.basename_wo_extension);
    EXPECT_EQ("xml", path.extension);
    EXPECT_EQ("blub.xml", path.basename);
}

TEST_F(PathToolsTest, point_delimiter_filename_and_extension) {
    const string full_path = "./blub.xml";
    path::Path path(full_path);
    EXPECT_EQ("./blub.xml", path.path);
    EXPECT_EQ(".", path.dirname);
    EXPECT_EQ("blub", path.basename_wo_extension);
    EXPECT_EQ("xml", path.extension);
    EXPECT_EQ("blub.xml", path.basename);
}

TEST_F(PathToolsTest, filename_without_extension) {
    const string full_path = "la/li/lu/blub";
    path::Path path(full_path);
    EXPECT_EQ("la/li/lu/blub", path.path);
    EXPECT_EQ("la/li/lu", path.dirname);
    EXPECT_EQ("blub", path.basename_wo_extension);
    EXPECT_EQ("", path.extension);
    EXPECT_EQ("blub", path.basename);
}

TEST_F(PathToolsTest, double_dot_path_filename) {
    const string full_path = "../la/li/lu/blub";
    path::Path path(full_path);
    EXPECT_EQ("../la/li/lu/blub", path.path);
    EXPECT_EQ("../la/li/lu", path.dirname);
    EXPECT_EQ("blub", path.basename_wo_extension);
    EXPECT_EQ("", path.extension);
    EXPECT_EQ("blub", path.basename);
}

TEST_F(PathToolsTest, double_dot_path_filename_extension) {
    const string full_path = "../la/li/lu/blub.xml";
    path::Path path(full_path);
    EXPECT_EQ("../la/li/lu/blub.xml", path.path);
    EXPECT_EQ("../la/li/lu", path.dirname);
    EXPECT_EQ("blub", path.basename_wo_extension);
    EXPECT_EQ("xml", path.extension);
    EXPECT_EQ("blub.xml", path.basename);
}

TEST_F(PathToolsTest, multiplep_double_dot_path_filename_extension) {
    const string full_path = "../la/li/../lu/blub.xml";
    path::Path path(full_path);
    EXPECT_EQ("../la/li/../lu/blub.xml", path.path);
    EXPECT_EQ("../la/li/../lu", path.dirname);
    EXPECT_EQ("blub", path.basename_wo_extension);
    EXPECT_EQ("xml", path.extension);
    EXPECT_EQ("blub.xml", path.basename);
}

TEST_F(PathToolsTest, double_dots_in_middle_filename_extension) {
    const string full_path = "../la/li/../lu/blub.xml";
    path::Path path(full_path);
    EXPECT_EQ("../la/li/../lu/blub.xml", path.path);
    EXPECT_EQ("../la/li/../lu", path.dirname);
    EXPECT_EQ("blub", path.basename_wo_extension);
    EXPECT_EQ("xml", path.extension);
    EXPECT_EQ("blub.xml", path.basename);
}

TEST_F(PathToolsTest, join_no_delimiter) {
    string p1 = "/home/hans";
    string p2 = "file.txt";
    EXPECT_EQ("/home/hans/file.txt", path::join(p1, p2));
}

TEST_F(PathToolsTest, join_delimiter_only_first) {
    string p1 = "/home/hans/";
    string p2 = "file.txt";
    EXPECT_EQ("/home/hans/file.txt", path::join(p1, p2));
}

TEST_F(PathToolsTest, join_delimiter_only_last) {
    string p1 = "/home/hans";
    string p2 = "/file.txt";
    EXPECT_EQ("/home/hans/file.txt", path::join(p1, p2));
}

TEST_F(PathToolsTest, join_delimiter_both) {
    string p1 = "/home/hans/";
    string p2 = "/file.txt";
    EXPECT_EQ("/home/hans/file.txt", path::join(p1, p2));
}

TEST_F(PathToolsTest, join_first_path_empty) {
    string p1 = "";
    string p2 = "file.txt";
    EXPECT_EQ("file.txt", path::join(p1, p2));
}

TEST_F(PathToolsTest, join_second_path_empty) {
    string p1 = "hans";
    string p2 = "";
    EXPECT_EQ("hans/", path::join(p1, p2));
}
