//=================================
// include guard
#ifndef __SkyDome_H_INCLUDED__
#define __SkyDome_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Image.h"
#include "Core/Vec3.h"

namespace Visual {

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
    SkyDome(const Color color);
    SkyDome(const std::string _filename);
    void set_background_color(const Color background_color);
    Color get_color_for_direction(const Vec3 dir)const;
    std::string str()const;
private:

    Color sky_dome_color_for(const Vec3 dir)const;
    double get_zenith_distance_of(const Vec3 dir)const;
    double get_azimuth_angle_of(const Vec3 dir)const;
};

}//Visual
#endif // __SkyDome_H_INCLUDED__ 
