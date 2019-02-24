//=================================
// include guard
#ifndef __SegmentedReflectorGeometry_H_INCLUDED__
#define __SegmentedReflectorGeometry_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/scenery/SegmentedReflector/Config.h"
#include "Core/Vec3.h"
#include "Core/Rot3.h"

namespace relleums {
namespace SegmentedReflector {

	class Geometry {

		double _z_offset_makeing_avg_facet_dist_to_f_point_match_f;
		std::vector<Vec3> _facet_positions;
		std::vector<Rot3> _facet_orientations;
		Vec3 _focal_point;
	public:	

		const Config cfg;
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
		double thickness_of_dish()const;
		Vec3 focal_point()const;
		std::vector<Vec3> facet_positions()const;
		Rot3 get_rotation_for_facet_position(const Vec3 facet_position)const;
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
		std::string str()const;
		std::string facet_positions_and_normals_to_text()const;
	private:

		void init_focal_point();
		void init_facet_xy_positions();
		void init_facet_z_positions();
		void init_facet_orientations();
		void make_average_facet_distance_to_focal_point_match_focal_length();
		void abort_if_too_many_iterations(const unsigned int iteration_conter);
		void move_all_facets_in_z(const double movement);
	};

}  // SegmentedReflector
}  // namespace relleums

#endif // __SegmentedReflectorGeometry_H_INCLUDED__ 