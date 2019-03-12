// Copyright 2014 Sebastian A. Mueller
#include "merlict_corsika/corsika.h"
#include <sstream>


namespace corsika {
namespace header {
namespace event {

float event_number(const std::array<float, 273> &eh) {
    return eh.at(1);
}
float particle_id(const std::array<float, 273> &eh) {
    return eh.at(2);
}
float total_energy_in_GeV(const std::array<float, 273> &eh) {
    return eh.at(3);
}
float zenith(const std::array<float, 273> &eh) {
    return eh.at(10);
}
float azimuth(const std::array<float, 273> &eh) {
    return eh.at(11);
}
float first_interaction_height_in_cm(const std::array<float, 273> &eh) {
    return eh.at(7-1);
}

}  // namespace event

namespace run {

float run_number(const std::array<float, 273> &rh) {
    return rh.at(1);
}
float slope_of_energy_spektrum(const std::array<float, 273> &rh) {
    return rh.at(15);
}
float energy_range_start(const std::array<float, 273> &rh) {
    return rh.at(16);
}
float energy_range_end(const std::array<float, 273> &rh) {
    return rh.at(17);
}
float num_observation_levels(const std::array<float, 273> &rh) {
    return rh.at(4);
}
float observation_level_at(
  const std::array<float, 273> &rh,
  const unsigned int i
) {
    return rh.at(5+i);
}

}  // namespace run
}  // namespace header

float str2float(const std::string word) {
    if (word.size() != 4) {
        std::stringstream info;
        info << "corsika::" << __func__ << "()\n";
        info << "Expected word size: 4 chars, but actual: ";
        info << word.size() << " chars.\n";
        info << "word: '" << word << "'.\n";
        throw std::invalid_argument(info.str());
    }

    char word_array[] = {word.at(0), word.at(1), word.at(2), word.at(3)};
    float f;
    memcpy(&f, &word_array, sizeof(f));
    return f;
}

std::string float2str(const float word_in_float) {
    char word_array[4];
    memcpy(&word_array, &word_in_float, sizeof(word_array));
    std::string word;
    for (unsigned int i = 0; i < 4; i++)
        word += word_array[i];
    return word;
}

}  // namespace corsika
