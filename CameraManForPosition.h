//=================================
// include guard
#ifndef __CAMERAMANFORPOSITION_H_INCLUDE__
#define __CAMERAMANFORPOSITION_H_INCLUDE__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "CameraDevice.h"
#include "Functions.h"
//=================================
class CameraManForPosition {
public:
	CameraManForPosition(CameraDevice* camera_to_work_with);
	set_default_position();
private:
	CameraDevice* camera;
	bool verbosity = false;
	const Vector3D default_position(0.0,0.0,0.0);
	const Rotatin3D default_orientation(0.0,0.0,0.0);
};
#endif // __CAMERAMANFORPOSITION_H_INCLUDE__