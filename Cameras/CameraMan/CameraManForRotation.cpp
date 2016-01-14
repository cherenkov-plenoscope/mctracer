#include "CameraManForRotation.h"

CameraManForRotation::CameraManForRotation(CameraDevice* camera_to_work_with){
	camera = camera_to_work_with;
}

void CameraManForRotation::set_default_rotation(const Rotation3D rot){
	RotWorld2CameraY_in_rad = rot.get_rot_y();
	RotWorld2CameraZ_in_rad = rot.get_rot_z();
	update_R_World2Camera();
}

void CameraManForRotation::look_further_up_when_possible(){
	if(is_not_falling_backwards_over_when_looking_up()){
		look_further_up();
		print_when_verbose_camera_rotation_for_y_axis_manipulation();
	}else{
		look_straight_up();
		print_when_verbose_camera_can_not_look_further("up");
	}
}

bool CameraManForRotation::is_not_falling_backwards_over_when_looking_up()const{
	return RotWorld2CameraY_in_rad < -rotation_increment();
}

void CameraManForRotation::look_further_up(){
	increment_rot_y();
	update_R_World2Camera();			
}

void CameraManForRotation::look_straight_up() {
	RotWorld2CameraY_in_rad = 0.0;
	update_R_World2Camera();
}

void CameraManForRotation::increment_rot_y(){
	RotWorld2CameraY_in_rad = RotWorld2CameraY_in_rad + rotation_increment();
}

void CameraManForRotation::update_R_World2Camera(){
	R_World2Camera.set(0.0, RotWorld2CameraY_in_rad, RotWorld2CameraZ_in_rad);
	camera->update_orientation(R_World2Camera);		
}

void CameraManForRotation::look_further_down_when_possible(){	
	if(is_not_falling_forward_over_when_looking_down()){
		look_further_down();
		print_when_verbose_camera_rotation_for_y_axis_manipulation();
	}else{
		look_straight_down();
		print_when_verbose_camera_can_not_look_further("down");
	}
}

void CameraManForRotation::look_further_down(){
	decrease_rot_y();
	update_R_World2Camera();			
}

void CameraManForRotation::look_straight_down() {
	RotWorld2CameraY_in_rad = -M_PI;
	update_R_World2Camera();
}

bool CameraManForRotation::is_not_falling_forward_over_when_looking_down()const{
	return RotWorld2CameraY_in_rad > -M_PI + rotation_increment();
}

void CameraManForRotation::decrease_rot_y(){
	RotWorld2CameraY_in_rad = RotWorld2CameraY_in_rad - rotation_increment();
}

void CameraManForRotation::print_when_verbose_camera_rotation_for_y_axis_manipulation()const {
	if(verbose) {
		std::cout << camera->get_name() << " rot y: ";
		std::cout << Rad2Deg(RotWorld2CameraY_in_rad) << "DEG\n";
	}
}

void CameraManForRotation::print_when_verbose_camera_can_not_look_further(
	const std::string dir
)const{
	if(verbose) {
		std::cout << camera->get_name() << " can not look further " << dir << ", ";
		std::cout << " rot y: " << Rad2Deg(RotWorld2CameraY_in_rad) << "DEG\n";
	}
}

void CameraManForRotation::look_left(){
	increase_rot_z();
	update_R_World2Camera();
	print_when_verbose_camera_rotation_for_z_axis_manipulation();
}

void CameraManForRotation::look_right(){
	decrease_rot_z();
	update_R_World2Camera();
	print_when_verbose_camera_rotation_for_z_axis_manipulation();
}

void CameraManForRotation::increase_rot_z(){
	assert_RotWorld2CameraZ_is_not_overtwisting();
	RotWorld2CameraZ_in_rad = RotWorld2CameraZ_in_rad + rotation_increment();
}

void CameraManForRotation::decrease_rot_z(){
	assert_RotWorld2CameraZ_is_not_overtwisting();
	RotWorld2CameraZ_in_rad = RotWorld2CameraZ_in_rad - rotation_increment();
}

void CameraManForRotation::print_when_verbose_camera_rotation_for_z_axis_manipulation()const {
	if(verbose) {
		std::cout << camera->get_name() << " rot z: ";
		std::cout << Rad2Deg(RotWorld2CameraZ_in_rad) << "DEG\n";
	}
}

void CameraManForRotation::assert_RotWorld2CameraZ_is_not_overtwisting(){
	RotWorld2CameraZ_in_rad = fmod(RotWorld2CameraZ_in_rad,(2.0*M_PI));
}

double CameraManForRotation::rotation_increment()const{
	return camera->get_FoV_in_rad()/25.0;
}

Rotation3D CameraManForRotation::get_R_World2Camera()const {
	return R_World2Camera;
}