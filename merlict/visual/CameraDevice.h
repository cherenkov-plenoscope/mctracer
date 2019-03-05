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

namespace merlict {
namespace visual {

class CameraDevice{
 public:
    const std::string name;
    const unsigned int num_cols;
    const unsigned int num_rows;
 protected:
    HomTra3 __camera2root;
    Vec3 __position;
    Rot3 __rotation;
    CameraRay __optical_axis;
    double __field_of_view;

 public:
    HomTra3 camera2root()const;
    Vec3 position()const;
    Rot3 rotation()const;
    CameraRay optical_axis()const;
    double field_of_view()const;
    CameraDevice(
        const std::string name,
        const unsigned int num_cols,
        const unsigned int num_rows);
    virtual void acquire_image(
        const Frame* world,
        const Config* visual_config,
        Image* image) = 0;
    virtual void set_position_and_orientation(
        const Vec3 position,
        const Rot3 rotation);
    virtual void set_field_of_view(const double field_of_view);
    Vec3 direction_to_the_right()const;
    std::string str()const;
    void assert_resolution(Image* image)const;
    std::string camera_str()const;
    void assert_field_of_view_is_valid(const double field_of_view)const;
    void update_optical_axis_and_orientation();
};

}  // namespace visual
}  // namespace merlict

#endif  // VISUAL_CAMERADEVICE_H_
