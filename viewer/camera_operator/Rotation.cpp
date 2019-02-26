// Copyright 2014 Sebastian A. Mueller
#include <math.h>
#include "viewer/camera_operator/Rotation.h"
#include "merlict/tools.h"
using std::cout;

namespace merlict {
namespace visual {
namespace camera_operator {

Rotation::Rotation(CameraDevice* camera_to_work_with) {
    camera = camera_to_work_with;
}

void Rotation::set_default_rotation(const Rot3 rot) {
    RotWorld2CameraY_in_rad = rot.get_rot_y();
    RotWorld2CameraZ_in_rad = rot.get_rot_z();
    update_R_World2Camera();
}

void Rotation::look_further_up_when_possible() {
    if (is_not_falling_backwards_over_when_looking_up()) {
        look_further_up();
        print_when_verbose_camera_rotation_for_y_axis_manipulation();
    } else {
        look_straight_up();
        print_when_verbose_camera_can_not_look_further("up");
    }
}

bool Rotation::is_not_falling_backwards_over_when_looking_up()const {
    return RotWorld2CameraY_in_rad < -rotation_increment();
}

void Rotation::look_further_up() {
    increment_rot_y();
    update_R_World2Camera();
}

void Rotation::look_straight_up() {
    RotWorld2CameraY_in_rad = 0.0;
    update_R_World2Camera();
}

void Rotation::increment_rot_y() {
    RotWorld2CameraY_in_rad = RotWorld2CameraY_in_rad + rotation_increment();
}

void Rotation::update_R_World2Camera() {
    R_World2Camera.set(0.0, RotWorld2CameraY_in_rad, RotWorld2CameraZ_in_rad);
    camera->update_orientation(R_World2Camera);
}

void Rotation::look_further_down_when_possible() {
    if (is_not_falling_forward_over_when_looking_down()) {
        look_further_down();
        print_when_verbose_camera_rotation_for_y_axis_manipulation();
    } else {
        look_straight_down();
        print_when_verbose_camera_can_not_look_further("down");
    }
}

void Rotation::look_further_down() {
    decrease_rot_y();
    update_R_World2Camera();
}

void Rotation::look_straight_down() {
    RotWorld2CameraY_in_rad = -M_PI;
    update_R_World2Camera();
}

bool Rotation::is_not_falling_forward_over_when_looking_down()const {
    return RotWorld2CameraY_in_rad > -M_PI + rotation_increment();
}

void Rotation::decrease_rot_y() {
    RotWorld2CameraY_in_rad = RotWorld2CameraY_in_rad - rotation_increment();
}

void
Rotation::print_when_verbose_camera_rotation_for_y_axis_manipulation()const {
    if (verbose) {
        cout << camera->name << " rot y: ";
        cout << rad2deg(RotWorld2CameraY_in_rad) << "DEG\n";
    }
}

void Rotation::print_when_verbose_camera_can_not_look_further(
    const std::string dir
)const {
    if (verbose) {
        cout << camera->name << " can not look further " << dir << ", ";
        cout << " rot y: " << rad2deg(RotWorld2CameraY_in_rad) << "DEG\n";
    }
}

void Rotation::look_left() {
    increase_rot_z();
    update_R_World2Camera();
    print_when_verbose_camera_rotation_for_z_axis_manipulation();
}

void Rotation::look_right() {
    decrease_rot_z();
    update_R_World2Camera();
    print_when_verbose_camera_rotation_for_z_axis_manipulation();
}

void Rotation::increase_rot_z() {
    assert_RotWorld2CameraZ_is_not_overtwisting();
    RotWorld2CameraZ_in_rad = RotWorld2CameraZ_in_rad + rotation_increment();
}

void Rotation::decrease_rot_z() {
    assert_RotWorld2CameraZ_is_not_overtwisting();
    RotWorld2CameraZ_in_rad = RotWorld2CameraZ_in_rad - rotation_increment();
}

void
Rotation::print_when_verbose_camera_rotation_for_z_axis_manipulation()const {
    if (verbose) {
        cout << camera->name << " rot z: ";
        cout << rad2deg(RotWorld2CameraZ_in_rad) << "DEG\n";
    }
}

void Rotation::assert_RotWorld2CameraZ_is_not_overtwisting() {
    RotWorld2CameraZ_in_rad = fmod(RotWorld2CameraZ_in_rad, (2.0*M_PI));
}

double Rotation::rotation_increment()const {
    return camera->get_FoV_in_rad()/25.0;
}

Rot3 Rotation::get_R_World2Camera()const {
    return R_World2Camera;
}

}  // namespace camera_operator
}  // namespace visual
}  // namespace merlict
