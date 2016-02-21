//=================================
// include guard
#ifndef __FlyingCamera_H_INCLUDED__
#define __FlyingCamera_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "ApertureCamera.h"
#include "PinHoleCamera.h"
#include "Tools/Tools.h"
#include "Cameras/CameraMan/CameraMan.h"
#include "Tools/TimeStamp.h"
#include "Tools/UserInteraction.h"

//=================================
class FlyingCamera {
// Fly through the scene like you want!
// This FlyingCamera class allows the user to interact with the 
// pin_hole_camera during run time. 
// A pin_hole_cam is created. Its position, orientation and field of
// view can be modified by the user during run time.
// The image is displayed in a additional window and not written to the
// harddisk.
// Above this one can take snapshots using a aperture camera.
// Also 3D stereo images are possible
public:
	
	FlyingCamera(const Frame *world, const TracerSettings *settings);

	void continue_with_new_scenery_and_settings(
		const Frame *world, 
		const TracerSettings *settings
	);

	~FlyingCamera();
private:
	
	const std::string display_name = "Monte Carlo Ray Tracer";

	PinHoleCamera *flying_camera;
	PinHoleCamera *flying_camera_full_resolution;

	CameraImage image;

	CameraManFoV 				*FoV_operator;
	CameraManForTranslation 	*Translation_operator;
	CameraManForRotation 		*Rotation_operator;
	CameraManForStereo3D		*Stereo_operator;

	bool stereo3D = false;

	const Frame *world;
	const TracerSettings *settings;

	int snapshot_counter = 0;
	uint user_input_counter = 0;
	TimeStamp time_stamp;

	int wait_for_user_key_stroke();
	void toggle_stereo3D();
	void create_CameraMen_to_safely_operate_the_flying_camera();
	void enter_interactive_display();
	void aquire_image();
	void update_display();
	void update_display_full_resolution();
	void take_snapshot_manual_focus_on_pixel_col_row(int col, int y);
	void print_display_help_text()const;
	void increase_stereo_offset();
	void decrease_stereo_offset();
	static void mouse_button_event(
		int event, 
		int col, 
		int y, 
		int flags, 
		void *param
	);
	void print_info_of_probing_ray_for_pixel_col_row(int x, int y);
	string get_intersection_info_print(const Intersection* intersec)const;
	void update_current_camera_position(Vector3D new_translation_World2Camera);
	std::string get_prefix_print()const;
	ApertureCamera get_ApertureCamera_based_on_display_camera()const;
	void create_display();
	void destroy_display();
	void reset_camera();
	void reset_stereo_view();
	void reset_free_orbit();
	bool stereo_offset_wont_be_too_large_when_increased()const;
	bool stereo_offset_wont_be_too_small_when_decreased()const;
	void print_stereo_offset_manipulation(const std::string status)const;
	std::string get_snapshot_filename();
	bool it_is_time_again_to_show_the_help();
	const CameraImage* acquire_scaled_image_with_camera(
		const bool scale, CameraDevice* cam
	);
};
#endif // __FlyingCamera_H_INCLUDED__