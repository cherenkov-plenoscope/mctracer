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

//=================================
class FreeOrbitCamera {
// Fly through the scene like you want!
// This FreeOrbitCamera class allows the user to interact with the 
// pin_hole_camera during run time. 
// A pin_hole_cam is created. Its position, orientation and field of
// view can be modified by the user during run time.
// The image is displayed in a additional window and not written to the
// harddisk.
private:
double FieldOfView_in_rad;
int ImageResolutionInX;
int ImageResolutionInY;

Vector3D t_World2Camera;
double RotWorld2CameraY_in_rad;
double RotWorld2CameraZ_in_rad;
Rotation3D R_World2Camera;

PinHoleCamera FlyingPinHoleCamera;
//==================//
bool flag_world_and_settings_are_set;
CartesianFrame *ptr_to_world;
GlobalSettings *ptr_to_globalsettings;
//==================//
// free orbit display name
std::string free_orbit_display_name;

//==================//
// snapshot
int snapshot_counter;

//==================//
// stereo view
double stereo_offset_increment_in_m;
double stereo_offset_in_m;
public:
//======================================================================
FreeOrbitCamera();
//======================================================================
void set_free_orbit(
CartesianFrame *ptr_to_new_world,
GlobalSettings *ptr_to_new_globalsettings);
//======================================================================
void start_free_orbit();
//======================================================================
private:
void aquire_image();
//======================================================================
void update_free_orbit_display();
//======================================================================
void move_forward();
//======================================================================
void move_backward();
//======================================================================
void move_left();
//======================================================================
void move_right();
//======================================================================
void increase_FoV();
//======================================================================
void decrease_FoV();
//======================================================================
void look_up();
//======================================================================
void look_down();
//======================================================================
void look_left();
//======================================================================
void look_right();
//======================================================================
void take_snapshot();
//======================================================================
void display_help()const;
//======================================================================
void increase_stereo_offset();
void decrease_stereo_offset();
//======================================================================

};
#endif // __FREEORBITCAMERA_H_INCLUDED__



