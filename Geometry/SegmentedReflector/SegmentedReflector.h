//=================================
// include guard
#ifndef __SegmentedReflectorConfig_H_INCLUDED__
#define __SegmentedReflectorConfig_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Tools/Tools.h"
#include "Core/SurfaceEntity.h"
#include "HexGridAnnulus.h"

namespace SegmentedReflector {

	struct Config {

		double focal_length;
		double DaviesCotton_over_parabolic_mixing_factor;
		double max_outer_aperture_radius;
		double min_inner_aperture_radius;
		double facet_inner_hex_radius;
		double gap_between_facets;	

		const Color *mirror_color;
		const Color *inner_mirror_color;
		const Function::Func1D* reflectivity;

		Config();
	};

	class Geometry :public Printable {

		const Config cfg;
		double _davies_cotton_weight;
		double _parabolic_weight;
		double _z_offset_makeing_avg_facet_dist_to_f_point_match_f;
		std::vector<Vector3D> _facet_positions;
		std::vector<Rotation3D> _facet_orientations;
		Vector3D _focal_point;
	public:	

		Geometry(const Config config);
		double focal_length()const;
		double DaviesCotton_over_parabolic_mixing_factor()const;
		double max_outer_aperture_radius()const;
		double min_inner_aperture_radius()const;
		double facet_inner_hex_radius()const;
		double facet_outer_hex_radius()const;
		double gap_between_facets()const;
		double z_pos_given_dist_to_optical_axis(const double dist)const;
		double z_Davies_Cotton_given_dist_to_optical_axis(double dist)const;
		double z_Parabola_given_dist_to_optical_axis(double dist)const;
		double z_offset_makeing_avg_facet_dist_to_f_point_match_f()const;
		Vector3D focal_point()const;
		std::vector<Vector3D> facet_positions()const;
		Rotation3D get_rotation_for_facet_position(const Vector3D facet_position)const;
		double DaviesCotton_weight()const;
		double Parabolic_weight()const;
		double facet_spacing()const;
		double naive_f_over_D()const;
		double effective_f_over_D()const;
		double naive_area()const;
		double effective_area()const;
		double effective_radius()const;
		double facet_area()const;
		double number_of_facets()const;
		double average_image_distances_of_facets()const;
		std::string get_print()const;
	private:

		void init_focal_point();
		void init_facet_xy_positions();
		void init_facet_z_positions();
		void init_facet_orientations();
		void make_average_facet_distance_to_focal_point_match_focal_length();
		void abort_if_too_many_iterations(const uint iteration_conter);
		void move_all_facets_in_z(const double movement);
	};

	class Factory {

		const Config cfg;
		Geometry geometry;
		std::vector<Frame*> facets;
		Frame* reflector;
	public:

		Factory(const Config ncfg);
		Frame* get_reflector();
	private:

		void init_facets();
		void init_reflector();
		std::string get_name_of_facet(const uint i)const;
	};
} // SegmentedReflector

#endif // __SegmentedReflectorConfig_H_INCLUDED__ 
