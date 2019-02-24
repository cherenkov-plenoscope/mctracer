// Copyright 2014 Sebastian A. Mueller
#include "visual/CameraRay.h"
#include <sstream>
#include "Core/RayAndFrame.h"
#include "Core/Intersection.h"

namespace relleums {
namespace visual {

CameraRay::CameraRay() {}

CameraRay::CameraRay(const Vec3 support, const Vec3 direction) {
    set_support_and_direction(support, direction);
}

std::string CameraRay::str()const {
    std::stringstream out;
    out << Ray::str() << ", color: " << color.str();
    return out.str();
}

}  // namespace visual
}  // namespace relleums
