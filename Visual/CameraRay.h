//=================================
// include guard
#ifndef __CAMERARAY_H_INCLUDED__
#define __CAMERARAY_H_INCLUDED__

//=================================
// forward declared dependencies
class Frame;
class Intersection;
//=================================
// included dependencies
#include "Core/Ray.h"
#include "Core/Color.h"
#include "Config.h"

namespace Visual {

class CameraRay :public Ray{
protected:
	
	Color color;
public:
	CameraRay(){};
	CameraRay(const Vec3 support, const Vec3 direction);
	std::string get_print()const;
};

}//Visual

#endif // __CAMERARAY_H_INCLUDED__ 