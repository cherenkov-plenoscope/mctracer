// Copyright 2014 Sebastian A. Mueller
#include "Visual/PixelCoordinate.h"
#include <vector>

namespace Visual {

std::vector<PixelCoordinate> pixel_coordinates_above_threshold(
    const Image &image,
    const double threshold
) {
    std::vector<PixelCoordinate> coordinates;
    for (unsigned int row = 1; row < image.get_number_of_rows() - 1; row++) {
        for (
            unsigned int col = 1;
            col < image.get_number_of_cols() - 1;
            col++
        ) {
            double lum = 0.0;
            Color c = image.get_color_row_col(row, col);
            lum = c.r + c.g + c.b;
            if (lum > threshold) {
                PixelCoordinate pc;
                pc.row = row;
                pc.col = col;
                coordinates.push_back(pc);
            }
        }
    }
    return coordinates;
}

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

}  // namespace Visual
