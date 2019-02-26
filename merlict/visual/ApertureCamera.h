// Copyright 2014 Sebastian A. Mueller
#ifndef VISUAL_APERTURECAMERA_H_
#define VISUAL_APERTURECAMERA_H_

namespace relleums {class Frame;}

#include <string>
#include <vector>
#include "merlict/DistanceMeter.h"
#include "PixelCoordinate.h"
#include "CameraDevice.h"
#include "merlict/random/random.h"

namespace relleums {
namespace visual {

class ApertureCamera :public CameraDevice{
 public:
    using CameraDevice::CameraDevice;
    void set_focus_to(const double ObjectDistance_in_m);
    void set_FoV_in_rad(const double field_of_view);
    void set_fStop_sesnorWidth(
        const double new_FStopNumber,
        const double new_SensorSizeX);
    CameraRay get_ray_for_pixel_in_row_and_col(
        const unsigned int row,
        const unsigned int col,
        random::Mt19937 *prng)const;
    void acquire_image(
        const Frame* world,
        const Config* visual_config,
        Image* image);
    std::string str()const;

 private:
    double FStopNumber;
    double ApertureRadius_in_m;
    double FocalLength_in_m;
    double ObjectDistance_in_m;
    double SensorDistance_in_m;
    double sensor_width_in_m;
    double sensor_height_in_m;
    double PixelPitch_in_m;
    const double default_object_distance_in_m = 25.0;
    void set_F_stop_number(const double new_FStopNumber);
    void set_sensor_size_using_width(const double sensor_width_in_m);
    void update_sensor_pixel_pitch();
    void update_sensor_distance_given_focal_and_object_distance();
    void set_object_distance(const double ObjectDistance_in_m);
    Vec3 get_random_point_on_bounded_aperture_plane(random::Mt19937 *prng)const;
    Vec3 get_intersec_of_cam_ray_for_pix_row_col_with_obj_plane(
        const unsigned int row,
        const unsigned int col,
        random::Mt19937 *prng)const;
    Vec3 camera_ray_support_vector_in_world_frame(
        const Vec3 &cam_ray_support_in_cam_frame)const;
    Vec3 camera_ray_direction_vector_in_world_frame(
        const Vec3 &cam_ray_direction)const;
    double get_object_size_for_image_size(const double image_size_in_m)const;
    void update_focal_length();
    void update_aperture_radius();
    void set_default_object_distance();
    std::string get_aperture_camera_print()const;
    std::vector<Color> acquire_pixels(
        const Frame* world,
        const Config* visual_config,
        const std::vector<PixelCoordinate> pixels);
};

}  // namespace visual
}  // namespace relleums

#endif  // VISUAL_APERTURECAMERA_H_
