//=================================
// include guard
#ifndef __FREEORBITCAMERA_H_INCLUDED__
#define __FREEORBITCAMERA_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "ApertureCamera.h"
#include "PinHoleCamera.h"
#include "SmartImage.h"
#include "Functions.h"
//=================================
class FreeOrbitCamera {
// Fly through the scene like you want!
// This FreeOrbitCamera class allows the user to interact with the 
// pin_hole_camera during run time. 
// A pin_hole_cam is created. Its position, orientation and field of
// view can be modified by the user during run time.
// The image is displayed in a additional window and not written to the
// harddisk.
// Above this one can take snapshots using a aperture camera set to the Mamiya 645
// By using two spatial seperater cameras also 3D stereo images are possible
private:
	double FieldOfView_in_rad;

	SmartImage 	Image;

	Vector3D 	t_World2Camera;
	double 		RotWorld2CameraY_in_rad;
	double 		RotWorld2CameraZ_in_rad;
	Rotation3D 	R_World2Camera;

	PinHoleCamera FlyingPinHoleCamera;

	//ool flag_world_and_settings_are_set;
	CartesianFrame *world;
	GlobalSettings *settings;

	// free orbit display name
	std::string free_orbit_display_name = "Monte Carlo Tracer -> Free Orbit";

	// snapshot
	int snapshot_counter;

	// stereo view
	double stereo_offset_increment_in_m;
	double stereo_offset_in_m;
public:
	
	FreeOrbitCamera(CartesianFrame *world,	GlobalSettings *settings);
private:
	void start_free_orbit();

	void aquire_image();
	
	void update_free_orbit_display();
	
	void move_forward();
	
	void move_backward();
	
	void move_left();
	
	void move_right();
	
	void increase_FoV();
	
	void decrease_FoV();
	
	void look_up();
	
	void look_down();
	
	void look_left();
	
	void look_right();
	
	void take_snapshot();
	
	void display_help()const;
	
	void increase_stereo_offset();
	
	void decrease_stereo_offset();

	static void onMouse(int event, int x, int y, int flags, void *param);

	void DispImageInfo(int x, int y);

	bool is_not_Escape(const int key)const;

	double rotation_increment()const;

	double translation_increment()const;

	void update_camera_position(Vector3D new_translation_World2Camera);

	std::string get_prefix_print()const;

	void assert_RotWorld2CameraZ_is_not_overtwisting();

	ApertureCamera create_Mamiya645_based_on_free_orbit_camera()const;
};
#endif // __FREEORBITCAMERA_H_INCLUDED__



