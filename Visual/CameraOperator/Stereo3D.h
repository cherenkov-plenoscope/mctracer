// Copyright 2014 Sebastian A. Mueller
#ifndef VISUAL_CAMERAOPERATOR_STEREO3D_H_
#define VISUAL_CAMERAOPERATOR_STEREO3D_H_

class Frame;

#include <string>
#include "Core/DistanceMeter.h"
#include "Verbosity.h"

namespace Visual {
namespace CameraOperator {

class Stereo3D: public Verbosity {
 public:
    explicit Stereo3D(CameraDevice* camera_to_work_with);
    void aquire_stereo_image(
        const Frame* world,
        const Config* visual_config);
    void increase_stereo_offset();
    void decrease_stereo_offset();
    const Image* get_anaglyph_stereo3D_image();
    void use_same_stereo_offset_as(const Stereo3D *college);

 private:
    CameraDevice* camera;
    Image *left_image;
    Image *right_image;
    Vec3 initial_camera_pos;
    Rot3 initial_camera_rotation;
    Vec3 initial_camera_image_upward_direction;
    Vec3 left_camera_pos;
    Rot3 left_camera_rotation;
    Vec3 left_camera_direction_optical_axis;
    Vec3 right_camera_pos;
    Rot3 right_camera_rotation;
    Vec3 right_camera_direction_optical_axis;

    double distance_to_object;
    Vec3 intersection_point_for_l_and_r_optical_axes;

    double stereo_offset_in_m = 6e-2;
    void calc_pos_and_rot_for_left_and_right_camera_config(
        const Frame* world);
    void remember_initial_camera_config();
    void set_up_camera_back_to_initial_config();
    void remember_initial_camera_position_and_rotation();
    void remmber_initial_camera_image_upward_get_direction();
    void set_positions_for_left_and_right_stereo_config();
    void set_object_distance_to_focus_on(const Frame* world);
    void set_intersec_point_for_left_and_right_optical_axis();
    void set_pointing_dir_for_left_and_right_stereo_config();
    void set_up_camera_in_left_stereo_config();
    void set_up_camera_in_right_stereo_config();
    Vec3 offset_to_the_right()const;
    void set_intersection_for_l_and_r_cameras_optical_axes();
    void print_stereo_offset_manipulation(const std::string status)const;
    void take_left_image(
        const Frame* world,
        const Config* visual_config);
    void take_right_image(
        const Frame* world,
        const Config* visual_config);
};

}  // namespace CameraOperator
}  // namespace Visual

#endif  // VISUAL_CAMERAOPERATOR_STEREO3D_H_
