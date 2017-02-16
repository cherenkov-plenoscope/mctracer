#include "CameraRay.h"
#include "Core/RayAndFrame.h"
#include "Core/Intersection.h"

namespace Visual {
//------------------------------------------------------------------------------
CameraRay::CameraRay(const Vec3 support, const Vec3 direction){
	set_support_and_direction(support, direction);
}
//------------------------------------------------------------------------------
std::string CameraRay::get_print()const{
	std::stringstream out; 
	out << Ray::get_print() << ", color: " << color;
	return out.str();
}
//------------------------------------------------------------------------------
}//Visual