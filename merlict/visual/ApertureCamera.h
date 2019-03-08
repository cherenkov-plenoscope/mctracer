// Copyright 2014 Sebastian A. Mueller
#ifndef VISUAL_APERTURECAMERA_H_
#define VISUAL_APERTURECAMERA_H_

namespace merlict {class Frame;}

#include <string>
#include <vector>
#include "merlict/DistanceMeter.h"
#include "PixelCoordinate.h"
#include "CameraDevice.h"
#include "merlict/random/random.h"

namespace merlict {
namespace visual {

class ApertureCamera :public CameraDevice{
 public:
    using CameraDevice::CameraDevice;
    void set_pos_rot_fov(
        const Vec3 position,
        const Rot3 rotation,
        const double field_of_view);
    void set_fstop_sensorwidth_focus(
        const double new_f_stop_number,
        const double new_sensor_width,
        const double new_object_distance);
    CameraRay get_ray_for_pixel_in_row_and_col(
        const unsigned int row,
        const unsigned int col,
        random::Mt19937 *prng)const;
    void acquire_image(
        const Frame* world,
        const Config* visual_config,
        Image* image);
    std::string str()const;
    double f_stop_number()const;
    double sensor_width()const;
    double object_distance()const;

 private:
    double _f_stop_number;
    double _aperture_radius;
    double _focal_length;
    double _object_distance;
    double _sensor_distance;
    double _sensor_width;
    double _sensor_height;
    double _pixel_pitch;
    void assert_f_stop_number(const double f_stop_number);
    void assert_sensor_width(const double sensor_width);
    void assert_object_distance(const double object_distance);
    void init_sensor_distance();
    Vec3 random_position_on_aperture_disc(random::Mt19937 *prng)const;
    Vec3 intersection_position_on_object_plane_for_row_col(
        const unsigned int row,
        const unsigned int col,
        random::Mt19937 *prng)const;
    Vec3 ray_support_in_root(const Vec3 &support)const;
    Vec3 ray_direction_in_root(const Vec3 &direction)const;
    double object_size_for_image_size(const double image_size)const;
    std::string get_aperture_camera_print()const;
    std::vector<Color> acquire_pixels(
        const Frame* world,
        const Config* visual_config,
        const std::vector<PixelCoordinate> pixels);
};

}  // namespace visual
}  // namespace merlict

#endif  // VISUAL_APERTURECAMERA_H_
