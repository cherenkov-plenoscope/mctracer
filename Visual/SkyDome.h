// Copyright 2016 Sebastian A. Mueller, Max L. Ahnen
#ifndef VISUAL_SKYDOME_H_
#define VISUAL_SKYDOME_H_

#include <string>
#include "Visual/Image.h"
#include "Core/Vec3.h"

namespace relleums {
namespace visual {

class SkyDome {
    bool has_texture;
    std::string filename;
    Image sky;
    unsigned int central_row;
    unsigned int central_col;
    unsigned int zenith_to_horizon_radius;
    Color background_color;

 public:
    SkyDome();
    explicit SkyDome(const Color color);
    explicit SkyDome(const std::string _filename);
    void set_background_color(const Color background_color);
    Color get_color_for_direction(const Vec3 dir)const;
    std::string str()const;

 private:
    Color sky_dome_color_for(const Vec3 dir)const;
    double get_zenith_distance_of(const Vec3 dir)const;
    double get_azimuth_angle_of(const Vec3 dir)const;
};

}  // namespace visual
}  // namespace relleums

#endif  // VISUAL_SKYDOME_H_
