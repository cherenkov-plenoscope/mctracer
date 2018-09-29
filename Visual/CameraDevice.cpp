// Copyright 2014 Sebastian A. Mueller
#include "Visual/CameraDevice.h"
#include <exception>
#include "Tools/Tools.h"
#include "Tools/StringTools.h"

namespace Visual {

CameraDevice::CameraDevice(
    const std::string _name,
    const unsigned int sensor_cols,
    const unsigned int sensor_rows
): name(_name), image(sensor_cols, sensor_rows) {}

void CameraDevice::update_position(const Vec3 _pos_in_root) {
    update_position_and_orientation(_pos_in_root, rot_in_root);
}

void CameraDevice::update_orientation(const Rot3 _rot_in_root) {
    update_position_and_orientation(pos_in_root, _rot_in_root);
}

void CameraDevice::update_position_and_orientation(
    const Vec3 _pos_in_root,
    const Rot3 _rot_in_root
) {
    set_position_and_orientation(_pos_in_root, _rot_in_root);
    update_optical_axis_and_orientation();
}

void CameraDevice::set_position_and_orientation(
    const Vec3 _pos_in_root,
    const Rot3 _rot_in_root
) {
    this->pos_in_root = _pos_in_root;
    this->rot_in_root = _rot_in_root;
    camera2root.set_transformation(rot_in_root, pos_in_root);
}

void CameraDevice::update_optical_axis_and_orientation() {
    pointing =
        camera2root.get_transformed_orientation(Vec3::UNIT_Z);
    optical_axis.set_support_and_direction(pos_in_root, pointing);
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
        pos_in_root);
    update_optical_axis_and_orientation();
}

Vec3 CameraDevice::get_image_upwards_direction_in_world_frame()const {
    return camera2root.get_transformed_orientation(Vec3::UNIT_Y);
}

std::string CameraDevice::get_camera_print()const {
    std::stringstream out;
    out << " _camera:_" << name << "________________\n";
    out << "| T_" << name << "2world:\n";
    out << StringTools::place_first_infront_of_each_new_line_of_second(
        "| ",
        camera2root.str());
    out << "| camera position          : ";
    out << pos_in_root.str() << "\n";
    out << "| direction of optical axis: ";
    out << pointing.str() << "\n";
    out << "| field of view: " << Rad2Deg(field_of_view) <<" deg\n";
    out << "| resolution: cols x rows : ";
    out << image.number_cols << "x";
    out << image.number_rows <<" pixels";
    out << " / " << (image.number_cols*image.number_rows)/1e6 << " M pixels\n";
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
    if (field_of_view <= Deg2Rad(0.0) || field_of_view > Deg2Rad(180.0)) {
        std::stringstream info;
        info << "CameraDevice::" << __func__ << "()\n";
        info << "Expected 0.0DEG < FoV < 180.0DEG, but actual FoV: ";
        info << Rad2Deg(field_of_view) << "DEG\n";
        throw std::invalid_argument(info.str());
    }
}

std::string CameraDevice::get_name()const {
    return name;
}

const Image* CameraDevice::get_image()const {
    return &image;
}

Vec3 CameraDevice::get_position_in_world()const {
    return pos_in_root;
}

Rot3 CameraDevice::get_rotation_in_world()const {
    return rot_in_root;
}

Vec3 CameraDevice::direction_to_the_right_of_the_camera()const {
    return Vec3::UNIT_Z.cross(get_normalized_pointing_get_direction());
}

Ray CameraDevice::get_optical_axis_in_world()const {
    return optical_axis;
}

unsigned int CameraDevice::get_number_of_sensor_cols()const {
    return image.number_cols;
}

unsigned int CameraDevice::get_number_of_sensor_rows()const {
    return image.number_rows;
}

}  // namespace Visual
