// Copyright 2014 Sebastian A. Mueller
#include "gtest/gtest.h"
#include "corsika/block.h"
using std::string;
using std::vector;
using std::array;

class HeaderBlockTest : public ::testing::Test {};

TEST_F(HeaderBlockTest, write_and_read_binary_block) {
    array<float, 273> block;
    for (unsigned int i = 0; i < 273; i++)
        block.at(i) = i*1.01;
    const string path = "InOut/header_block.bin";
    corsika::block::write(block, path);
    vector<array<float, 273>> blocks_in = corsika::block::read(path);
    ASSERT_EQ(blocks_in.size(), 1u);
    for (unsigned int i = 0; i < 273; i++)
        EXPECT_NEAR(blocks_in.at(0).at(i), i*1.01, 1e-4);
}

TEST_F(HeaderBlockTest, write_and_read_several_binary_blocks) {
    vector<array<float, 273>> blocks;
    const unsigned int number_blocks = 11;
    for (unsigned int j = 0; j < number_blocks; j++) {
        array<float, 273> block;
        blocks.push_back(block);
        for (unsigned int i = 0; i < 273; i++) {
            blocks.at(j).at(i) = (i+1)*(j+1)*1.01;
        }
    }
    const string path = "InOut/header_block.bin";
    corsika::block::write(blocks, path);
    vector<array<float, 273>> blocks_in = corsika::block::read(path);
    ASSERT_EQ(blocks_in.size(), number_blocks);
    for (unsigned int j = 0; j < number_blocks; j++) {
        for (unsigned int i = 0; i < 273; i++) {
             EXPECT_NEAR(blocks_in.at(j).at(i), (i+1)*(j+1)*1.01, 1e-3);
        }
    }
}

TEST_F(HeaderBlockTest, read_non_existing_file) {
    EXPECT_THROW(
        corsika::block::read("InOut/non_existing_file.bin"),
        std::runtime_error);
}

TEST_F(HeaderBlockTest, write_and_read_empty_file) {
    vector<array<float, 273>> blocks;
    const string path = "InOut/header_block.bin";
    corsika::block::write(blocks, path);
    vector<array<float, 273>> blocks_in = corsika::block::read(path);
    EXPECT_EQ(blocks_in.size(), 0u);
}
