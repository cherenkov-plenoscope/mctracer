// Copyright 2014 Sebastian A. Mueller
#ifndef VISUAL_IMAGE_H_
#define VISUAL_IMAGE_H_

#include <cv.h>
#include <highgui.h>
#include <string>
#include <opencv2/opencv.hpp>
#include "Core/Color.h"

namespace Visual {

class Image{
 public:
    cv::Mat raw_image;
    Image();
    Image(const unsigned int cols, const unsigned int rows);
    explicit Image(const Image* image_to_copy_from);
    explicit Image(const std::string filename_of_image);
    unsigned int get_number_of_pixels()const;
    unsigned int get_number_of_cols()const;
    unsigned int get_number_of_rows()const;
    void save(std::string filename)const;
    void load(const std::string filename);
    void set_row_col_to_color(
        const unsigned int row, const unsigned int col, const Color &color);
    Color get_color_row_col(
        const unsigned int row,
        const unsigned int col)const;
    double get_width_to_height_ratio()const;
    void merge_left_and_right_image_to_anaglyph_3DStereo(
        Image* left_image, Image* right_image);
    void scale(const unsigned int scale);

 private:
    void convert_to_grayscale();
};

}  // namespace Visual

#endif  // VISUAL_IMAGE_H_
