// Copyright 2015 Sebastian A. Mueller
#ifndef PLENOSCOPE_LIGHTFIELDSENSOR_GEOMETRY_H_
#define PLENOSCOPE_LIGHTFIELDSENSOR_GEOMETRY_H_

#include <array>
#include <vector>
#include <string>
#include "Plenoscope/LightFieldSensor/Config.h"
#include "Core/Vec3.h"

namespace Plenoscope {
namespace LightFieldSensor {

class Geometry {
 public:
    // PIXEL: picture cell
    std::vector<relleums::Vec3> pixel_grid;
    std::vector<std::vector<unsigned int>> pixel_neighborhood;

    // PAXEL: principal aperture cell
    std::vector<relleums::Vec3> paxel_per_pixel_template_grid;
    std::vector<std::vector<unsigned int>> paxel_neighborhood;

    std::vector<relleums::Vec3> paxel_grid_centers_of_pixels;

    // LIXEL: light field cell
    std::vector<relleums::Vec3> lixel_grid;
    double lixel_hex_flat2flat_diameter;

    double pixel_lens_curv_radius;
    double pixel_lens_mean_refrac;

    const Config config;

    explicit Geometry(const Config ncfg);
    std::string str()const;
    double max_outer_sensor_radius()const;
    double max_FoV_radius()const;
    double pixel_spacing()const;
    double pixel_lens_inner_aperture_radius()const;
    double pixel_lens_outer_aperture_radius()const;
    double pixel_lens_focal_length()const;
    double pixel_lens_f_over_D()const;
    double pixel_lens_curvature_radius()const;
    double pixel_lens_mean_refraction()const;
    double pixel_FoV_hex_flat2flat()const;
    double pixel_plane_to_paxel_plane_distance()const;
    double aperture_image_radius_on_paxel_plane()const;
    unsigned int number_of_pixels()const;
    double outer_sensor_housing_radius()const;
    std::vector<relleums::Vec3> pixel_positions()const;
    std::vector<std::vector<unsigned int>>  pixel_neighbor_relations()const;
    double number_of_paxel_per_pixel()const;
    double lixel_outer_radius()const;
    double lixel_inner_radius()const;
    double lixel_spacing()const;
    double lixel_z_orientation()const;
    const std::vector<relleums::Vec3>& lixel_positions()const;
    std::vector<std::vector<unsigned int>>  paxel_neighbor_relations()const;
    std::vector<relleums::Vec3> paxel_grid_center_positions()const;
    unsigned int number_of_lixel()const;
    void write_lixel_positions(const std::string &path)const;
    void write_info_header(const std::string &path)const;
    double bin_hight()const;
    double field_of_view_solid_angle()const;
    double expected_imaging_system_focal_length()const;
    double expected_imaging_system_max_aperture_radius()const;
    std::array<float, 273> get_info_header()const;

 private:
    void set_up_pixel_grid();
    void set_up_pixel_lens_geometry();
    void set_up_paxel_grid_centers_of_pixels();
    void set_up_paxel_per_pixel_template_grid();
    void set_up_lixel_grid();
};

}  // namespace LightFieldSensor
}  // namespace Plenoscope

#endif  // PLENOSCOPE_LIGHTFIELDSENSOR_GEOMETRY_H_
