//=================================
// include guard
#ifndef __CAMERARAY_H_INCLUDED__
#define __CAMERARAY_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Ray.h"
#include "ColourProperties.h"
//------------------------------------------------------------------------------
class CameraRay :public Ray{
protected:
	
	ColourProperties colour;
public:
	CameraRay(){};

	CameraRay(const Vector3D support, const Vector3D direction);

	void disp()const;

	std::string get_string()const;

	ColourProperties trace(
		const CartesianFrame* world,
		int refl_count,
		const CartesianFrame* object_propagated_from,
		const GlobalSettings *settings
	)const;

	friend std::ostream& operator<<(
		std::ostream& os, 
		const CameraRay& camera_ray_to_be_displayed
	);
};
#endif // __CAMERARAY_H_INCLUDED__ 
