// Copyright 2014 Sebastian A. Mueller
#include "viewer/camera_operator/Stereo3D.h"

namespace merlict {
namespace visual {
namespace camera_operator {

Stereo3D::Stereo3D(CameraDevice* camera_to_work_with):
    camera(camera_to_work_with),
    left_image(
        Image(
            camera->num_cols,
            camera->num_rows)),
    right_image(
        Image(
            camera->num_cols,
            camera->num_rows)) {}

void Stereo3D::aquire_stereo_image(
    const Frame* world,
    const Config* visual_config,
    Image* stereo_image
) {
    initial_camera_pos = camera->position();

    left_camera_pos = initial_camera_pos - offset_to_the_right();
    right_camera_pos = initial_camera_pos + offset_to_the_right();

    camera->set_position_and_orientation(left_camera_pos, camera->rotation());
    camera->acquire_image(world, visual_config, &left_image);
    camera->set_position_and_orientation(right_camera_pos, camera->rotation());
    camera->acquire_image(world, visual_config, &right_image);
    camera->set_position_and_orientation(initial_camera_pos, camera->rotation());
    merge_left_and_right_image_to_anaglyph_3DStereo(
        left_image,
        right_image,
        stereo_image);
}

Vec3 Stereo3D::offset_to_the_right()const {
    return camera->direction_to_the_right()*
        stereo_offset_in_m/2.0;
}

void Stereo3D::increase_stereo_offset() {
    stereo_offset_in_m = stereo_offset_in_m * 1.05;
    if (verbose) print_stereo_offset_manipulation("increased to");
}

void Stereo3D::decrease_stereo_offset() {
    stereo_offset_in_m = stereo_offset_in_m / 1.05;
    if (verbose) print_stereo_offset_manipulation("decreased to");
}

void Stereo3D::print_stereo_offset_manipulation(
    const std::string status
)const {
    std::cout << camera->name << " stereo offset " << status << ": ";
    std::cout << stereo_offset_in_m << "m\n";
}

void Stereo3D::use_same_stereo_offset_as(
    const Stereo3D *college
) {
    stereo_offset_in_m = college->stereo_offset_in_m;
}

}  // namespace camera_operator
}  // namespace visual
}  // namespace merlict
