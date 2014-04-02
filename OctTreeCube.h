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
};

#endif // __OCTTREECUBE_H_INCLUDED__
