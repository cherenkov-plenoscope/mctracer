//=================================
// include guard
#ifndef __DualSphericalPrismZ_H_INCLUDE__
#define __DualSphericalPrismZ_H_INCLUDE__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "PrismZ.h"
#include "Core/Vec2.h"
//=================================
class DualSphericalPrismZ :public PrismZ {
private:

	double circle_distance;
	double circle_radius;
	double hight;
	double width;

	Vec2 circ1;
	Vec2 circ2;
public:

	void set_x_hight_and_y_width(
		const double hight,
		const double width
	);
	double get_hight()const;
	double get_width()const;
	double get_enclosing_radius()const;
	double get_circle_radius()const;
	double get_area()const;
	bool is_inside(const Vec3* vec)const;
};
#endif // __DualSphericalPrismZ_H_INCLUDE__
