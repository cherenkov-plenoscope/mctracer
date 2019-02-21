// Copyright 2014 Sebastian A. Mueller
#include "gtest/gtest.h"
#include "Corsika/Tools.h"
#include "Core/Random/Random.h"
using std::string;
using namespace relleums;

class Corsika_io : public ::testing::Test {};

TEST_F(Corsika_io, word_size_is_too_large) {
    EXPECT_THROW(
        Corsika::str2float("Hans Peter"),
        std::invalid_argument);
}

TEST_F(Corsika_io, 4char_word_forth_and_back) {
    float float_word = Corsika::str2float("Hans");
    string word = Corsika::float2str(float_word);
    EXPECT_EQ("Hans", word);
}

TEST_F(Corsika_io, four_char_word_forth_and_back) {
    Random::Mt19937 prng(0);
    for (unsigned int i = 0; i < 1e4; i++) {
        float num = static_cast<float>(prng.uniform());
        string word = Corsika::float2str(num);
        float num_back = Corsika::str2float(word);
        EXPECT_EQ(num, num_back);
    }
}
