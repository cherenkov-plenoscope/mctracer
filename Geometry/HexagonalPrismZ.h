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
	static const Vec3 hexU;
	static const Vec3 hexV;
	static const Vec3 hexW;

	double outer_radius;
	double inner_radius;
public:
	void set_outer_radius(const double outer_radius);
	bool is_inside(const Vec3* vec)const;
	double get_area()const;
	double get_outer_radius()const;
private:
	void assert_outer_radius_positive(const double outer_radius)const;
};
#endif // __HEXAGONALPRISMZ_H_INCLUDE__
