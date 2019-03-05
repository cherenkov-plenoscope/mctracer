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

void CameraDevice::update_position(const Vec3 _position) {
    update_position_and_orientation(_position, rotation);
}

void CameraDevice::update_orientation(const Rot3 _rotation) {
    update_position_and_orientation(position, _rotation);
}

void CameraDevice::update_position_and_orientation(
    const Vec3 _position,
    const Rot3 _rotation
) {
    set_position_and_orientation(_position, _rotation);
    update_optical_axis_and_orientation();
}

void CameraDevice::set_position_and_orientation(
    const Vec3 _position,
    const Rot3 _rotation
) {
    this->position = _position;
    this->rotation = _rotation;
    camera2root.set_transformation(rotation, position);
}

void CameraDevice::update_optical_axis_and_orientation() {
    optical_axis.set_support_and_direction(
        position,
        camera2root.get_transformed_orientation(VEC3_UNIT_Z));
}

std::string CameraDevice::get_camera_print()const {
    std::stringstream out;
    out << " _camera:_" << name << "________________\n";
    out << "| T_" << name << "2world:\n";
    out << txt::place_first_infront_of_each_new_line_of_second(
        "| ",
        camera2root.str());
    out << "| camera position          : ";
    out << position.str() << "\n";
    out << "| direction of optical axis: ";
    out << optical_axis.direction().str() << "\n";
    out << "| field of view: " << rad2deg(field_of_view) <<" deg\n";
    out << "| resolution: cols x rows : ";
    out << num_cols << "x";
    out << num_rows <<" pixels";
    out << " / " << (num_cols*num_rows)/1e6 << " M pixels\n";
    return out.str();
}

std::string CameraDevice::str()const {
    return get_camera_print();
}

void CameraDevice::set_field_of_view(const double field_of_view) {
    assert_field_of_view_is_valid(field_of_view);
    this->field_of_view = field_of_view;
}

double CameraDevice::get_FoV_in_rad()const {
    return field_of_view;
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
    return VEC3_UNIT_Z.cross(optical_axis.direction());
}

}  // namespace visual
}  // namespace merlict
