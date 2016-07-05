//=================================
// include guard
#ifndef __Prism3D_H_INCLUDED__
#define __Prism3D_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/SurfaceEntity.h"
#include "Triangle.h"
#include "Plane.h"
//=================================
class Prism3D :public SurfaceEntity{	
public:

	void set_triangle_edge(const double edge_length);
protected:

};
#endif // __Prism3D_H_INCLUDED__ 