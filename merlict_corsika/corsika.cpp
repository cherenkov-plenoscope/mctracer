// Copyright 2014 Sebastian A. Mueller
#include "merlict_corsika/corsika.h"
#include <fstream>
#include <sstream>
#include <cstring>


namespace corsika {

void write_273_f4_to_path(
    std::array<float, 273> block,
    const std::string &path
) {
    std::vector<std::array<float, 273>> blocks;
    blocks.push_back(block);
    write_273_f4_to_path(blocks, path);
}

void write_273_f4_to_path(
    std::vector<std::array<float, 273>> blocks,
    const std::string &path
) {
    std::ofstream file;
    file.open(path, std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        std::stringstream info;
        info << __FILE__ << " " << __LINE__ << "\n";
        info << "HeaderBlock: Unable to write file: '" << path << "'\n";
        throw std::runtime_error(info.str());
    }

    for (const std::array<float, 273> &block : blocks)
        file.write(
            reinterpret_cast<const char*>(&block),
            sizeof(std::array<float, 273>));

    file.close();
}

std::vector<std::array<float, 273>> read_273_f4_from_path(
    const std::string &path
) {
    std::ifstream file;
    file.open(path, std::ios::binary);
    if (!file.is_open()) {
        std::stringstream info;
        info << __FILE__ << " " << __LINE__ << "\n";
        info << "HeaderBlock: Unable to read file: '" << path << "'\n";
        throw std::runtime_error(info.str());
    }

    std::vector<std::array<float, 273>> blocks;
    while (true) {
        std::array<float, 273> block;
        file.read(
            reinterpret_cast<char*>(&block),
            sizeof(std::array<float, 273>));
        if (!file.eof())
            blocks.push_back(block);
        else
            break;
    }
    file.close();
    return blocks;
}

bool is_equal_273_f4(
    const std::array<float, 273> a,
    const std::array<float, 273> b) {
    for (uint64_t i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}


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
