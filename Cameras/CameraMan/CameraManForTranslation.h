//=================================
// include guard
#ifndef __CAMERAMANFORTRANSLATION_H_INCLUDE__
#define __CAMERAMANFORTRANSLATION_H_INCLUDE__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "CameraManVerbosity.h"
#include "Cameras/CameraDevice.h"

//=================================
class CameraManForTranslation: public CameraManVerbosity {
public:

	CameraManForTranslation(CameraDevice* camera_to_work_with);
	void set_default_position();
	void set_default_position(const Vec3 default_pos);
	void move_for();
	void move_back();
	void move_left();
	void move_right();
	void move_right(const double step_in_m);
	void move_up();
	void move_down();
	void move_to(const Vec3 pos);
private:

	CameraDevice* camera;
	bool verbosity = false;
	Vec3 default_position = Vec3::null;

	void print_camera_moved_in_direction(const std::string dir)const;
	double translation_increment()const;
};
#endif // __CAMERAMANFORTRANSLATION_H_INCLUDE__