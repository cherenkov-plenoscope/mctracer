// Copyright 2016 Sebastian A. Mueller, Max L. Ahnen
#ifndef VISUAL_SKYDOME_H_
#define VISUAL_SKYDOME_H_

#include <string>
#include "merlict/visual/Image.h"
#include "merlict/Vec3.h"

namespace merlict {
namespace visual {

struct SkyDome {
    bool has_texture;
    Image sky;
    unsigned int central_row;
    unsigned int central_col;
    unsigned int zenith_to_horizon_radius;
    Color background_color;

    SkyDome();
    explicit SkyDome(const Color color);
    explicit SkyDome(const Image sky);
    void set_background_color(const Color background_color);
    Color color_for_direction(const Vec3 &dir)const;
    std::string str()const;

    Color sky_dome_color_for(const Vec3 &dir)const;
    double zenith_distance_of_direction(const Vec3 &dir)const;
    double azimuth_angle_for_direction(const Vec3 &dir)const;
};

}  // namespace visual
}  // namespace merlict

#endif  // VISUAL_SKYDOME_H_
