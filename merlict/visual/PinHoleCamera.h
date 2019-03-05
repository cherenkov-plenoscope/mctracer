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
        const Vec3 position,
        const Rot3 rotation,
        const double field_of_view);
    void acquire_image(
        const Frame* world,
        const Config* visual_config,
        Image* image);
    CameraRay get_ray_for_pixel_in_row_and_col(
        const int row,
        const int col)const;

 private:
    Vec3 principal_point;
    Vec3 sensor_col;
    Vec3 sensor_row;
    double distance_to_principal_point;
    Vec3 get_intersection_of_ray_on_image_sensor_for_pixel(
        const int row,
        const int col)const;
    Vec3 get_direction_of_ray_for_pixel(
        const int row,
        const int col)const;
    std::string pin_hole_camera_str()const;
};

}  // namespace visual
}  // namespace merlict

#endif  // VISUAL_PINHOLECAMERA_H_
