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
    *this = ppm::read_image_from_path(path);
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

void truncate_to_255(Image* image) {
    for (unsigned int pix = 0; pix < image->raw.size(); pix++) {
        if (image->raw.at(pix).r > 255.) image->raw.at(pix).r = 255.;
        if (image->raw.at(pix).g > 255.) image->raw.at(pix).g = 255.;
        if (image->raw.at(pix).b > 255.) image->raw.at(pix).b = 255.;
    }
}

void sobel_operator(const Image &image, Image* out) {
    for (unsigned int col = 1; col < image.number_cols - 1; col++) {
        for (unsigned int row = 1; row < image.number_rows - 1; row++) {
            const unsigned int idx_cm1_rp1 = image._idx(col - 1, row + 1);
            const unsigned int idx_cm1_rp0 = image._idx(col - 1, row    );
            const unsigned int idx_cm1_rm1 = image._idx(col - 1, row - 1);

            const unsigned int idx_cp1_rp1 = image._idx(col + 1, row + 1);
            const unsigned int idx_cp1_rp0 = image._idx(col + 1, row    );
            const unsigned int idx_cp1_rm1 = image._idx(col + 1, row - 1);

            const unsigned int idx_cp0_rp1 = image._idx(col,     row + 1);
            const unsigned int idx_cp0_rm1 = image._idx(col,     row - 1);

            const unsigned int idx = out->_idx(col, row);

            double xr = 0;
            double xg = 0;
            double xb = 0;

            xr += -1.*image.raw.at(idx_cm1_rp1).r;
            xg += -1.*image.raw.at(idx_cm1_rp1).g;
            xb += -1.*image.raw.at(idx_cm1_rp1).b;

            xr += -2.*image.raw.at(idx_cm1_rp0).r;
            xg += -2.*image.raw.at(idx_cm1_rp0).g;
            xb += -2.*image.raw.at(idx_cm1_rp0).b;

            xr += -1.*image.raw.at(idx_cm1_rm1).r;
            xg += -1.*image.raw.at(idx_cm1_rm1).g;
            xb += -1.*image.raw.at(idx_cm1_rm1).b;

            xr += +1.*image.raw.at(idx_cp1_rp1).r;
            xg += +1.*image.raw.at(idx_cp1_rp1).g;
            xb += +1.*image.raw.at(idx_cp1_rp1).b;

            xr += +2.*image.raw.at(idx_cp1_rp0).r;
            xg += +2.*image.raw.at(idx_cp1_rp0).g;
            xb += +2.*image.raw.at(idx_cp1_rp0).b;

            xr += +1.*image.raw.at(idx_cp1_rm1).r;
            xg += +1.*image.raw.at(idx_cp1_rm1).g;
            xb += +1.*image.raw.at(idx_cp1_rm1).b;

            double yr = 0;
            double yg = 0;
            double yb = 0;

            yr += -1.*image.raw.at(idx_cm1_rp1).r;
            yg += -1.*image.raw.at(idx_cm1_rp1).g;
            yb += -1.*image.raw.at(idx_cm1_rp1).b;

            yr += -2.*image.raw.at(idx_cp0_rp1).r;
            yg += -2.*image.raw.at(idx_cp0_rp1).g;
            yb += -2.*image.raw.at(idx_cp0_rp1).b;

            yr += -1.*image.raw.at(idx_cp1_rp1).r;
            yg += -1.*image.raw.at(idx_cp1_rp1).g;
            yb += -1.*image.raw.at(idx_cp1_rp1).b;

            yr += +1.*image.raw.at(idx_cp1_rm1).r;
            yg += +1.*image.raw.at(idx_cp1_rm1).g;
            yb += +1.*image.raw.at(idx_cp1_rm1).b;

            yr += +2.*image.raw.at(idx_cp0_rm1).r;
            yg += +2.*image.raw.at(idx_cp0_rm1).g;
            yb += +2.*image.raw.at(idx_cp0_rm1).b;

            yr += +1.*image.raw.at(idx_cm1_rm1).r;
            yg += +1.*image.raw.at(idx_cm1_rm1).g;
            yb += +1.*image.raw.at(idx_cm1_rm1).b;

            out->raw.at(idx).r = hypot(xr, yr);
            out->raw.at(idx).g = hypot(xg, yg);
            out->raw.at(idx).b = hypot(xb, yb);
        }
    }
}

