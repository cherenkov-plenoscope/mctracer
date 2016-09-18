#include "Stereo3D.h"
namespace CameraMan {
//------------------------------------------------------------------------------
Stereo3D::Stereo3D(CameraDevice* camera_to_work_with) {
	camera = camera_to_work_with;
}
//------------------------------------------------------------------------------
void Stereo3D::aquire_stereo_image(
	const Frame* world,
	const TracerSettings* settings
){
	remember_initial_camera_config();
	calc_pos_and_rot_for_left_and_right_camera_config(world, settings);
	set_up_camera_in_left_stereo_config();
	take_left_image(world, settings);
	set_up_camera_in_right_stereo_config();
	take_right_image(world, settings);
	set_up_camera_back_to_initial_config();
}
//------------------------------------------------------------------------------
void Stereo3D::calc_pos_and_rot_for_left_and_right_camera_config(
	const Frame* world,
	const TracerSettings* settings
){
	set_positions_for_left_and_right_stereo_config();
	set_object_distance_to_focus_on(world, settings);
	set_intersec_point_for_left_and_right_optical_axis();
	set_pointing_dir_for_left_and_right_stereo_config();	
}
//------------------------------------------------------------------------------
void Stereo3D::remember_initial_camera_config(){
	remember_initial_camera_position_and_rotation();
	remmber_initial_camera_image_upward_get_direction();
}
//------------------------------------------------------------------------------
void Stereo3D::remember_initial_camera_position_and_rotation(){
	initial_camera_pos = camera->get_position_in_world();
	initial_camera_rotation = camera->get_rotation_in_world();
}
//------------------------------------------------------------------------------
void Stereo3D::set_positions_for_left_and_right_stereo_config(){
	left_camera_pos = initial_camera_pos - offset_to_the_right();
	right_camera_pos = initial_camera_pos + offset_to_the_right();
}
//------------------------------------------------------------------------------
void Stereo3D::take_left_image(
	const Frame* world,
	const TracerSettings* settings
){
	camera->acquire_image(world, settings);
	left_image = new CameraImage(camera->get_image());
}
//------------------------------------------------------------------------------
void Stereo3D::take_right_image(
	const Frame* world,
	const TracerSettings* settings
){
	camera->acquire_image(world, settings);
	right_image = new CameraImage(camera->get_image());
}
//------------------------------------------------------------------------------
Vec3 Stereo3D::offset_to_the_right()const {
	return 	camera->direction_to_the_right_of_the_camera()*
		stereo_offset_in_m/2.0;
}
//------------------------------------------------------------------------------
void Stereo3D::remmber_initial_camera_image_upward_get_direction(){
	initial_camera_image_upward_direction = 
		camera->get_image_upwards_direction_in_world_frame();
}
//------------------------------------------------------------------------------
void Stereo3D::set_object_distance_to_focus_on(
	const Frame* world,
	const TracerSettings* settings
){
	Ray optical_axis = camera->get_optical_axis_in_world();
	DistanceMeter dist_meter(&optical_axis, world);

	if(dist_meter.does_face_an_object()) {
		distance_to_object = dist_meter.get_distance_to_closest_object();
	}else{
		distance_to_object = 1e2;
	}
}
//------------------------------------------------------------------------------
void Stereo3D::set_intersec_point_for_left_and_right_optical_axis(){
	intersection_point_for_l_and_r_optical_axes = 
		camera->get_optical_axis_in_world().get_pos_at(distance_to_object);
}
//------------------------------------------------------------------------------
void Stereo3D::set_pointing_dir_for_left_and_right_stereo_config(){
	left_camera_direction_optical_axis = 
		intersection_point_for_l_and_r_optical_axes - left_camera_pos;

	right_camera_direction_optical_axis = 
		intersection_point_for_l_and_r_optical_axes - right_camera_pos;

	left_camera_direction_optical_axis.normalize();	
	right_camera_direction_optical_axis.normalize();
}
//------------------------------------------------------------------------------
void Stereo3D::set_up_camera_in_left_stereo_config(){
	camera->update_position(left_camera_pos);
	camera->set_pointing_direction(
		left_camera_direction_optical_axis,
		initial_camera_image_upward_direction
	);
}
//------------------------------------------------------------------------------
void Stereo3D::set_up_camera_in_right_stereo_config(){
	camera->update_position(right_camera_pos);
	camera->set_pointing_direction(
		right_camera_direction_optical_axis,
		initial_camera_image_upward_direction
	);	
}
//------------------------------------------------------------------------------
void Stereo3D::set_up_camera_back_to_initial_config(){
	camera->update_position_and_orientation(
		initial_camera_pos,
		initial_camera_rotation
	);
}
//------------------------------------------------------------------------------
void Stereo3D::increase_stereo_offset(){
	stereo_offset_in_m = stereo_offset_in_m * 1.05;
	if(verbose) print_stereo_offset_manipulation("increased to");
}
//------------------------------------------------------------------------------
void Stereo3D::decrease_stereo_offset(){
	stereo_offset_in_m = stereo_offset_in_m / 1.05;
	if(verbose) print_stereo_offset_manipulation("decreased to");
}
//------------------------------------------------------------------------------
void Stereo3D::print_stereo_offset_manipulation(
	const std::string status
)const{
	std::cout << camera->get_name() << " stereo offset " << status << ": ";
	std::cout << stereo_offset_in_m << "m\n";
}
//------------------------------------------------------------------------------
const CameraImage* Stereo3D::get_anaglyph_stereo3D_image() {
	left_image->merge_left_and_right_image_to_anaglyph_3DStereo(
		left_image,
		right_image	
	);
	return left_image;
}
//------------------------------------------------------------------------------
void Stereo3D::use_same_stereo_offset_as(
	const Stereo3D *college
) {
	stereo_offset_in_m = college->stereo_offset_in_m;
}
//------------------------------------------------------------------------------
}//CameraMan