//=================================
// include guard
#ifndef __Prism3D_H_INCLUDED__
#define __Prism3D_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/SurfaceEntity.h"
#include "Geometry/Triangle.h"
#include "Plane.h"
//=================================
class Prism3D :public SurfaceEntity{

	Triangle tri_1;
	Triangle tri_2;
	Plane plane_a, plane_b, plane_c;		
public:

	void set_triangle_edge(const double edge_length);
protected:

};
#endif // __Prism3D_H_INCLUDED__ 