// Copyright 2014 Sebastian A. Mueller
#include "Visual/CameraDevice.h"
#include <exception>
#include "Tools/Tools.h"
#include "Tools/StringTools.h"

namespace relleums {
namespace visual {

CameraDevice::CameraDevice(
    const std::string _name,
    const unsigned int _number_cols,
    const unsigned int _number_rows
):
    name(_name),
    number_cols(_number_cols),
    number_rows(_number_rows) {}

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
    pointing =
        camera2root.get_transformed_orientation(VEC3_UNIT_Z);
    optical_axis.set_support_and_direction(position, pointing);
}

void CameraDevice::set_pointing_direction(
    Vec3 camera_pointing_direction_in_World,
    Vec3 camera_image_upwards_image_dir_in_world
) {
    camera_pointing_direction_in_World.normalize();
    // image orientation is defined by x,y.
    // camera y is supposed to point "upwards" in world system
    Vec3 cam_z_axis_in_world = camera_pointing_direction_in_World;
    Vec3 cam_y_axis_in_world = camera_image_upwards_image_dir_in_world;
    Vec3 cam_x_axis_in_world =
        cam_y_axis_in_world.cross(cam_z_axis_in_world);
    camera2root.set_transformation(
        cam_x_axis_in_world,
        cam_y_axis_in_world,
        cam_z_axis_in_world,
        position);
    update_optical_axis_and_orientation();
}

Vec3 CameraDevice::get_image_upwards_direction_in_world_frame()const {
    return camera2root.get_transformed_orientation(VEC3_UNIT_Y);
}

std::string CameraDevice::get_camera_print()const {
    std::stringstream out;
    out << " _camera:_" << name << "________________\n";
    out << "| T_" << name << "2world:\n";
    out << StringTools::place_first_infront_of_each_new_line_of_second(
        "| ",
        camera2root.str());
    out << "| camera position          : ";
    out << position.str() << "\n";
    out << "| direction of optical axis: ";
    out << pointing.str() << "\n";
    out << "| field of view: " << Rad2Deg(field_of_view) <<" deg\n";
    out << "| resolution: cols x rows : ";
    out << number_cols << "x";
    out << number_rows <<" pixels";
    out << " / " << (number_cols*number_rows)/1e6 << " M pixels\n";
    return out.str();
}

Vec3 CameraDevice::get_normalized_pointing_get_direction()const {
    return pointing/pointing.norm();
}

std::string CameraDevice::str()const {
    return get_camera_print();
}

void CameraDevice::set_FoV_in_rad(const double field_of_view) {
    assert_field_of_view_is_valid(field_of_view);
    this -> field_of_view = field_of_view;
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
        info << Rad2Deg(field_of_view) << "DEG\n";
        throw std::invalid_argument(info.str());
    }
}

void CameraDevice::assert_resolution(Image* image)const {
    if (
        image->number_cols != number_cols ||
        image->number_rows != number_rows
    ) {
        std::stringstream info;
        info << "CameraDevice::" << __func__ << "()\n";
        info << "Expected resolution of image to be ";
        info << number_cols << "x";
        info << number_rows << ", but actual it is: ";
        info << image->number_cols << "x";
        info << image->number_rows << ".\n";
        throw std::runtime_error(info.str());
    }
}

Vec3 CameraDevice::get_position_in_world()const {
    return position;
}

Rot3 CameraDevice::get_rotation_in_world()const {
    return rotation;
}

Vec3 CameraDevice::direction_to_the_right_of_the_camera()const {
    return VEC3_UNIT_Z.cross(get_normalized_pointing_get_direction());
}

Ray CameraDevice::get_optical_axis_in_world()const {
    return optical_axis;
}

}  // namespace visual
}  // namespace relleums
