// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "merlict/merlict.h"
using std::string;
using namespace merlict;



TEST_CASE("PathToolsTest: empty", "[mctracer]") {
    ospath::Path path;
    CHECK(path.path == "");
    CHECK(path.dirname == "");
    CHECK(path.basename_wo_extension == "");
    CHECK(path.extension == "");
    CHECK(path.basename == "");
}

TEST_CASE("PathToolsTest: complete_path", "[mctracer]") {
    const string full_path = "la/li/lu/blub.xml";
    ospath::Path path(full_path);
    CHECK(path.path == "la/li/lu/blub.xml");
    CHECK(path.dirname == "la/li/lu");
    CHECK(path.basename_wo_extension == "blub");
    CHECK(path.extension == "xml");
    CHECK(path.basename == "blub.xml");
}

TEST_CASE("PathToolsTest: only_filename_and_extension", "[mctracer]") {
    const string full_path = "blub.xml";
    ospath::Path path(full_path);
    CHECK(path.path == "blub.xml");
    CHECK(path.dirname == "");
    CHECK(path.basename_wo_extension == "blub");
    CHECK(path.extension == "xml");
    CHECK(path.basename == "blub.xml");
}

TEST_CASE("PathToolsTest: point_delimiter_filename_and_extension", "[mctracer]") {
    const string full_path = "./blub.xml";
    ospath::Path path(full_path);
    CHECK(path.path == "./blub.xml");
    CHECK(path.dirname == ".");
    CHECK(path.basename_wo_extension == "blub");
    CHECK(path.extension == "xml");
    CHECK(path.basename == "blub.xml");
}

TEST_CASE("PathToolsTest: filename_without_extension", "[mctracer]") {
    const string full_path = "la/li/lu/blub";
    ospath::Path path(full_path);
    CHECK(path.path == "la/li/lu/blub");
    CHECK(path.dirname == "la/li/lu");
    CHECK(path.basename_wo_extension == "blub");
    CHECK(path.extension == "");
    CHECK(path.basename == "blub");
}

TEST_CASE("PathToolsTest: double_dot_path_filename", "[mctracer]") {
    const string full_path = "../la/li/lu/blub";
    ospath::Path path(full_path);
    CHECK(path.path == "../la/li/lu/blub");
    CHECK(path.dirname == "../la/li/lu");
    CHECK(path.basename_wo_extension == "blub");
    CHECK(path.extension == "");
    CHECK(path.basename == "blub");
}

TEST_CASE("PathToolsTest: double_dot_path_filename_extension", "[mctracer]") {
    const string full_path = "../la/li/lu/blub.xml";
    ospath::Path path(full_path);
    CHECK(path.path == "../la/li/lu/blub.xml");
    CHECK(path.dirname == "../la/li/lu");
    CHECK(path.basename_wo_extension == "blub");
    CHECK(path.extension == "xml");
    CHECK(path.basename == "blub.xml");
}

TEST_CASE("PathToolsTest: multiplep_double_dot_path_filename_extension", "[mctracer]") {
    const string full_path = "../la/li/../lu/blub.xml";
    ospath::Path path(full_path);
    CHECK(path.path == "../la/li/../lu/blub.xml");
    CHECK(path.dirname == "../la/li/../lu");
    CHECK(path.basename_wo_extension == "blub");
    CHECK(path.extension == "xml");
    CHECK(path.basename == "blub.xml");
}

TEST_CASE("PathToolsTest: double_dots_in_middle_filename_extension", "[mctracer]") {
    const string full_path = "../la/li/../lu/blub.xml";
    ospath::Path path(full_path);
    CHECK(path.path == "../la/li/../lu/blub.xml");
    CHECK(path.dirname == "../la/li/../lu");
    CHECK(path.basename_wo_extension == "blub");
    CHECK(path.extension == "xml");
    CHECK(path.basename == "blub.xml");
}

TEST_CASE("PathToolsTest: join_no_delimiter", "[mctracer]") {
    string p1 = "/home/hans";
    string p2 = "file.txt";
    CHECK(ospath::join(p1, p2) == "/home/hans/file.txt");
}

TEST_CASE("PathToolsTest: join_delimiter_only_first", "[mctracer]") {
    string p1 = "/home/hans/";
    string p2 = "file.txt";
    CHECK(ospath::join(p1, p2) == "/home/hans/file.txt");
}

TEST_CASE("PathToolsTest: join_delimiter_only_last", "[mctracer]") {
    string p1 = "/home/hans";
    string p2 = "/file.txt";
    CHECK(ospath::join(p1, p2) == "/home/hans/file.txt");
}

TEST_CASE("PathToolsTest: join_delimiter_both", "[mctracer]") {
    string p1 = "/home/hans/";
    string p2 = "/file.txt";
    CHECK(ospath::join(p1, p2) == "/home/hans/file.txt");
}

TEST_CASE("PathToolsTest: join_first_path_empty", "[mctracer]") {
    string p1 = "";
    string p2 = "file.txt";
    CHECK(ospath::join(p1, p2) == "file.txt");
}

TEST_CASE("PathToolsTest: join_second_path_empty", "[mctracer]") {
    string p1 = "hans";
    string p2 = "";
    CHECK(ospath::join(p1, p2) == "hans/");
}
