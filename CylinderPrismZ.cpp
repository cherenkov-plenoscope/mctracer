#include "CylinderPrismZ.h"
//------------------------------------------------------------------------------
void CylinderPrismZ::set_radius(const double radius) {
	assert_radius_is_positiv(radius);
	this->radius = radius;
}
//------------------------------------------------------------------------------
bool CylinderPrismZ::is_inside(const Vector3D* vec)const {
	double distance_vec_to_z_axis = hypot(vec->x(), vec->y());
	return distance_vec_to_z_axis < radius;
}
//------------------------------------------------------------------------------
double CylinderPrismZ::get_radius()const {
	return radius;
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
	return radius*radius*M_PI;
}