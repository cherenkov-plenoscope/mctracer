#include "CameraRay.h"
#include "Core/RayAndFrame.h"
#include "Core/Intersection.h"

namespace Visual {
//------------------------------------------------------------------------------
CameraRay::CameraRay(const Vec3 support, const Vec3 direction){
	set_support_and_direction(support, direction);
}
//------------------------------------------------------------------------------
std::string CameraRay::str()const{
	std::stringstream out; 
	out << Ray::str() << ", color: " << color.str();
	return out.str();
}
//------------------------------------------------------------------------------
}//Visual