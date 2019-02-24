// Copyright 2014 Sebastian A. Mueller
#include "gtest/gtest.h"
#include "corsika/corsika.h"
#include "Core/random/random.h"
using std::string;
using namespace relleums;

class Corsika_io : public ::testing::Test {};

TEST_F(Corsika_io, word_size_is_too_large) {
    EXPECT_THROW(
        corsika::str2float("Hans Peter"),
        std::invalid_argument);
}

TEST_F(Corsika_io, 4char_word_forth_and_back) {
    float float_word = corsika::str2float("Hans");
    string word = corsika::float2str(float_word);
    EXPECT_EQ("Hans", word);
}

TEST_F(Corsika_io, four_char_word_forth_and_back) {
    random::Mt19937 prng(0);
    for (unsigned int i = 0; i < 1e4; i++) {
        float num = static_cast<float>(prng.uniform());
        string word = corsika::float2str(num);
        float num_back = corsika::str2float(word);
        EXPECT_EQ(num, num_back);
    }
}
