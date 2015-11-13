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
#include "Tools/Tools.h"
#include "CameraMan.h"
#include "CameraManForTranslation.h"
#include "CameraManForRotation.h"
#include "CameraManForStereo3D.h"
#include "CameraManForFieldOfView.h"
#include "Core/TimeStamp.h"

//=================================
class FreeOrbitCamera {
// Fly through the scene like you want!
// This FreeOrbitCamera class allows the user to interact with the 
// pin_hole_camera during run time. 
// A pin_hole_cam is created. Its position, orientation and field of
// view can be modified by the user during run time.
// The image is displayed in a additional window and not written to the
// harddisk.
// Above this one can take snapshots using a aperture camera.
// Also 3D stereo images are possible
public:
	
	FreeOrbitCamera(
		const Frame *world, 
		const TracerSettings *settings
	);

	void continue_with_new_scenery_and_settings(
		const Frame *world, 
		const TracerSettings *settings
	);
private:
	const std::string free_orbit_display_name = 
		"Monte Carlo Tracer -> Free Orbit";

	PinHoleCamera *flying_camera;

	CameraManFoV 				*FoV_operator;
	CameraManForTranslation 	*Translation_operator;
	CameraManForRotation 		*Rotation_operator;
	CameraManForStereo3D		*Stereo_operator;

	bool stereo3D = false;

	const Frame *world;
	const TracerSettings *settings;

	int snapshot_counter = 0;
	uint iteration_counter = 0;
	TimeStamp time_stamp;

	int wait_for_user_key_stroke();
	void toggle_stereo3D();
	void create_CameraMen_to_safely_operate_the_flying_camera();
	void start_free_orbit();
	void aquire_image();
	void update_free_orbit_display();
	void take_snapshot_manual_focus_on_pixel_col_row(int x, int y);
	void print_free_orbit_help_text()const;
	void increase_stereo_offset();
	void decrease_stereo_offset();
	static void mouse_button_event(
		int event, 
		int x, 
		int y, 
		int flags, 
		void *param
	);
	void print_info_of_probing_ray_for_pixel_col_row(int x, int y);
	void update_current_camera_position(Vector3D new_translation_World2Camera);
	std::string get_prefix_print()const;
	ApertureCamera get_ApertureCamera_based_on_free_orbit_camera()const;
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
	const CameraImage* acquire_image_with_camera(CameraDevice* cam);
};
#endif // __FREEORBITCAMERA_H_INCLUDED__