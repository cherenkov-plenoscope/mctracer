// Copyright 2014 Sebastian A. Mueller
#ifndef VISUAL_IMAGE_H_
#define VISUAL_IMAGE_H_

#include <string>
#include <vector>
#include "merlict/Color.h"
#include "PixelCoordinate.h"

namespace merlict {
namespace visual {

struct Image {
    // For fast striding: Use cols in outer loop and rows in inner loop.
    unsigned int number_cols;
    unsigned int number_rows;
    std::vector<Color> raw;
    Image(unsigned int _number_cols, unsigned int _number_rows);
    explicit Image(const std::string path);
    Color at_col_row(unsigned int col, unsigned int row)const;
    void set_col_row(unsigned int col, unsigned int row, Color c);
    unsigned int _idx(unsigned int col, unsigned int row)const;
};

void merge_left_and_right_image_to_anaglyph_3DStereo(
    const Image &left,
    const Image &right,
    Image* stereo_image);

void truncate_to_255(Image* image);

void sobel_operator(const Image &image, Image* out);

void assign_pixel_colors_to_sum_and_exposure_image(
    const std::vector<PixelCoordinate> &pixels,
    const std::vector<Color> &colors,
    Image *sum_image,
    Image* exposure_image);

void luminance_threshold_dilatation(
    const Image &image,
    const float threshold,
    Image* out);

void image_from_sum_and_exposure(
    const Image &sum,
    const Image &exposure,
    Image* out);

std::vector<PixelCoordinate> pixel_coordinates_above_threshold(
    const Image &image,
    const double threshold);

void fabs_difference(const Image &a, const Image &b, Image* out);

void scale_up(const Image &in, const unsigned int scale, Image* out);

}  // namespace visual
}  // namespace relleums

#endif  // VISUAL_IMAGE_H_
