// Copyright 2014 Sebastian A. Mueller
#ifndef VISUAL_CAMERAOPERATOR_ROTATION_H_
#define VISUAL_CAMERAOPERATOR_ROTATION_H_

#include <string>
#include "Verbosity.h"

namespace merlict {
namespace visual {
namespace camera_operator {

class Rotation: public Verbosity {
 public:
    explicit Rotation(CameraDevice* camera_to_work_with);
    void set_default_rotation(const Rot3 rot);
    void look_further_up_when_possible();
    void look_further_down_when_possible();
    void look_left();
    void look_right();
    Rot3 get_R_World2Camera()const;

 private:
    CameraDevice* camera;
    double RotWorld2CameraY_in_rad;
    double RotWorld2CameraZ_in_rad;
    Rot3 R_World2Camera;
    bool is_not_falling_backwards_over_when_looking_up()const;
    void look_further_up();
    void look_straight_up();
    void increment_rot_y();
    void update_R_World2Camera();
    void look_further_down();
    void look_straight_down();
    bool is_not_falling_forward_over_when_looking_down()const;
    void decrease_rot_y();
    void print_when_verbose_camera_rotation_for_y_axis_manipulation()const;
    void increase_rot_z();
    void decrease_rot_z();
    void print_when_verbose_camera_rotation_for_z_axis_manipulation()const;
    void assert_RotWorld2CameraZ_is_not_overtwisting();
    double rotation_increment()const;
    void print_when_verbose_camera_can_not_look_further(
        const std::string dir)const;
};

}  // namespace camera_operator
}  // namespace visual
}  // namespace merlict

#endif  // VISUAL_CAMERAOPERATOR_ROTATION_H_
