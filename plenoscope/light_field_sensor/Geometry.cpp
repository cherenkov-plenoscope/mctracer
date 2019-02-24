// Copyright 2015 Sebastian A. Mueller
#include "plenoscope/light_field_sensor/Geometry.h"
#include <math.h>
#include <sstream>
#include "Core/scenery/geometry/HexGridAnnulus.h"
#include "Core/scenery/geometry/GridNeighborhoodTopoligy.h"
#include "Core/scenery/geometry/HexGridFlower.h"
#include "Tools/AsciiIo.h"
#include "Core/mctracer.h"
#include "Tools/FileTools.h"
#include "Core/scenery/geometry/lens_maker.h"
#include "corsika/corsika.h"
using std::vector;
using std::array;
using std::string;
using std::stringstream;
using namespace relleums;

namespace plenoscope {
namespace light_field_sensor {

Geometry::Geometry(const Config ncfg):
    config(ncfg) {
    set_up_paxel_per_pixel_template_grid();
    set_up_pixel_grid();
    set_up_paxel_grid_centers_of_pixels();
    set_up_lixel_grid();
    set_up_pixel_lens_geometry();
}

void Geometry::set_up_pixel_grid() {
    HexGridAnnulus pixgrid(
        max_outer_sensor_radius() - pixel_lens_outer_aperture_radius(),
        pixel_spacing());

    pixel_grid = pixgrid.get_grid();

    GridNeighborhoodTopoligy topology(
        &pixel_grid,
        1.1*pixel_spacing());
    pixel_neighborhood = topology.get_neighbor_relations();
}

void Geometry::set_up_paxel_per_pixel_template_grid() {
    HexGridFlower paxel_template_grid_generator(
        pixel_lens_outer_aperture_radius(),
        config.number_of_paxel_on_pixel_diagonal);

    lixel_hex_flat2flat_diameter =
        paxel_template_grid_generator.get_facet_spacing();
    paxel_per_pixel_template_grid = paxel_template_grid_generator.get_grid();

    GridNeighborhoodTopoligy topology(
        &paxel_per_pixel_template_grid,
        1.1*lixel_hex_flat2flat_diameter);
    paxel_neighborhood = topology.get_neighbor_relations();
}

void Geometry::set_up_paxel_grid_centers_of_pixels() {
    paxel_grid_centers_of_pixels.reserve(pixel_grid.size());
    const double r = 1.0 + pixel_plane_to_paxel_plane_distance()/
        config.expected_imaging_system_focal_length;
    for (Vec3 pixel_pos : pixel_grid)
        paxel_grid_centers_of_pixels.push_back(
            pixel_pos*r);
}

void Geometry::set_up_lixel_grid() {
    lixel_grid.reserve(
        paxel_grid_centers_of_pixels.size()*
        paxel_per_pixel_template_grid.size());
    for (Vec3 flower_pos : paxel_grid_centers_of_pixels)
        for (Vec3 paxel_pos : paxel_per_pixel_template_grid)
            lixel_grid.push_back(flower_pos + paxel_pos);
}

void Geometry::set_up_pixel_lens_geometry() {
    pixel_lens_mean_refrac = function::mean(*config.lens_refraction, 137);
    lens_maker::Config lmcfg;
    lmcfg.focal_length = pixel_lens_focal_length();
    lmcfg.aperture_radius = pixel_lens_outer_aperture_radius();
    lmcfg.refractive_index = pixel_lens_mean_refrac;
    double lens_maker_correction = 1.20;
    pixel_lens_curv_radius = lens_maker_correction*
        lens_maker::get_curvature_radius(lmcfg);
}

double Geometry::max_outer_sensor_radius()const {
    return config.expected_imaging_system_focal_length*tan(max_FoV_radius());
}

double Geometry::max_FoV_radius()const {
    return config.max_FoV_diameter/2.0;
}

unsigned int Geometry::number_of_pixels()const {
    return pixel_grid.size();
}

double Geometry::pixel_spacing()const {
    return pixel_lens_inner_aperture_radius()*2.0;
}

double Geometry::pixel_lens_inner_aperture_radius()const {
    return config.expected_imaging_system_focal_length*tan(
        config.pixel_FoV_hex_flat2flat/2.0);
}

double Geometry::pixel_lens_outer_aperture_radius()const {
    return pixel_lens_inner_aperture_radius()*2.0/sqrt(3.0);
}

double Geometry::aperture_image_radius_on_paxel_plane()const {
    return pixel_lens_inner_aperture_radius() - 0.5*lixel_inner_radius();
}

double Geometry::pixel_lens_focal_length()const {
    const double expected_imaging_system_naive_f_over_D =
        config.expected_imaging_system_focal_length/
            (2.0*config.expected_imaging_system_max_aperture_radius);
    return expected_imaging_system_naive_f_over_D*
        2.0*aperture_image_radius_on_paxel_plane();
}

double Geometry::pixel_plane_to_paxel_plane_distance()const {
    return pixel_lens_focal_length();
}

double Geometry::pixel_lens_f_over_D()const {
    return pixel_lens_focal_length()/(2.0*pixel_lens_outer_aperture_radius());
}

double Geometry::pixel_lens_curvature_radius()const {
    return pixel_lens_curv_radius;
}

double Geometry::pixel_lens_mean_refraction()const {
    return pixel_lens_mean_refrac;
}

double Geometry::pixel_FoV_hex_flat2flat()const {
    return config.pixel_FoV_hex_flat2flat;
}

double Geometry::outer_sensor_housing_radius()const {
    return max_outer_sensor_radius()*config.housing_overhead;
}

std::vector<Vec3> Geometry::pixel_positions()const {
    return pixel_grid;
}

vector<vector<unsigned int>> Geometry::pixel_neighbor_relations()const {
    return pixel_neighborhood;
}

const std::vector<Vec3>& Geometry::lixel_positions()const {
    return lixel_grid;
}

vector<vector<unsigned int>> Geometry::paxel_neighbor_relations()const {
    return paxel_neighborhood;
}

std::vector<Vec3> Geometry::paxel_grid_center_positions()const {
    return paxel_grid_centers_of_pixels;
}

double Geometry::lixel_z_orientation()const {
    return 1.0/6.0*M_PI;
}

double Geometry::number_of_paxel_per_pixel()const {
    return paxel_per_pixel_template_grid.size();
}

double Geometry::lixel_outer_radius()const {
    return lixel_inner_radius()*2.0/sqrt(3.0);
}

double Geometry::lixel_inner_radius()const {
    return 0.5*lixel_spacing();
}

double Geometry::lixel_spacing()const {
    return lixel_hex_flat2flat_diameter;
}

unsigned int Geometry::number_of_lixel()const {
    return lixel_grid.size();
}

double Geometry::bin_hight()const {
    return pixel_plane_to_paxel_plane_distance()*0.25;
}

double Geometry::expected_imaging_system_focal_length()const {
    return config.expected_imaging_system_focal_length;
}

double Geometry::expected_imaging_system_max_aperture_radius()const {
    return config.expected_imaging_system_max_aperture_radius;
}

void Geometry::write_lixel_positions(const string &path)const {
    std::ofstream file;
    file.open(path, std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        std::stringstream info;
        info << __FILE__ << " " << __LINE__ << "\n";
        info << __func__ << "\n";
        info << "Unable to write file: '" << path << "'\n";
        throw std::runtime_error(info.str());
    }

    for (Vec3 pos : lixel_grid) {
        const float x = pos.x;
        const float y = pos.y;
        file.write((char*)&x, sizeof(float));
        file.write((char*)&y, sizeof(float));
    }

    file.close();
}

array<float, 273> Geometry::get_info_header()const {
    array<float, 273> header;
    for (unsigned int i = 0; i < header.size(); i++) header[i]=0.0;

    header[  1-1] = corsika::str2float("PLGH");

    header[ 11-1] = config.sensor_plane2imaging_system.get_rot_x().x;
    header[ 12-1] = config.sensor_plane2imaging_system.get_rot_x().y;
    header[ 13-1] = config.sensor_plane2imaging_system.get_rot_x().z;

    header[ 14-1] = config.sensor_plane2imaging_system.get_rot_y().x;
    header[ 15-1] = config.sensor_plane2imaging_system.get_rot_y().y;
    header[ 16-1] = config.sensor_plane2imaging_system.get_rot_y().z;

    header[ 17-1] = config.sensor_plane2imaging_system.get_rot_z().x;
    header[ 18-1] = config.sensor_plane2imaging_system.get_rot_z().y;
    header[ 19-1] = config.sensor_plane2imaging_system.get_rot_z().z;

    header[ 20-1] = config.sensor_plane2imaging_system.get_translation().x;
    header[ 21-1] = config.sensor_plane2imaging_system.get_translation().y;
    header[ 22-1] = config.sensor_plane2imaging_system.get_translation().z;

    header[ 23-1] = config.expected_imaging_system_focal_length;
    header[ 24-1] = config.expected_imaging_system_max_aperture_radius;
    header[ 25-1] = config.max_FoV_diameter;
    header[ 26-1] = config.pixel_FoV_hex_flat2flat;
    header[ 27-1] = config.number_of_paxel_on_pixel_diagonal;
    header[ 28-1] = config.housing_overhead;

    header[101-1] = number_of_pixels();
    header[102-1] = number_of_paxel_per_pixel();

    header[103-1] = lixel_outer_radius();
    header[104-1] = lixel_spacing();
    header[105-1] = lixel_z_orientation();

    header[106-1] = pixel_FoV_hex_flat2flat();
    header[107-1] = pixel_lens_outer_aperture_radius();
    header[108-1] = pixel_spacing();

    header[109-1] = pixel_lens_f_over_D();
    header[110-1] = pixel_lens_focal_length();
    header[111-1] = pixel_lens_curvature_radius();
    header[112-1] = pixel_plane_to_paxel_plane_distance();
    header[113-1] = aperture_image_radius_on_paxel_plane();
    header[114-1] = pixel_lens_mean_refraction();

    header[115-1] = field_of_view_solid_angle();
    header[116-1] = max_outer_sensor_radius();

    return header;
}

void Geometry::write_info_header(const string &path)const {
    FileTools::write_text_to_file(str(), path);
}

string Geometry::str()const {
    stringstream tab;
    tab << "__Light_Field_Sensor__\n";
    tab << " Field of View................. ";
    tab << rad2deg(config.max_FoV_diameter) << "deg\n";
    tab << " Field of View solid angle..... ";
    tab << field_of_view_solid_angle() << "sr\n";
    tab << " Max sensor radius............. ";
    tab << max_outer_sensor_radius() << "m\n";
    tab << " Number of pixels.............. ";
    tab << number_of_pixels() << "\n";
    tab << " Number of paxels in pixel..... ";
    tab << number_of_paxel_per_pixel() << "\n";
    tab << " Number of lixel............... ";
    tab << lixel_grid.size() << "\n";
    tab << "\n";
    tab << "__Pixel_Lens__\n";
    tab << " FoV hex flat2flat....... ";
    tab << rad2deg(pixel_FoV_hex_flat2flat()) << "deg\n";
    tab << " Spacing................. " << pixel_spacing() << "m\n";
    tab << " Inner aperture radius... ";
    tab << pixel_lens_inner_aperture_radius() << "m\n";
    tab << " Outer aperture radius... ";
    tab << pixel_lens_outer_aperture_radius() << "m\n";
    tab << " Lens f over D........... " << pixel_lens_f_over_D() << "\n";
    tab << " Lens focal length....... " << pixel_lens_focal_length() << "m\n";
    tab << " Lens curvature radius... ";
    tab << pixel_lens_curvature_radius() << "m\n";
    tab << " Lens to paxel plane dist ";
    tab << pixel_plane_to_paxel_plane_distance() << "m\n";
    tab << " Lens refraction mean.... " << pixel_lens_mean_refraction() << "\n";
    tab << " Expected aperture image radius... ";
    tab << aperture_image_radius_on_paxel_plane() << "m\n";
    tab << "\n";
    tab << "__Lixel__\n";
    tab << " Outer radius........ " << lixel_outer_radius() << "m\n";
    tab << " Inner radius........ " << lixel_inner_radius() << "m\n";
    tab << " Spacing............. " << lixel_spacing() << "m\n";
    tab << "\n";
    tab << "__Concentrator_Bin__\n";
    tab << " Bin_hight........... " << bin_hight() << "m\n";
    tab << " Reflectivity........ " << function::mean(*config.bin_reflection, 137) << "\n";
    return tab.str();
}

double Geometry::field_of_view_solid_angle()const {
    return get_solid_angle_for_opening_angle(max_FoV_radius());
}

}  // namespace light_field_sensor
}  // namespace plenoscope