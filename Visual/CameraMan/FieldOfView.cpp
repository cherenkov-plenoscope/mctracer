#include "FieldOfView.h"
#include "Tools/Tools.h"

namespace Visual {
namespace CameraMan {
//------------------------------------------------------------------------------
FieldOfView::FieldOfView(CameraDevice* camera_to_work_with) {
	camera = camera_to_work_with;
	set_default_FoV();
}
//------------------------------------------------------------------------------
void FieldOfView::set_default_FoV() {
	camera->set_FoV_in_rad( default_FoV_in_rad );
}
//------------------------------------------------------------------------------
void FieldOfView::increase_when_possible() {
	if(too_large_when_increased())
		increase();
	else
		print_can_not_be_increased_when_verbose();
}
//------------------------------------------------------------------------------
void FieldOfView::decrease_when_possible() {
	if(too_small_when_decreased())
		decrease();
	else
		print_can_not_be_decreased_when_verbose();
}
//------------------------------------------------------------------------------
double FieldOfView::rate()const {
	return 1.1;
}
//------------------------------------------------------------------------------
bool FieldOfView::too_large_when_increased()const {
	return camera->get_FoV_in_rad()*rate() < max_FoV_in_rad;
}
//------------------------------------------------------------------------------
bool FieldOfView::too_small_when_decreased()const {
	return camera->get_FoV_in_rad()/rate() > min_FoV_in_rad;
}
//------------------------------------------------------------------------------
void  FieldOfView::increase() {
	camera->set_FoV_in_rad( camera->get_FoV_in_rad()*rate());
	print_manipulation_when_verbose();
}
//------------------------------------------------------------------------------
void  FieldOfView::decrease() {
	camera->set_FoV_in_rad( camera->get_FoV_in_rad()/rate());
	print_manipulation_when_verbose();
}
//------------------------------------------------------------------------------
double FieldOfView::get_default_FoV_in_rad()const {
	return default_FoV_in_rad;
}
//------------------------------------------------------------------------------
void FieldOfView::print_manipulation_when_verbose()const {
	if(verbose){
		print_prefix();
		std::cout << Rad2Deg(camera->get_FoV_in_rad()) << " DEG\n";	
	}
}
//------------------------------------------------------------------------------
void FieldOfView::print_can_not_be_decreased_when_verbose()const {
	if(verbose){
		print_prefix();
		std::cout << "Can not decrease FoV any further. Min Fov: ";
		std::cout << Rad2Deg(min_FoV_in_rad) << " DEG\n";
	}
}
//------------------------------------------------------------------------------
void FieldOfView::print_can_not_be_increased_when_verbose()const {
	if(verbose){
		print_prefix();
		std::cout << "Can not increase FoV any further. Max Fov: ";
		std::cout << Rad2Deg(max_FoV_in_rad) << " DEG\n";
	}
}
//------------------------------------------------------------------------------
void FieldOfView::print_prefix()const {
	std::cout << camera->get_name() << " FoV: ";
}
//------------------------------------------------------------------------------
}//CameraMan
}//Visual