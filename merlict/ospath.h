// Copyright 2014 Sebastian A. Mueller
#ifndef TOOLS_PATHTOOLS_H_
#define TOOLS_PATHTOOLS_H_

#include <string>

namespace merlict {
namespace ospath {
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

}  // namespace ospath
}  // namespace merlict

#endif  // TOOLS_PATHTOOLS_H_
