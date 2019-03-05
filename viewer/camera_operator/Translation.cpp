// Copyright 2014 Sebastian A. Mueller
#include "viewer/camera_operator/Translation.h"

namespace merlict {
namespace visual {
namespace camera_operator {

Translation::Translation(CameraDevice* camera_to_work_with) {
    camera = camera_to_work_with;
}

void Translation::set_default_position() {
    camera->set_position_and_orientation(
        default_position,
        camera->rotation());
}

void Translation::set_default_position(const Vec3 default_pos) {
    default_position = default_pos;
    camera->set_position_and_orientation(
        default_position,
        camera->rotation());
}

void Translation::move_forward() {
    camera->set_position_and_orientation(
        camera->position() +
        camera->optical_axis().direction()*translation_increment(),
        camera->rotation());
    if (verbose) print_camera_moved_in_direction("forward");
}

void Translation::move_back() {
    camera->set_position_and_orientation(
        camera->position() -
        camera->optical_axis().direction()*translation_increment(),
        camera->rotation());
    if (verbose) print_camera_moved_in_direction("back");
}

void Translation::move_left() {
    camera->set_position_and_orientation(
        camera->position() -
        camera->direction_to_the_right()*translation_increment(),
        camera->rotation());
    if (verbose) print_camera_moved_in_direction("left");
}

void Translation::move_right() {
    camera->set_position_and_orientation(
        camera->position() +
        camera->direction_to_the_right()*translation_increment(),
        camera->rotation());
    if (verbose) print_camera_moved_in_direction("right");
}

double Translation::translation_increment()const {
    return 0.5/camera->field_of_view();
}

void Translation::print_camera_moved_in_direction(const std::string dir)const {
    std::cout << camera->name << " move " << dir << ": ";
    std::cout << camera->position().str() << "\n";
}

void Translation::move_right(const double step_in_m) {
    camera->set_position_and_orientation(
        camera->position() +
        camera->direction_to_the_right()*step_in_m,
        camera->rotation());
    if (verbose) print_camera_moved_in_direction("right");
}

void Translation::move_up() {
    camera->set_position_and_orientation(
        camera->position() + VEC3_UNIT_Z*translation_increment(),
        camera->rotation());
    if (verbose) print_camera_moved_in_direction("up");
}

void Translation::move_down() {
    camera->set_position_and_orientation(
        camera->position() - VEC3_UNIT_Z*translation_increment(),
        camera->rotation());
    if (verbose) print_camera_moved_in_direction("up");
}

void Translation::move_to(const Vec3 pos) {
    camera->set_position_and_orientation(pos, camera->rotation());
    if (verbose) {
        std::cout << camera->name << " move to: ";
        std::cout << camera->position().str() << "\n";
    }
}

}  // namespace camera_operator
}  // namespace visual
}  // namespace merlict
