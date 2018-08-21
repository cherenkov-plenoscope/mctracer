// Copyright 2014 Sebastian A. Mueller
#ifndef VISUAL_PORTABLEPIXMAP_H_
#define VISUAL_PORTABLEPIXMAP_H_

#include <fstream>
#include "Visual/Image.h"

namespace Visual {
namespace ppm {

void append_image_to_file(const Image &img, std::ostream &fout);
Image read_image_from_file(std::istream &fin);

void write_image_to_path(const Image &img, const std::string path);
Image read_image_from_path(const std::string path);

}  // namespace ppm
}  // namespace Visual

#endif  // VISUAL_PORTABLEPIXMAP_H_
