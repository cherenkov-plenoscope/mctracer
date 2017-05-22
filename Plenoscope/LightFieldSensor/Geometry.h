//=================================
// include guard
#ifndef __PlenoscopeLightFieldSensorGeometry_H_INCLUDED__
#define __PlenoscopeLightFieldSensorGeometry_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Plenoscope/LightFieldSensor/Config.h"
#include "Core/Vec3.h"
#include <array>
using std::vector;
using std::array;
using std::string;
using std::stringstream;

namespace Plenoscope {
namespace LightFieldSensor {

	class Geometry : public Printable {
	public:
		// PIXEL: picture cell
		vector<Vec3> pixel_grid;
		vector<vector<uint>> pixel_neighborhood;

		// PAXEL: principal aperture cell
		vector<Vec3> paxel_per_pixel_template_grid;
		vector<vector<uint>> paxel_neighborhood;

		vector<Vec3> paxel_grid_centers_of_pixels;

		// LIXEL: light field cell
		vector<Vec3> lixel_grid;
		double lixel_hex_flat2flat_diameter;

		double pixel_lens_curv_radius;
		double pixel_lens_mean_refrac;
	
		const Config config;

		Geometry(const Config ncfg);
		string get_print()const;
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
		uint number_of_pixels()const;
		double outer_sensor_housing_radius()const;
		vector<Vec3> pixel_positions()const;
		vector<vector<uint>>  pixel_neighbor_relations()const;
		double number_of_paxel_per_pixel()const;
		double lixel_outer_radius()const;
		double lixel_inner_radius()const;
		double lixel_spacing()const;
		double lixel_z_orientation()const;
		const vector<Vec3>& lixel_positions()const;
		vector<vector<uint>>  paxel_neighbor_relations()const;
		vector<Vec3> paxel_grid_center_positions()const;
		uint number_of_lixel()const;
		void write_lixel_positions(const string &path)const;
		double bin_hight()const;
		double field_of_view_solid_angle()const;
		double expected_imaging_system_focal_length()const;
		double expected_imaging_system_max_aperture_radius()const;
		array<float, 273> get_info_header()const;
	private:

		void set_up_pixel_grid();
		void set_up_pixel_lens_geometry();
		void set_up_paxel_grid_centers_of_pixels();
		void set_up_paxel_per_pixel_template_grid();
		void set_up_lixel_grid();
	};
}//LightFieldSensor
}//Plenoscope
#endif // __PlenoscopeLightFieldSensorGeometry_H_INCLUDED__ 
