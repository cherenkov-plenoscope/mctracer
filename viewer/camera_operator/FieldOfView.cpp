// Copyright 2014 Sebastian A. Mueller
#include "viewer/camera_operator/FieldOfView.h"

namespace merlict {
namespace visual {
namespace camera_operator {

FieldOfView::FieldOfView(CameraDevice* _camera) {
    camera = _camera;
    set_default();
}

void FieldOfView::set_default() {
    camera->set_pos_rot_fov(
        camera->position(),
        camera->rotation(),
        default_fov);
}

void FieldOfView::increase_when_possible() {
    if (below_max_when_increased())
        increase();
    else
        print_can_not_be_increased_when_verbose();
}

void FieldOfView::decrease_when_possible() {
    if (above_min_when_decreased())
        decrease();
    else
        print_can_not_be_decreased_when_verbose();
}

bool FieldOfView::below_max_when_increased()const {
    return camera->field_of_view()*rate < max_fov;
}

bool FieldOfView::above_min_when_decreased()const {
    return camera->field_of_view()/rate > min_fov;
}

void FieldOfView::increase() {
    camera->set_pos_rot_fov(
        camera->position(),
        camera->rotation(),
        camera->field_of_view()*rate);
    print_manipulation_when_verbose();
}

void FieldOfView::decrease() {
    camera->set_pos_rot_fov(
        camera->position(),
        camera->rotation(),
        camera->field_of_view()/rate);
    print_manipulation_when_verbose();
}

void FieldOfView::print_manipulation_when_verbose()const {
    if (verbose) {
        print_prefix();
        std::cout << rad2deg(camera->field_of_view()) << " DEG\n";
    }
}

void FieldOfView::print_can_not_be_decreased_when_verbose()const {
    if (verbose) {
        print_prefix();
        std::cout << "Can not decrease FoV any further. Min Fov: ";
        std::cout << rad2deg(min_fov) << " DEG\n";
    }
}

void FieldOfView::print_can_not_be_increased_when_verbose()const {
    if (verbose) {
        print_prefix();
        std::cout << "Can not increase FoV any further. Max Fov: ";
        std::cout << rad2deg(max_fov) << " DEG\n";
    }
}

void FieldOfView::print_prefix()const {
    std::cout << camera->name << " FoV: ";
}

}  // namespace camera_operator
}  // namespace visual
}  // namespace merlict
