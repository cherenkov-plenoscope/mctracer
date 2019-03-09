// Copyright 2014 Sebastian A. Mueller
#include "merlict_corsika_eventio/block.h"
#include <fstream>
#include <sstream>


namespace corsika {
namespace block {

void write(std::array<float, 273> block, const std::string &path) {
    std::vector<std::array<float, 273>> blocks;
    blocks.push_back(block);
    write(blocks, path);
}

void write(
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

std::vector<std::array<float, 273>> read(const std::string &path) {
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

}  // namespace block
}  // namespace corsika
