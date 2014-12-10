//=================================
// include guard
#ifndef __OCTTREECUBE_H_INCLUDED__
#define __OCTTREECUBE_H_INCLUDED__

//=================================
// forward declared dependencies
class CartesianFrame;
//=================================
// included dependencies
#include <iostream>
#include <vector>
#include "Vector3D.h"
//======================================================================
class OctTreeCube{
	static const uint max_number_of_frames_in_single_OctTreeCube = 7;

    std::vector<OctTreeCube*> 		ChildCubes;
    std::vector<CartesianFrame*> 	ChildFrames;

    Vector3D 	CenterPosition;
    double 		EdgeLength;

	Vector3D position_of_child_for_index_xyz(uint x,uint y,uint z)const;

	const std::vector<CartesianFrame*> CalculateSubSetOfFramesInCube(
	    const std::vector<CartesianFrame*> possible_children
	);
	void set_limits(const Vector3D CenterPosition);

	double half_edge_length()const;

	void add_frame_to_list_when_inside_boundaries(
	    CartesianFrame *frame, 
	    std::vector<CartesianFrame*> &SubSetOfFrames
	)const;

	bool is_intersecting_cube_boundary_volume(
	    CartesianFrame* frame
	)const;

	bool there_are_so_many_frmaes_that_we_need_sub_cubes(
	    const std::vector<CartesianFrame*> &frames
	)const;

	void fill_frames_in_additional_sub_cubes(
	    const std::vector<CartesianFrame*> &frames_to_fill_in
	);

	OctTreeCube* create_sub_cube_at_index_xyz(
    	const uint x, const uint y, const uint z
    )const;
public:	  
    double 		limits[3][2];
	OctTreeCube(const Vector3D CenterPosition, const double LengthOfEdge);
	void fill(const std::vector<CartesianFrame*> &ChildrenToFillIn);
	uint get_max_number_of_frames_in_cube()const;
	bool has_child_cubes()const;
	const std::vector<OctTreeCube*> get_child_cubes()const;
	const std::vector<CartesianFrame*> get_child_frames()const;
	Vector3D get_center_position()const;
	double get_edge_length()const;
	double get_number_of_child_cubes()const;
	double get_number_of_child_frames()const;
};
#endif // __OCTTREECUBE_H_INCLUDED__
