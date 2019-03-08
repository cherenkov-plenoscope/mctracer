// Copyright 2014 Sebastian A. Mueller
#include "merlict/visual/CameraDevice.h"
#include <exception>
#include "merlict/merlict.h"

namespace merlict {
namespace visual {

CameraDevice::CameraDevice(
    const std::string _name,
    const unsigned int _num_cols,
    const unsigned int _num_rows
):
    name(_name),
    num_cols(_num_cols),
    num_rows(_num_rows) {}

void CameraDevice::set_pos_rot_fov(
    const Vec3 position,
    const Rot3 rotation,
    const double field_of_view
) {
    __position = position;
    __rotation = rotation;
    __camera2root.set_transformation(rotation, position);

    __optical_axis.set_support_and_direction(
        __position,
        __camera2root.orientation(VEC3_UNIT_Z));

    assert_field_of_view_is_valid(field_of_view);
    __field_of_view = field_of_view;
}

HomTra3 CameraDevice::camera2root()const {return __camera2root;}
Vec3 CameraDevice::position()const {return __position;}
Rot3 CameraDevice::rotation()const {return __rotation;}
CameraRay CameraDevice::optical_axis()const {return __optical_axis;}
double CameraDevice::field_of_view()const {return __field_of_view;}

std::string CameraDevice::camera_str()const {
    std::stringstream out;
    out << " _camera:_" << name << "________________\n";
    out << "| T_" << name << "2world:\n";
    out << txt::place_first_infront_of_each_new_line_of_second(
        "| ",
        __camera2root.str());
    out << "| camera position          : ";
    out << __position.str() << "\n";
    out << "| direction of optical axis: ";
    out << __optical_axis.direction().str() << "\n";
    out << "| field of view: " << rad2deg(__field_of_view) <<" deg\n";
    out << "| resolution: cols x rows : ";
    out << num_cols << "x";
    out << num_rows <<" pixels";
    out << " / " << (num_cols*num_rows)/1e6 << " M pixels\n";
    return out.str();
}

std::string CameraDevice::str()const {
    return camera_str();
}

void CameraDevice::assert_field_of_view_is_valid(
    const double field_of_view
)const {
    if (field_of_view <= deg2rad(0.0) || field_of_view > deg2rad(180.0)) {
        std::stringstream info;
        info << "CameraDevice::" << __func__ << "()\n";
        info << "Expected 0.0DEG < FoV < 180.0DEG, but actual FoV: ";
        info << rad2deg(field_of_view) << "DEG\n";
        throw std::invalid_argument(info.str());
    }
}

void CameraDevice::assert_resolution(Image* image)const {
    if (
        image->num_cols != num_cols ||
        image->num_rows != num_rows
    ) {
        std::stringstream info;
        info << "CameraDevice::" << __func__ << "()\n";
        info << "Expected resolution of image to be ";
        info << num_cols << "x";
        info << num_rows << ", but actual it is: ";
        info << image->num_cols << "x";
        info << image->num_rows << ".\n";
        throw std::runtime_error(info.str());
    }
}

Vec3 CameraDevice::direction_to_the_right()const {
    return VEC3_UNIT_Z.cross(__optical_axis.direction());
}

}  // namespace visual
}  // namespace merlict
