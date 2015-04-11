//=================================
// include guard
#ifndef __SegmetedReflectorGenerator_H_INCLUDED__
#define __SegmetedReflectorGenerator_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "HexGridXy.h"
#include "Core/Frame.h"
#include <iostream> 
#include <string>
#include <sstream>
#include <vector>
#include "SphereCapWithHexagonalBound.h"
//=================================
class SegmetedReflectorGenerator{

	const Vector3D optical_axis = Vector3D::unit_z;
	
	std::vector<Vector3D> facet_positions;
	std::vector<Rotation3D> facet_orientations;
	std::vector<Frame*> facets;

	Frame* reflector;
	double DaviesCotton_to_parabolic_mixing_factor = 0.0;
	double davies_cotton_weight;
	double parabolic_weight;
	double focal_length = 1.0;
	Vector3D focal_point;
	double facet_spacing = 1.0;
	double max_outer_diameter = 1.0;
	double min_inner_diameter = 0.0;

	double facet_radius;
	double facet_fill_factor = 0.99;

	Color *mirror_colour;
	Color *inner_mirror_colour;
	ReflectionProperties *outer_mirror_reflection;
public:
	void set_hybrid_geometry(const double alpha);
	void set_focal_length(const double _focal_length);
	void set_facet_spacing(const double _facet_spacing);
	void set_max_outer_diameter(const double _max_outer_diameter);
	void set_min_inner_diameter(const double _min_inner_diameter);
	Frame* get_reflector();
	std::string get_print()const;
private:
	void init_facet_z_positions();
	void init_facet_xy_positions();
	double get_z_pos_given_dist_to_optical_axis(
		const double dist_facet_xy_to_optical_axis
	)const;
	void set_image_distance();
	void init_facet_orientations();
	void init_focal_point();
	double get_average_image_distances_of_facets();
	void optimize_reflector_z_pos();
	void init_facet_surface();
	void init_facet_radius();
	void init_facets();
	void init_reflector();
	bool is_center_position(const Vector3D pos);
	void abort_if_too_many_iterations(const uint iteration_conter);
	void move_all_facets_in_z(const double movement);
	std::string get_name_of_facet(const uint i)const;
	Rotation3D get_rotation_of_facet(const uint i)const;
};
#endif // __SegmetedReflectorGenerator_H_INCLUDED__ 
