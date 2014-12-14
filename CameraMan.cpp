#include "CameraMan.h"

CameraManFoV::CameraManFoV(CameraDevice* camera_to_work_with) {
	camera = camera_to_work_with;
	set_default_FoV();
}

void CameraManFoV::set_default_FoV() {
	camera->set_FoV_in_rad( default_FoV_in_rad );
}

void CameraManFoV::increase_FoV_when_possible() {
	if(FoV_is_not_too_large_when_increased()){
		increase_FoV();
	}else{
		print_can_not_be_increased_when_verbose();
	}
}

void CameraManFoV::decrease_FoV_when_possible() {
	if(FoV_is_not_too_small_when_decreased()){
		decrease_FoV();
	}else{
		print_can_not_be_decreased_when_verbose();
	}
}

double CameraManFoV::FoV_increment_in_rad()const {
	return Deg2Rad(5.0);
}

bool CameraManFoV::FoV_is_not_too_large_when_increased()const {
	return camera->get_FoV_in_rad() + FoV_increment_in_rad() < max_FoV_in_rad;
}

bool CameraManFoV::FoV_is_not_too_small_when_decreased()const {
	return camera->get_FoV_in_rad() - FoV_increment_in_rad() > min_FoV_in_rad;
}

void  CameraManFoV::increase_FoV() {
	camera->set_FoV_in_rad( camera->get_FoV_in_rad() + FoV_increment_in_rad());
	print_FoV_manipulation_when_verbose();
}

void  CameraManFoV::decrease_FoV() {
	camera->set_FoV_in_rad( camera->get_FoV_in_rad() - FoV_increment_in_rad());
	print_FoV_manipulation_when_verbose();
}

double CameraManFoV::get_default_FoV_in_rad()const {
	return default_FoV_in_rad;
}

void CameraManFoV::print_FoV_manipulation_when_verbose()const {
	if(verbosity){
		print_prefix();
		std::cout << Rad2Deg(camera->get_FoV_in_rad()) << " DEG\n";	
	}
}

void CameraManFoV::print_can_not_be_decreased_when_verbose()const {
	if(verbosity){
		print_prefix();
		std::cout << "Can not decrease FoV any further. Min Fov: ";
		std::cout << Rad2Deg(min_FoV_in_rad) << " DEG\n";
	}
}

void CameraManFoV::print_can_not_be_increased_when_verbose()const {
	if(verbosity){
		print_prefix();
		std::cout << "Can not increase FoV any further. Max Fov: ";
		std::cout << Rad2Deg(max_FoV_in_rad) << " DEG\n";
	}
}

void CameraManFoV::print_prefix()const {
	std::cout << camera->get_name() << " FoV: ";
}