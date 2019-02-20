// Copyright 2014 Sebastian A. Mueller
#ifndef VISUAL_PIXELCOORDINATE_H_
#define VISUAL_PIXELCOORDINATE_H_

#include <vector>

namespace visual {

struct PixelCoordinate{
    unsigned int row;
    unsigned int col;
};

std::vector<PixelCoordinate> pixel_coordinates(
    const unsigned int cols,
    const unsigned int rows);

}  // namespace visual

#endif  // VISUAL_PIXELCOORDINATE_H_
