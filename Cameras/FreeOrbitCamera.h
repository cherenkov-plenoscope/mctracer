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
#include "Tools/Tools.h"
#include "CameraMan.h"
#include "CameraManForTranslation.h"
#include "CameraManForRotation.h"
#include "CameraManForStereo3D.h"

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
public:
	
	FreeOrbitCamera(CartesianFrame *world,	GlobalSettings *settings);
private:
	const std::string free_orbit_display_name = 
		"Monte Carlo Tracer -> Free Orbit";

	SmartImage 	Image;

	PinHoleCamera *flying_camera;

	CameraManFoV 				*FoV_operator;
	CameraManForTranslation 	*Translation_operator;
	CameraManForRotation 		*Rotation_operator;
	CameraManForStereo3D		*Stereo_operator;

	bool stereo3D = false;

	CartesianFrame *world;
	GlobalSettings *settings;

	int snapshot_counter = 0;
	uint iteration_counter = 0;

	void toggle_stereo3D();
	void create_CameraMen_to_safely_operate_the_flying_camera();
	void start_free_orbit();
	void aquire_image();
	void update_free_orbit_display();
	void take_snapshot();
	void print_free_orbit_help_text()const;
	void increase_stereo_offset();
	void decrease_stereo_offset();
	static void left_mouse_button_event(
		int event, 
		int x, 
		int y, 
		int flags, 
		void *param
	);
	void print_info_of_probing_ray_for_pixel_x_y(int x, int y);
	void update_current_camera_position(Vector3D new_translation_World2Camera);
	std::string get_prefix_print()const;
	ApertureCamera get_Mamiya645_based_on_free_orbit_camera()const;
	void create_free_orbit_display();
	void destroy_free_orbit_display();
	void reset_camera();
	void reset_stereo_view();
	void reset_free_orbit();
	bool stereo_offset_wont_be_too_large_when_increased()const;
	bool stereo_offset_wont_be_too_small_when_decreased()const;
	void print_stereo_offset_manipulation(const std::string status)const;
	std::string get_snapshot_filename();
	bool it_is_time_again_to_show_the_help();
};
#endif // __FREEORBITCAMERA_H_INCLUDED__