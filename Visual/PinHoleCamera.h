// Copyright 2014 Sebastian A. Mueller
#ifndef VISUAL_PINHOLECAMERA_H_
#define VISUAL_PINHOLECAMERA_H_

class Frame;

#include <string>
#include "CameraDevice.h"

namespace Visual {

class PinHoleCamera:public CameraDevice{
 public:
    using CameraDevice::CameraDevice;
    void update_position_and_orientation(
        const Vec3 cam_pos_in_world,
        const Rot3 cam_rot_in_world);
    void print()const;
    void acquire_image(
        const Frame* world,
        const Config* visual_config);
    void set_FoV_in_rad(const double field_of_view);
    CameraRay get_ray_for_pixel_in_row_and_col(
        const int row, const int col)const;

 private:
    Vec3 principal_point;
    Vec3 SensorDirectionHori;
    Vec3 SensorDirectionVert;
    double FieldOfView_in_Rad;
    double dist_camera_support_to_principal_point;
    void update_principal_point_for_current_FoV();
    Vec3 get_intersection_of_ray_on_image_sensor_for_pixel(
        const int row, const int col)const;
    Vec3 get_direction_of_ray_for_pixel(
        const int row, const int col)const;
    std::string get_pin_hole_cam_print()const;
};

}  // namespace Visual

#endif  // VISUAL_PINHOLECAMERA_H_
