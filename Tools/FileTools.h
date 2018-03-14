// Copyright 2014 Sebastian A. Mueller
#ifndef MCTRACER_TOOLS_FILETOOLS_H_
#define MCTRACER_TOOLS_FILETOOLS_H_

#include <string>

namespace FileTools {

void write_text_to_file(
    const std::string &text,
    const std::string &path);

}  // namespace FileTools

#endif  // MCTRACER_TOOLS_FILETOOLS_H_
