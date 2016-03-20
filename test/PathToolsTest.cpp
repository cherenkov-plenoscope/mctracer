#include "gtest/gtest.h"
#include "Tools/PathTools.h"
using std::string;

class PathToolsTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(PathToolsTest, split) {
	
	const string path = "/home/hans/kram/blub.xml";	
	
	PathTools::FullPath full_path = PathTools::split_path_and_filename(path);

	EXPECT_EQ("/home/hans/kram/", full_path.path);
	EXPECT_EQ("blub.xml", full_path.filename);
}
//------------------------------------------------------------------------------
TEST_F(PathToolsTest, split_filname_without_extension) {
	
	const string path = "/home/hans/kram";	
	
	PathTools::FullPath full_path = PathTools::split_path_and_filename(path);

	EXPECT_EQ("/home/hans/", full_path.path);
	EXPECT_EQ("kram", full_path.filename);
}
//------------------------------------------------------------------------------
TEST_F(PathToolsTest, split_filname_without_a_filename) {
	
	const string path = "/home/hans/kram/";	
	
	PathTools::FullPath full_path = PathTools::split_path_and_filename(path);

	EXPECT_EQ("/home/hans/kram/", full_path.path);
	EXPECT_EQ("", full_path.filename);
}















//------------------------------------------------------------------------------
TEST_F(PathToolsTest, split_filname_only_a_filename) {
	
	const string path = "blub.xml";	
	
	PathTools::FullPath full_path = PathTools::split_path_and_filename(path);

	EXPECT_EQ("", full_path.path);
	EXPECT_EQ("blub.xml", full_path.filename);
}
//------------------------------------------------------------------------------
TEST_F(PathToolsTest, complete_path) {
	
	const string full_path = "la/li/lu/blub.xml";	
	
	PathTools::Path path(full_path);
	EXPECT_EQ("la/li/lu/blub.xml", path.full);
	EXPECT_EQ("la/li/lu", path.path);
	EXPECT_EQ("blub", path.filename);
	EXPECT_EQ("xml", path.extension);
}
//------------------------------------------------------------------------------
TEST_F(PathToolsTest, only_filename_and_extension) {
	
	const string full_path = "blub.xml";	
	
	PathTools::Path path(full_path);
	EXPECT_EQ("blub.xml", path.full);
	EXPECT_EQ("", path.path);
	EXPECT_EQ("blub", path.filename);
	EXPECT_EQ("xml", path.extension);
}
//------------------------------------------------------------------------------
TEST_F(PathToolsTest, point_delimiter_filename_and_extension) {
	
	const string full_path = "./blub.xml";	
	
	PathTools::Path path(full_path);
	EXPECT_EQ("./blub.xml", path.full);
	EXPECT_EQ(".", path.path);
	EXPECT_EQ("blub", path.filename);
	EXPECT_EQ("xml", path.extension);
}
//------------------------------------------------------------------------------
TEST_F(PathToolsTest, filename_without_extension) {
	
	const string full_path = "la/li/lu/blub";	
	
	PathTools::Path path(full_path);
	EXPECT_EQ("la/li/lu/blub", path.full);
	EXPECT_EQ("la/li/lu", path.path);
	EXPECT_EQ("blub", path.filename);
	EXPECT_EQ("", path.extension);
}
//------------------------------------------------------------------------------
TEST_F(PathToolsTest, double_dot_path_filename) {
	
	const string full_path = "../la/li/lu/blub";	
	
	PathTools::Path path(full_path);
	EXPECT_EQ("../la/li/lu/blub", path.full);
	EXPECT_EQ("../la/li/lu", path.path);
	EXPECT_EQ("blub", path.filename);
	EXPECT_EQ("", path.extension);
}
//------------------------------------------------------------------------------
TEST_F(PathToolsTest, double_dot_path_filename_extension) {
	
	const string full_path = "../la/li/lu/blub.xml";	
	
	PathTools::Path path(full_path);
	EXPECT_EQ("../la/li/lu/blub.xml", path.full);
	EXPECT_EQ("../la/li/lu", path.path);
	EXPECT_EQ("blub", path.filename);
	EXPECT_EQ("xml", path.extension);
}
//------------------------------------------------------------------------------
TEST_F(PathToolsTest, multiplep_double_dot_path_filename_extension) {
	
	const string full_path = "../la/li/../lu/blub.xml";	
	
	PathTools::Path path(full_path);
	EXPECT_EQ("../la/li/../lu/blub.xml", path.full);
	EXPECT_EQ("../la/li/../lu", path.path);
	EXPECT_EQ("blub", path.filename);
	EXPECT_EQ("xml", path.extension);
}
//------------------------------------------------------------------------------
TEST_F(PathToolsTest, double_dots_in_middle_filename_extension) {
	
	const string full_path = "../la/li/../lu/blub.xml";	
	
	PathTools::Path path(full_path);
	EXPECT_EQ("../la/li/../lu/blub.xml", path.full);
	EXPECT_EQ("../la/li/../lu", path.path);
	EXPECT_EQ("blub", path.filename);
	EXPECT_EQ("xml", path.extension);
}