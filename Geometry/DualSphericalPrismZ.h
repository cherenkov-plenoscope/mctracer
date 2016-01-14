//=================================
// include guard
#ifndef __DualSphericalPrismZ_H_INCLUDE__
#define __DualSphericalPrismZ_H_INCLUDE__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "PrismZ.h"
#include "Core/Vector2D.h"
//=================================
class DualSphericalPrismZ :public PrismZ {
private:

	double circle_distance;
	double circle_radius;
	double hight;
	double width;

	Vector2D circ1;
	Vector2D circ2;
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
	bool is_inside(const Vector3D* vec)const;
};
#endif // __DualSphericalPrismZ_H_INCLUDE__
