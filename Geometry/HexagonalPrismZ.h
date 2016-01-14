//=================================
// include guard
#ifndef __HEXAGONALPRISMZ_H_INCLUDE__
#define __HEXAGONALPRISMZ_H_INCLUDE__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "PrismZ.h"

//=================================
class HexagonalPrismZ :public PrismZ {
private:	
	double outer_radius;

	Vector3D ex; 
	Vector3D ey; 
	Vector3D hexU;
	Vector3D hexV;
	Vector3D hexW;

	double inner_radius;
	double edge2edge;
public:
	void set_outer_radius(const double outer_radius);
	bool is_inside(const Vector3D* vec)const;
	double get_area()const;
	double get_outer_radius()const;
private:
	void assert_outer_radius_positive(const double outer_radius)const;
};
#endif // __HEXAGONALPRISMZ_H_INCLUDE__
