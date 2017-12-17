// Copyright 2014 Sebastian A. Mueller
#ifndef TOOLS_PATHTOOLS_H_
#define TOOLS_PATHTOOLS_H_

#include <string>
#include "StringTools.h"

namespace PathTools {
// Inspired by python os.path.

std::string join(std::string path1, std::string path2);

struct Path {
    std::string path;
    std::string dirname;
    std::string basename;
    std::string basename_wo_extension;
    std::string extension;
    Path(const std::string full_path);
    Path();
};

}  // namespace PathTools

#endif  // TOOLS_PATHTOOLS_H_
