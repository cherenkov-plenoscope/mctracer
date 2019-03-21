// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "merlict_corsika/corsika.h"


TEST_CASE("HeaderBlockTest: write_and_read_binary_block", "[merlict]") {
    std::array<float, 273> block;
    for (unsigned int i = 0; i < 273; i++)
        block.at(i) = i*1.01;
    const std::string path = "resources/header_block.bin";
    corsika::write_273_f4_to_path(block, path);
    std::vector<std::array<float, 273>> blocks_in =
        corsika::read_273_f4_from_path(path);
    REQUIRE(1u == blocks_in.size());
    for (unsigned int i = 0; i < 273; i++)
        CHECK(blocks_in.at(0).at(i) == Approx(i*1.01).margin(1e-4));
}

TEST_CASE("HeaderBlockTest: write_and_read_several_binary_blocks", "[merlict]") {
    std::vector<std::array<float, 273>> blocks;
    const unsigned int num_blocks = 11;
    for (unsigned int j = 0; j < num_blocks; j++) {
        std::array<float, 273> block;
        blocks.push_back(block);
        for (unsigned int i = 0; i < 273; i++) {
            blocks.at(j).at(i) = (i+1)*(j+1)*1.01;
        }
    }
    const std::string path = "resources/header_block.bin";
    corsika::write_273_f4_to_path(blocks, path);
    std::vector<std::array<float, 273>> blocks_in =
        corsika::read_273_f4_from_path(path);
    REQUIRE(num_blocks == blocks_in.size());
    for (unsigned int j = 0; j < num_blocks; j++) {
        for (unsigned int i = 0; i < 273; i++) {
             CHECK(blocks_in.at(j).at(i) == Approx((i+1)*(j+1)*1.01).margin(1e-3));
        }
    }
}

TEST_CASE("HeaderBlockTest: read_non_existing_file", "[merlict]") {
    CHECK_THROWS_AS(corsika::read_273_f4_from_path("resources/non_existing_file.bin"), std::runtime_error);
}

TEST_CASE("HeaderBlockTest: write_and_read_empty_file", "[merlict]") {
    std::vector<std::array<float, 273>> blocks;
    const std::string path = "resources/header_block.bin";
    corsika::write_273_f4_to_path(blocks, path);
    std::vector<std::array<float, 273>> blocks_in =
        corsika::read_273_f4_from_path(path);
    CHECK(0u == blocks_in.size());
}
