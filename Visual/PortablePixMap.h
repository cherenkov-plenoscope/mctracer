// Copyright 2014 Sebastian A. Mueller
#ifndef MCTRACER_VISUAL_PORTABLEPIXMAP_H_
#define MCTRACER_VISUAL_PORTABLEPIXMAP_H_

#include <fstream>
#include "Visual/Image.h"

namespace Visual {

void append_picture_to_file(const Image &img, std::ostream &fout);
Image read_picture_from_file(std::istream &fin);

}  // namespace Visual

#endif  // MCTRACER_VISUAL_PORTABLEPIXMAP_H_
