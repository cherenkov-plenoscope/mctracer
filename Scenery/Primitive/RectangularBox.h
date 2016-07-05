//=================================
// include guard
#ifndef __RectangularBox_H_INCLUDED__
#define __RectangularBox_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Plane.h"

//=================================
class RectangularBox :public SurfaceEntity{
public:

	void set_xyz_width(
		const double x_width,
		const double y_width,
		const double z_width
	);
protected:

};
#endif // __RectangularBox_H_INCLUDED__ 