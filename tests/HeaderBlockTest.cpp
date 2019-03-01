// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "corsika/block.h"


TEST_CASE("HeaderBlockTest: write_and_read_binary_block", "[merlict]") {
    std::array<float, 273> block;
    for (unsigned int i = 0; i < 273; i++)
        block.at(i) = i*1.01;
    const std::string path = "InOut/header_block.bin";
    corsika::block::write(block, path);
    std::vector<std::array<float, 273>> blocks_in = corsika::block::read(path);
    REQUIRE(1u == blocks_in.size());
    for (unsigned int i = 0; i < 273; i++)
        CHECK(blocks_in.at(0).at(i) == Approx(i*1.01).margin(1e-4));
}

TEST_CASE("HeaderBlockTest: write_and_read_several_binary_blocks", "[merlict]") {
    std::vector<std::array<float, 273>> blocks;
    const unsigned int number_blocks = 11;
    for (unsigned int j = 0; j < number_blocks; j++) {
        std::array<float, 273> block;
        blocks.push_back(block);
        for (unsigned int i = 0; i < 273; i++) {
            blocks.at(j).at(i) = (i+1)*(j+1)*1.01;
        }
    }
    const std::string path = "InOut/header_block.bin";
    corsika::block::write(blocks, path);
    std::vector<std::array<float, 273>> blocks_in = corsika::block::read(path);
    REQUIRE(number_blocks == blocks_in.size());
    for (unsigned int j = 0; j < number_blocks; j++) {
        for (unsigned int i = 0; i < 273; i++) {
             CHECK(blocks_in.at(j).at(i) == Approx((i+1)*(j+1)*1.01).margin(1e-3));
        }
    }
}

TEST_CASE("HeaderBlockTest: read_non_existing_file", "[merlict]") {
    CHECK_THROWS_AS(corsika::block::read("InOut/non_existing_file.bin"), std::runtime_error);
}

TEST_CASE("HeaderBlockTest: write_and_read_empty_file", "[merlict]") {
    std::vector<std::array<float, 273>> blocks;
    const std::string path = "InOut/header_block.bin";
    corsika::block::write(blocks, path);
    std::vector<std::array<float, 273>> blocks_in = corsika::block::read(path);
    CHECK(0u == blocks_in.size());
}
