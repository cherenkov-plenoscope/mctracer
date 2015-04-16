//=================================
// include guard
#ifndef __CAMERARAY_H_INCLUDED__
#define __CAMERARAY_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/Ray.h"
#include "Core/Color.h"
//#include "InteractionSurfaceFinder.h"
//------------------------------------------------------------------------------
class CameraRay :public Ray{
protected:
	
	Color colour;
public:
	CameraRay(){};

	CameraRay(const Vector3D support, const Vector3D direction);

	std::string get_print()const;

	Color trace(
		const Frame* world,
		uint refl_count,
		const TracerSettings *settings
	)const;

	friend std::ostream& operator<<(
		std::ostream& os, 
		const CameraRay& camera_ray_to_be_displayed
	);
private:
	Color absorption_on_the_outer_side(
		const Intersection* intersection,
		const TracerSettings *settings
	)const;

	Color absorption_on_the_inner_side(
		const Intersection* intersection,
		const TracerSettings *settings
	)const;
};
#endif // __CAMERARAY_H_INCLUDED__ 
