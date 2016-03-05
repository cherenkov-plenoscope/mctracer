//=================================
// include guard
#ifndef __LightFieldTelescopeGeometry_H_INCLUDED__
#define __LightFieldTelescopeGeometry_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "LightFieldTelescope/Config.h"
#include "ThinLensEquation.h"
using std::vector;
using std::string;
using std::stringstream;

namespace LightFieldTelescope {

	class Geometry : public Printable {

		vector<Vector3D> pixel_grid;
		vector<vector<uint>> pixel_neighborhood;

		vector<Vector3D> sub_pixel_flower_template_grid;
		vector<vector<uint>> sub_pixel_neighborhood;

		vector<Vector3D> sub_pixel_flower_grid;
		vector<Vector3D> sub_pixel_grid;

		double sub_pixel_flat2flat;
		double pixel_lens_curv_radius;
		double pixel_lens_mean_refrac;
	public:
		const Config config;
		const SegmentedReflector::Geometry reflector;

		Geometry(const Config ncfg);
		string get_print()const;
		double lightfield_sensor_distance()const;
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
		double pixel_lens_sub_pixel_distance()const;
		double aperture_image_radius_on_sub_pixel_sensor()const;
		uint number_of_pixels()const;
		double outer_sensor_housing_radius()const;
		vector<Vector3D> pixel_positions()const;
		vector<vector<uint>>  pixel_neighbor_relations()const;
		double sub_pixel_per_pixel()const;
		double sub_pixel_outer_radius()const;
		double sub_pixel_inner_radius()const;
		double sub_pixel_spacing()const;
		double sub_pixel_z_orientation()const;
		vector<Vector3D> sub_pixel_positions()const;
		vector<vector<uint>>  sub_pixel_neighbor_relations()const;
		vector<Vector3D> sub_pixel_flower_positions()const;
		uint total_number_of_sub_pixels()const;
		void write_sub_pixel_positions(const string path)const;
		double bin_hight()const;
		double principal_aperture_radius_to_throw_photons_in()const;
		double field_of_view_solid_angle()const;
	private:
		void set_up_pixel_grid();
		void set_up_sub_pixel_grid();
		void set_up_lens_geometry();
		void set_up_flower_grid();
		void set_up_sub_pixel_flower_template_grid();
		string get_image_sensor_print()const;
		string get_pixel_lens_print()const;
		string get_sub_pixel_print()const;
		string get_concentrator_bin_print()const;
	};
}
#endif // __LightFieldTelescopeGeometry_H_INCLUDED__ 
