#include "CameraManForTranslation.h"

CameraManForTranslation::CameraManForTranslation(CameraDevice* camera_to_work_with) {
	camera = camera_to_work_with;
}

void CameraManForTranslation::set_default_position(){
	camera->update_position(default_position);
}

void CameraManForTranslation::set_default_position(const Vector3D default_pos) {
	default_position = default_pos;
	camera->update_position(default_position);
}

void CameraManForTranslation::move_for(){
	camera->update_position(
		camera->get_position_in_world() +
		camera->get_normalized_pointing_direction()*translation_increment() 
	);
	print_camera_moved_in_direction("for");
}

void CameraManForTranslation::move_back(){
	camera->update_position(
		camera->get_position_in_world() -
		camera->get_normalized_pointing_direction()*translation_increment() 
	);
	print_camera_moved_in_direction("back");
}

void CameraManForTranslation::move_left(){
	camera->update_position(
		camera->get_position_in_world() -
		camera->direction_to_the_right_of_the_camera()*translation_increment() 
	);
	print_camera_moved_in_direction("left");
}

void CameraManForTranslation::move_right(){
	camera->update_position(
		camera->get_position_in_world() +
		camera->direction_to_the_right_of_the_camera()*translation_increment() 
	);
	print_camera_moved_in_direction("right");
}

double CameraManForTranslation::translation_increment()const{
	return 0.5/camera->get_FoV_in_rad();
}

void CameraManForTranslation::print_camera_moved_in_direction(
	const std::string dir
)const{
	std::cout << camera->get_name() << " move " << dir << ": ";
	std::cout << camera->get_position_in_world() << "\n";
}

void CameraManForTranslation::move_right(const double step_in_m){
	camera->update_position(
		camera->get_position_in_world() +
		camera->direction_to_the_right_of_the_camera()*step_in_m 
	);
	print_camera_moved_in_direction("right");
}

void CameraManForTranslation::move_up() {
	camera->update_position(
		camera->get_position_in_world() + 
		Vector3D::unit_z*translation_increment() 
	);
	print_camera_moved_in_direction("up");	
}

void CameraManForTranslation::move_down() {
	camera->update_position(
		camera->get_position_in_world() - 
		Vector3D::unit_z*translation_increment() 
	);
	print_camera_moved_in_direction("up");
}