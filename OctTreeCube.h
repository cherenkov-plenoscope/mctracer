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
public:  
    Vector3D 	CenterPosition;
    double 		EdgeLength;
    double 		limits[3][2];

    std::vector<OctTreeCube*> 		ChildCubes;
    std::vector<CartesianFrame*> 	ChildFrames;

    //const uint max_number_of_frames_in_OctTree = 7;
    /*
	double EdgeLengthOfChildCube()const;
	Vector3D CalculateCentrePositionOfChildCube(uint x,uint y,uint z)const;
	
	void FillOctTree( 
		const std::vector<CartesianFrame*> ChildrenToFillIn,
		const uint max_number_of_frames_in_OctTree,
		const Vector3D position_of_frame_in_mother_frame,
		const double radius_of_frame
	);

	std::vector<CartesianFrame*> CalculateSubSetOfFramesInCube(
		const std::vector<CartesianFrame*> possible_children
	);

	void set_OctTree(Vector3D CenterPosition, double EdgeLength);

	//void create_OctTree();
	*/
};
#endif // __OCTTREECUBE_H_INCLUDED__
