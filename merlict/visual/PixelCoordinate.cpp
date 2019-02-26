// Copyright 2014 Sebastian A. Mueller
#include "merlict/visual/PixelCoordinate.h"
#include <vector>

namespace merlict {
namespace visual {

std::vector<PixelCoordinate> pixel_coordinates(
    const unsigned int cols,
    const unsigned int rows
) {
    std::vector<PixelCoordinate> coordinates;
    for (unsigned int row = 0; row < rows; row++) {
        for (unsigned int col = 0; col < cols; col++) {
            PixelCoordinate pc;
            pc.row = row;
            pc.col = col;
            coordinates.push_back(pc);
        }
    }
    return coordinates;
}

}  // namespace visual
}  // namespace relleums
