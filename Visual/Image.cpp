// Copyright 2014 Sebastian A. Mueller
#include "Visual/Image.h"
#include <math.h>
#include <vector>
#include "Visual/PortablePixMap.h"

namespace Visual {

Image::Image(unsigned int _number_cols, unsigned int _number_rows):
    number_cols(_number_cols),
    number_rows(_number_rows),
    raw(std::vector<Color>(number_cols*number_rows)) {}

Image::Image(const std::string path) {
    std::ifstream fin;
    fin.open(path.c_str(), std::ios::in | std::ios::binary);
    *this = read_picture_from_file(fin);
    fin.close();
}

Color Image::at_col_row(unsigned int col, unsigned int row)const {
    return raw.at(_idx(col, row));
}

void Image::set_col_row(unsigned int col, unsigned int row, Color c) {
    raw.at(_idx(col, row)) = c;
}

unsigned int Image::_idx(unsigned int col, unsigned int row)const {
    return col*number_rows + row;
}

Image truncate_to_255(const Image &image) {
    Image rc(image.number_cols, image.number_rows);
    for (unsigned int col = 0; col < image.number_cols; col++) {
        for (unsigned int row = 0; row < image.number_rows; row++) {
            Color c = image.at_col_row(col, row);
            if (c.r > 255.) c.r = 255.;
            if (c.g > 255.) c.g = 255.;
            if (c.b > 255.) c.b = 255.;
            rc.set_col_row(col, row, c);
        }
    }
    return rc;
}

Image sobel_operator(const Image &image) {
    Image rc(image.number_cols, image.number_rows);
    for (unsigned int col = 1; col < image.number_cols - 1; col++) {
        for (unsigned int row = 1; row < image.number_rows - 1; row++) {
            double xr = 0;
            double xg = 0;
            double xb = 0;

            Color z;
            z = image.at_col_row(col - 1, row + 1);
            xr += -1*z.r; xg += -1*z.g; xb += -1*z.b;
            z = image.at_col_row(col - 1, row);
            xr += -2*z.r; xg += -2*z.g; xb += -2*z.b;
            z = image.at_col_row(col - 1, row - 1);
            xr += -1*z.r; xg += -1*z.g; xb += -1*z.b;

            z = image.at_col_row(col + 1, row + 1);
            xr += z.r; xg += z.g; xb += z.b;
            z = image.at_col_row(col + 1, row);
            xr += 2*z.r; xg += 2*z.g; xb += 2*z.b;
            z = image.at_col_row(col + 1, row - 1);
            xr += z.r; xg += z.g; xb += z.b;

            double yr = 0;
            double yg = 0;
            double yb = 0;
            z = image.at_col_row(col + 1, row + 1);
            yr += -1*z.r; yg += -1*z.g; yb += -1*z.b;
            z = image.at_col_row(col    , row + 1);
            yr += -2*z.r; yg += -2*z.g; yb += -2*z.b;
            z = image.at_col_row(col - 1, row + 1);
            yr += -1*z.r; yg += -1*z.g; yb += -1*z.b;

            z = image.at_col_row(col + 1, row - 1);
            yr += z.r; yg += z.g; yb += z.b;
            z = image.at_col_row(col    , row - 1);
            yr += 2*z.r; yg += 2*z.g; yb += 2*z.b;
            z = image.at_col_row(col - 1, row - 1);
            yr += z.r; yg += z.g; yb += z.b;

            double gr = sqrt(xr*xr + yr*yr);
            double gg = sqrt(xg*xg + yg*yg);
            double gb = sqrt(xb*xb + yb*yb);
            Color g(gr, gg, gb);
            rc.set_col_row(col, row, g);
        }
    }
    return rc;
}

void assign_pixel_colors_to_sum_and_exposure_image(
    const std::vector<PixelCoordinate> &pixels,
    const std::vector<Color> &colors,
    Image *sum_image,
    Image* exposure_image
) {
    for (unsigned int pix = 0; pix < pixels.size(); pix++) {
        const unsigned int row = pixels.at(pix).row;
        const unsigned int col = pixels.at(pix).col;
        Color c;
        c.r = sum_image->at_col_row(col, row).r + colors.at(pix).r;
        c.g = sum_image->at_col_row(col, row).g + colors.at(pix).g;
        c.b = sum_image->at_col_row(col, row).b + colors.at(pix).b;
        sum_image->set_col_row(col, row, c);
        Color e;
        e.r = exposure_image->at_col_row(col, row).r + 1.;
        e.g = exposure_image->at_col_row(col, row).g + 1.;
        e.b = exposure_image->at_col_row(col, row).b + 1.;
        exposure_image->set_col_row(col, row, e);
    }
}


Image luminance_threshold_dilatation(
    const Image &image,
    const float threshold
) {
    const Color o(255., 255., 255.);
    const int rows = image.number_rows;
    const int cols = image.number_cols;
    Image rc(cols, rows);
    for (int col = 0; col < cols; col++) {
        for (int row = 0; row < rows; row++) {
            double luminance = 0.0;
            Color c = image.at_col_row(col, row);
            luminance = c.r + c.g + c.b;
            if (luminance > threshold) {
                for (int orow = -1; orow < 2; orow++) {
                    for (int ocol = -1; ocol < 2; ocol++) {
                        if (
                            row + orow >= 0 &&
                            col + ocol >= 0 &&
                            row + orow < rows &&
                            col + ocol < cols) {
                            rc.set_col_row(col + ocol, row + orow, o);
                        }
                    }
                }
            }
        }
    }
    return rc;
}


Image image_from_sum_and_exposure(
    const Image &sum,
    const Image &exposure
) {
    Image image(sum.number_cols, sum.number_rows);
    for (unsigned int col = 0; col < sum.number_cols; col++) {
        for (unsigned int row = 0; row < sum.number_rows; row++) {
            Color c;
            c.r = sum.at_col_row(col, row).r/exposure.at_col_row(col, row).r;
            c.g = sum.at_col_row(col, row).g/exposure.at_col_row(col, row).g;
            c.b = sum.at_col_row(col, row).b/exposure.at_col_row(col, row).b;
            image.set_col_row(col, row, c);
        }
    }
    return image;
}

std::vector<PixelCoordinate> pixel_coordinates_above_threshold(
    const Image &image,
    const double threshold
) {
    std::vector<PixelCoordinate> coordinates;
    for (unsigned int row = 0; row < image.number_rows; row++) {
        for (unsigned int col = 0; col < image.number_cols; col++) {
            double lum = 0.0;
            Color c = image.at_col_row(col, row);
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

Image fabs_image(const Image &a, const Image &b) {
    Image rc(a.number_cols, a.number_rows);
    for (unsigned int row = 0; row < a.number_rows; row++) {
        for (unsigned int col = 0; col < a.number_cols; col++) {
            Color c(
                fabs(a.at_col_row(col, row).r - b.at_col_row(col, row).r),
                fabs(a.at_col_row(col, row).g - b.at_col_row(col, row).g),
                fabs(a.at_col_row(col, row).b - b.at_col_row(col, row).b));
            rc.set_col_row(col, row, c);
        }
    }
    return rc;
}

Image scale_up(const Image &in, const unsigned int scale) {
    Image out(in.number_cols*scale, in.number_rows*scale);
    for (unsigned int row = 0; row < in.number_rows; row++) {
        for (unsigned int col = 0; col < in.number_cols; col++) {
            Color c = in.at_col_row(col, row);
            for (unsigned int srow = 0; srow < scale; srow++) {
                for (unsigned int scol = 0; scol < scale; scol++) {
                    unsigned int ocol = col*scale + scol;
                    unsigned int orow = row*scale + srow;
                    out.set_col_row(ocol, orow, c);
                }
            }
        }
    }
    return out;
}

Image merge_left_and_right_image_to_anaglyph_3DStereo(
    const Image &left,
    const Image &right
) {
    Image out(left.number_cols, left.number_rows);
    for (unsigned int row = 0; row < left.number_rows; row++) {
        for (unsigned int col = 0; col < left.number_cols; col++) {
            const Color cl = left.at_col_row(col, row);
            const double lum_left = (cl.r + cl.g + cl.b)/3;
            const Color cr = right.at_col_row(col, row);
            const double lum_right = (cr.r + cr.g + cr.b)/3;
            const Color ana(lum_left, lum_right, lum_right);
            out.set_col_row(col, row, ana);
        }
    }
    return out;
}

/*
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
*/

}  // namespace Visual
