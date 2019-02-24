// Copyright 2014 Sebastian A. Mueller
#include "Scenery/geometry/CylinderPrismZ.h"
#include <math.h>
#include <sstream>

namespace relleums {

void CylinderPrismZ::set_radius(const double radius) {
    assert_radius_is_positiv(radius);
    radius_square = radius*radius;
}

bool CylinderPrismZ::is_inside(const Vec3* vec)const {
    return vec->x*vec->x + vec->y*vec->y < radius_square;
}

double CylinderPrismZ::get_radius()const {
    return sqrt(radius_square);
}

void CylinderPrismZ::assert_radius_is_positiv(const double radius)const {
    if (radius <= 0.0) {
        std::stringstream info;
        info << "CylinderPrismZ::" << __func__ << "()\n";
        info << "The radius of a cylinder must be larger than 0.0m !\n";
        info << "Expected: >0.0, but actual: " << radius << "\n";
        throw std::invalid_argument(info.str());
    }
}

double CylinderPrismZ::get_area()const {
    return radius_square*M_PI;
}

}  // namespace relleums