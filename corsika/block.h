// Copyright 2014 Sebastian A. Mueller
#ifndef TOOLS_HEADERBLOCK_H_
#define TOOLS_HEADERBLOCK_H_

#include <string>
#include <array>
#include <vector>

namespace corsika {
namespace block {

    void write(std::array<float, 273> block, const std::string &path);
    void write(
        std::vector<std::array<float, 273>> blocks,
        const std::string &path);
    std::vector<std::array<float, 273>> read(const std::string &path);

}  // namespace block
}  // namespace corsika

#endif  // TOOLS_HEADERBLOCK_H_
