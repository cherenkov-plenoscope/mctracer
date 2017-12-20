// Copyright 2014 Sebastian A. Mueller
#include "Visual/Image.h"
#include <vector>

namespace Visual {

Image::Image() {}

Image::Image(const unsigned int cols, const unsigned int rows) {
    raw_image = cv::Mat(rows, cols, CV_8UC3);
}

Image::Image(const Image* image_to_copy_from) {
    raw_image = image_to_copy_from->raw_image.clone();
}

Image::Image(const std::string filename_of_image) {
    load(filename_of_image);
}

void Image::save(std::string filename_of_image)const {
    filename_of_image += ".png";
    std::vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(1);
    try {
        cv::imwrite(filename_of_image, raw_image, compression_params);
    }
    catch (std::runtime_error& ex) {
        std::stringstream info;
        info << "Image::" << __func__ << "()\n";
        info << "mctracer trys to save an OpenCV image called: ";
        info << filename_of_image<< "\n";
        info << "Exception converting image to PNG format: " << ex.what();
        info << "\n",
        throw std::runtime_error(info.str());
    }
}

void Image::load(const std::string filename) {
    try {;
        raw_image = cv::imread(filename, CV_LOAD_IMAGE_COLOR);
    } catch (std::runtime_error& ex) {
        std::stringstream info;
        info << "Image::" << __func__ << "()\n";
        info << "mctracer trys to load an OpenCV image called: ";
        info << filename << "\n";
        info << ex.what() << "\n",
        throw std::runtime_error(info.str());
    }

    if (!raw_image.data) {
        std::stringstream info;
        info << "Image::" << __func__ << "()\n";
        info << "mctracer failed to load an OpenCV image called: ";
        info << filename << "\n";
        throw std::runtime_error(info.str());
    }
}

unsigned int Image::get_number_of_pixels()const {
    return get_number_of_cols() * get_number_of_rows();
}

unsigned int Image::get_number_of_cols()const {
    return raw_image.cols;
}

unsigned int Image::get_number_of_rows()const {
    return raw_image.rows;
}

void Image::set_row_col_to_color(
    const unsigned int row,
    const unsigned int col,
    const Color &color
) {
    cv::Vec3b intensity;
    intensity.val[0] = color.b;
    intensity.val[1] = color.g;
    intensity.val[2] = color.r;
    raw_image.at<cv::Vec3b>(row, col) = intensity;
}

Color Image::get_color_row_col(
    const unsigned int row,
    const unsigned int col
)const {
    cv::Vec3b intensity = raw_image.at<cv::Vec3b>(row, col);
    return Color(intensity.val[0], intensity.val[1], intensity.val[2]);
}

double Image::get_width_to_height_ratio()const {
    return static_cast<double>(get_number_of_cols())/
        static_cast<double>(get_number_of_rows());
}

void Image::merge_left_and_right_image_to_anaglyph_3DStereo(
    Image* left_image, Image* right_image
) {
    left_image->convert_to_grayscale();
    right_image->convert_to_grayscale();

    std::vector<cv::Mat> single_channels_left(3);
    cv::split(left_image->raw_image, single_channels_left);

    std::vector<cv::Mat> single_channels_right(3);
    cv::split(right_image->raw_image, single_channels_right);

    std::vector<cv::Mat> anaglyph_image_channels;
    anaglyph_image_channels.push_back(single_channels_right.at(0));  // Blue
    anaglyph_image_channels.push_back(single_channels_right.at(1));  // Green
    anaglyph_image_channels.push_back(single_channels_left.at(2) );  // Red

    cv::merge(anaglyph_image_channels, raw_image);
}

void Image::convert_to_grayscale() {
    cv::cvtColor(raw_image, raw_image, CV_RGB2GRAY);
    cv::cvtColor(raw_image, raw_image, CV_GRAY2RGB);
}

void Image::scale(const unsigned int scale) {
    cv::Size dst_size(raw_image.cols*scale, raw_image.rows*scale);
    cv::resize(raw_image, raw_image, dst_size);
}

}  // namespace Visual
