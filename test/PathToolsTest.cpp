#include "Tools/PathTools.h"

class PathToolsTest : public ::testing::Test {};
//------------------------------------------------------------------------------
TEST_F(PathToolsTest, split) {
	
	const std::string path = "/home/hans/kram/blub.xml";	
	
	PathTools::FullPath full_path = PathTools::split_path_and_filename(path);

	EXPECT_EQ("/home/hans/kram/", full_path.path);
	EXPECT_EQ("blub.xml", full_path.filename);
}
//------------------------------------------------------------------------------
TEST_F(PathToolsTest, split_filname_without_extension) {
	
	const std::string path = "/home/hans/kram";	
	
	PathTools::FullPath full_path = PathTools::split_path_and_filename(path);

	EXPECT_EQ("/home/hans/", full_path.path);
	EXPECT_EQ("kram", full_path.filename);
}
//------------------------------------------------------------------------------
TEST_F(PathToolsTest, split_filname_without_a_filename) {
	
	const std::string path = "/home/hans/kram/";	
	
	PathTools::FullPath full_path = PathTools::split_path_and_filename(path);

	EXPECT_EQ("/home/hans/kram/", full_path.path);
	EXPECT_EQ("", full_path.filename);
}
//------------------------------------------------------------------------------
TEST_F(PathToolsTest, split_filname_only_a_filename) {
	
	const std::string path = "blub.xml";	
	
	PathTools::FullPath full_path = PathTools::split_path_and_filename(path);

	EXPECT_EQ("", full_path.path);
	EXPECT_EQ("blub.xml", full_path.filename);
}