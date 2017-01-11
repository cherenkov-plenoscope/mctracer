//=================================
// include guard
#ifndef __CAMERAMANFOV_H_INCLUDE__
#define __CAMERAMANFOV_H_INCLUDE__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Tools/Tools.h"
#include "Verbosity.h"


namespace Visual {
namespace CameraMan {

class FieldOfView: public Verbosity {
public:

	FieldOfView(CameraDevice* camera_to_work_with);
	void increase_when_possible();
	void decrease_when_possible();
	void set_default_FoV();
	double get_default_FoV_in_rad()const;
private:

	CameraDevice* camera;
	const double max_FoV_in_rad = Deg2Rad(175.0);
	const double min_FoV_in_rad = Deg2Rad(0.0001);
	const double default_FoV_in_rad = Deg2Rad(65.0);
	void increase();
	void decrease();
	double rate()const;
	bool too_large_when_increased()const;
	bool too_small_when_decreased()const;
	void print_manipulation_when_verbose()const;
	void print_can_not_be_decreased_when_verbose()const;
	void print_can_not_be_increased_when_verbose()const;
	void print_prefix()const;
};

}//CameraMan
}//Visual

#endif // __CAMERAMANFOV_H_INCLUDE__