void assign_pixel_colors_to_sum_and_exposure_image(
    const std::vector<PixelCoordinate> &pixels,
    const std::vector<Color> &colors,
    Image *sum_image,
    Image* exposure_image
) {
    for (unsigned int pix = 0; pix < pixels.size(); pix++) {
        const unsigned int idx = sum_image->_idx(
            pixels.at(pix).col,
            pixels.at(pix).row);
        sum_image->raw.at(idx).r += colors.at(pix).r;
        sum_image->raw.at(idx).g += colors.at(pix).g;
        sum_image->raw.at(idx).b += colors.at(pix).b;

        exposure_image->raw.at(idx).r += 1.;
        exposure_image->raw.at(idx).g += 1.;
        exposure_image->raw.at(idx).b += 1.;

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

void luminance_threshold_dilatation(
    const Image &image,
    const float threshold,
    Image* out
) {
    const int rows = image.number_rows;
    const int cols = image.number_cols;
    for (int col = 0; col < cols; col++) {
        for (int row = 0; row < rows; row++) {
            const unsigned int idx = image._idx(col, row);
            const float luminance = (
                image.raw.at(idx).r +
                image.raw.at(idx).g +
                image.raw.at(idx).b);
            if (luminance > threshold) {
                for (int orow = -1; orow < 2; orow++) {
                    for (int ocol = -1; ocol < 2; ocol++) {
                        if (
                            row + orow >= 0 &&
                            col + ocol >= 0 &&
                            row + orow < rows &&
                            col + ocol < cols
                        ) {
                            const unsigned int idxo = image._idx(
                                col + ocol,
                                row + orow);
                            out->raw.at(idxo).r = 255.;
                            out->raw.at(idxo).g = 255.;
                            out->raw.at(idxo).b = 255.;
                        }
                    }
                }
            }
        }
    }
}


void image_from_sum_and_exposure(
    const Image &sum,
    const Image &exposure,
    Image* out
) {
    for (unsigned int pix = 0; pix < out->raw.size(); pix++) {
        out->raw.at(pix).r = sum.raw.at(pix).r/exposure.raw.at(pix).r;
        out->raw.at(pix).g = sum.raw.at(pix).g/exposure.raw.at(pix).g;
        out->raw.at(pix).b = sum.raw.at(pix).b/exposure.raw.at(pix).b;
    }
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

void fabs_difference(const Image &a, const Image &b, Image* out) {
    for (unsigned int pix = 0; pix < out->raw.size(); pix++) {
        out->raw.at(pix).r = fabs(a.raw.at(pix).r - b.raw.at(pix).r);
        out->raw.at(pix).g = fabs(a.raw.at(pix).g - b.raw.at(pix).g);
        out->raw.at(pix).b = fabs(a.raw.at(pix).b - b.raw.at(pix).b);
    }
}

void scale_up(const Image &in, const unsigned int scale, Image* out) {
    for (unsigned int row = 0; row < in.number_rows; row++) {
        for (unsigned int col = 0; col < in.number_cols; col++) {
            const unsigned int idx = in._idx(col, row);
            for (unsigned int srow = 0; srow < scale; srow++) {
                for (unsigned int scol = 0; scol < scale; scol++) {
                    const unsigned int ocol = col*scale + scol;
                    const unsigned int orow = row*scale + srow;
                    const unsigned int oidx = out->_idx(ocol, orow);
                    out->raw.at(oidx).r = in.raw.at(idx).r;
                    out->raw.at(oidx).g = in.raw.at(idx).g;
                    out->raw.at(oidx).b = in.raw.at(idx).b;
                }
            }
        }
    }
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

}  // namespace Visual
