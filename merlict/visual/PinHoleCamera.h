// Copyright 2014 Sebastian A. Mueller
#ifndef VISUAL_PINHOLECAMERA_H_
#define VISUAL_PINHOLECAMERA_H_

namespace merlict {class Frame;}

#include <string>
#include "CameraDevice.h"

namespace merlict {
namespace visual {

class PinHoleCamera:public CameraDevice{
 public:
    using CameraDevice::CameraDevice;
    void set_pos_rot_fov(
        const Vec3 cam_pos_in_world,
        const Rot3 cam_rot_in_world,
        const double field_of_view);
    void print()const;
    void acquire_image(
        const Frame* world,
        const Config* visual_config,
        Image* image);
    CameraRay get_ray_for_pixel_in_row_and_col(
        const int row, const int col)const;

 private:
    Vec3 principal_point;
    Vec3 SensorDirectionHori;
    Vec3 SensorDirectionVert;
    double dist_camera_support_to_principal_point;
    Vec3 get_intersection_of_ray_on_image_sensor_for_pixel(
        const int row, const int col)const;
    Vec3 get_direction_of_ray_for_pixel(
        const int row, const int col)const;
    std::string get_pin_hole_cam_print()const;
};

}  // namespace visual
}  // namespace merlict

#endif  // VISUAL_PINHOLECAMERA_H_
