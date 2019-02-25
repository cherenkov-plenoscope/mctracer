// Copyright 2014 Sebastian A. Mueller
#include "catch.hpp"
#include "corsika/corsika.h"
#include "Core/random/random.h"
using std::string;
using namespace relleums;



TEST_CASE("Corsika_io: word_size_is_too_large", "[mctracer]") {
    CHECK_THROWS_AS(corsika::str2float("Hans Peter"), std::invalid_argument);
}

TEST_CASE("Corsika_io: 4char_word_forth_and_back", "[mctracer]") {
    float float_word = corsika::str2float("Hans");
    string word = corsika::float2str(float_word);
    CHECK(word == "Hans");
}

TEST_CASE("Corsika_io: four_char_word_forth_and_back", "[mctracer]") {
    random::Mt19937 prng(0);
    for (unsigned int i = 0; i < 1e4; i++) {
        float num = static_cast<float>(prng.uniform());
        string word = corsika::float2str(num);
        float num_back = corsika::str2float(word);
        CHECK(num_back == num);
    }
}
