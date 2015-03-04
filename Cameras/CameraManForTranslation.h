//=================================
// include guard
#ifndef __CAMERAMANFORTRANSLATION_H_INCLUDE__
#define __CAMERAMANFORTRANSLATION_H_INCLUDE__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "CameraDevice.h"

//=================================
class CameraManForTranslation {
public:
	CameraManForTranslation(CameraDevice* camera_to_work_with);
	void set_default_position();
	void set_default_position(const Vector3D default_pos);
	void move_forward();
	void move_backward();
	void move_left();
	void move_right();
	void move_right(const double step_in_m);
private:
	CameraDevice* camera;
	bool verbosity = false;
	Vector3D default_position = Vector3D(0.0,0.0,0.0);

	void print_camera_moved_in_direction(const std::string dir)const;
	double translation_increment()const;
};
#endif // __CAMERAMANFORTRANSLATION_H_INCLUDE__