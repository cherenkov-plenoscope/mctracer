// Copyright 2016 Sebastian A. Mueller, Max L. Ahnen
#include "merlict/visual/SkyDome.h"
#include <math.h>
#include <sstream>

namespace relleums {
namespace visual {

SkyDome::SkyDome(const std::string _filename):
    sky(Image(_filename)) {
    has_texture = true;
    filename = _filename;

    central_row = sky.number_rows/2;
    central_col = sky.number_cols/2;
    zenith_to_horizon_radius =
        central_row < central_col ? central_row : central_col;

    background_color = COLOR_BLACK;
}

SkyDome::SkyDome(const Color color): sky(Image(0, 0)) {
    has_texture = false;
    background_color = color;
}

SkyDome::SkyDome(): sky(Image(0, 0)) {
    has_texture = false;
    background_color = COLOR_SKY_BLUE;
}

void SkyDome::set_background_color(const Color background_color) {
    this->background_color = background_color;
}

Color SkyDome::get_color_for_direction(Vec3 dir)const {
    if (has_texture) {
        if (dir.z < 0.0)
            return background_color;
        else
            return sky_dome_color_for(dir);
    } else {
        return background_color;
    }
}

Color SkyDome::sky_dome_color_for(const Vec3 dir)const {
    const double zd = get_zenith_distance_of(dir);
    const double az = get_azimuth_angle_of(dir);

    const double zd_in_pix = zd*(2.0/M_PI)*zenith_to_horizon_radius;

    const int col_offset = static_cast<int>(zd_in_pix*cos(az));
    const int row_offset =-static_cast<int>(zd_in_pix*sin(az));

    const unsigned int col = central_col + col_offset;
    const unsigned int row = central_row + row_offset;

    return sky.at_col_row(col, row);
}

std::string SkyDome::str()const {
    std::stringstream out;
    out << "SkyDome ";
    out << sky.number_cols << "x" << sky.number_rows;
    if (has_texture) {
        out << ", " << filename << ", ";
        out << "central pixel (" << central_col << "," << central_row << "), ";
        out << "zenith to horizon " << zenith_to_horizon_radius << "pixels ";
    } else {
        out << "no texture, background color " << background_color.str();
    }

    return out.str();
}

double SkyDome::get_zenith_distance_of(const Vec3 dir)const {
    return VEC3_UNIT_Z.angle_in_between(dir);
}

double SkyDome::get_azimuth_angle_of(const Vec3 dir)const {
    return atan2(dir.y, dir.x);
}

}  // namespace visual
}  // namespace relleums
