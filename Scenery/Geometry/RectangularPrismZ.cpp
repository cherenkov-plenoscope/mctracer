// Copyright 2014 Sebastian A. Mueller
#include "Scenery/Geometry/RectangularPrismZ.h"
#include <math.h>
#include <string>
#include <sstream>

void RectangularPrismZ::set_x_y_width(
    const double x_width,
    const double y_width
) {
    set_rectangle_x_width(x_width);
    set_rectangle_y_width(y_width);
}

void RectangularPrismZ::set_rectangle_x_width(const double x_width) {
    assert_width_is_positive(x_width, "x");
    half_x_width = 0.5*x_width;
}

void RectangularPrismZ::set_rectangle_y_width(const double y_width) {
    assert_width_is_positive(y_width, "y");
    half_y_width = 0.5*y_width;
}

void RectangularPrismZ::assert_width_is_positive(
    const double width,
    const std::string dim
)const {
    if (width <= 0.0) {
        std::stringstream info;
        info << "RectangularPrismZ::" << __func__ << "()\n";
        info << "The " << dim << "_width must not be smaller or equal zero! ";
        info << "Expected: " << dim << "_width > 0, but actual: " << dim;
        info << "_width = " << width << "\n";
        throw std::invalid_argument(info.str());
    }
}

bool RectangularPrismZ::is_inside(const Vec3* vec)const {
    return fabs(vec->x) < half_x_width && fabs(vec->y) <  half_y_width;
}

double RectangularPrismZ::get_half_x_width()const {
    return half_x_width;
}

double RectangularPrismZ::get_half_y_width()const {
    return half_y_width;
}
