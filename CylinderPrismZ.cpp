#include "CylinderPrismZ.h"
//------------------------------------------------------------------------------
void CylinderPrismZ::set_radius(const double radius) {
	assert_radius_is_positiv(radius);
	radius_square = radius*radius;
}
//------------------------------------------------------------------------------
bool CylinderPrismZ::is_inside(const Vector3D* vec)const {
	return vec->x()*vec->x() + vec->y()*vec->y() < radius_square;
}
//------------------------------------------------------------------------------
double CylinderPrismZ::get_radius()const {
	return sqrt(radius_square);
}
//------------------------------------------------------------------------------
void CylinderPrismZ::assert_radius_is_positiv(const double radius)const {
	
	if(radius <= 0.0) {
		std::stringstream info;
		info << "CylinderPrismZ::" << __func__ << "()\n";
		info << "The radius of a cylinder must be larger than 0.0m !\n";
		info << "Expected: >0.0, but actual: " << radius << "\n";
		throw TracerException(info.str());
	}	
}
//------------------------------------------------------------------------------
double CylinderPrismZ::get_area()const {
	return radius_square*M_PI;
}