//=================================
// include guard
#ifndef __RectangularPrismZ_H_INCLUDE__
#define __RectangularPrismZ_H_INCLUDE__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "PrismZ.h"

//=================================
class RectangularPrismZ :public PrismZ {
private:

	double half_x_width;
	double half_y_width;
public:

	void set_x_y_width(
		const double x_width,
		const double y_width
	);
	bool is_inside(const Vec3* vec)const;
	double get_half_x_width()const;
	double get_half_y_width()const;
private:

	void assert_width_is_positive(
		const double width, 
		const std::string dim 
	)const;
	void set_rectangle_x_width(const double x_width);
	void set_rectangle_y_width(const double y_width);
};
#endif // __RectangularPrismZ_H_INCLUDE__
