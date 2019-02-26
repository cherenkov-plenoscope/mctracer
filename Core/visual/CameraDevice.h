// Copyright 2014 Sebastian A. Mueller
#ifndef VISUAL_CAMERADEVICE_H_
#define VISUAL_CAMERADEVICE_H_

#include <iostream>
#include <string>
#include <sstream>
#include "merlict/Rot3.h"
#include "merlict/Vec3.h"
#include "merlict/HomTra3.h"
#include "CameraRay.h"
#include "Image.h"

namespace relleums {
namespace visual {

class CameraDevice{
 public:
    CameraDevice(
        const std::string name,
        const unsigned int number_cols,
        const unsigned int number_rows);
    virtual void acquire_image(
        const Frame* world,
        const Config* visual_config,
        Image* image) = 0;
    virtual void update_position(const Vec3 position);
    virtual void update_orientation(const Rot3 rotation);
    virtual void update_position_and_orientation(
        const Vec3 position,
        const Rot3 rotation);
    void set_pointing_direction(
        Vec3 camera_pointing_direction_in_World,
        Vec3 camera_image_upwards_image_dir_in_world);
    virtual void set_FoV_in_rad(const double field_of_view);
    double get_FoV_in_rad()const;
    Vec3 get_normalized_pointing_get_direction()const;
    Vec3 direction_to_the_right_of_the_camera()const;
    Ray get_optical_axis_in_world()const;
    std::string str()const;
    Vec3 get_position_in_world()const;
    Rot3 get_rotation_in_world()const;
    Vec3 get_image_upwards_direction_in_world_frame()const;
    void assert_resolution(Image* image)const;

    const std::string name;
    const unsigned int number_cols;
    const unsigned int number_rows;

 protected:
    HomTra3 camera2root;
    Vec3 position;
    Rot3 rotation;
    Vec3 pointing;
    CameraRay optical_axis;
    double field_of_view;
    void set_position_and_orientation(
        const Vec3 position,
        const Rot3 rotation);
    std::string get_camera_print()const;
    void assert_field_of_view_is_valid(const double field_of_view)const;
    void update_optical_axis_and_orientation();
};

}  // namespace visual
}  // namespace relleums

#endif  // VISUAL_CAMERADEVICE_H_
