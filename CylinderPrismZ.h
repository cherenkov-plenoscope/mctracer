//=================================
// include guard
#ifndef __CYLINDERPRISMZ_H_INCLUDE__
#define __CYLINDERPRISMZ_H_INCLUDE__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "PrismZ.h"

//=================================
class CylinderPrismZ :public PrismZ {
protected:
	double radius; 	
public:
	void set_radius(const double radius);
	bool is_inside(const Vector3D* vec)const;
	double get_radius()const;
	double get_area()const;
private:
	void assert_radius_is_positiv(const double radius)const;
};
#endif // __CYLINDERPRISMZ_H_INCLUDE__
