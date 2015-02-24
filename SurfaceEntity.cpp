#include "SurfaceEntity.h"
//------------------------------------------------------------------------------
void SurfaceEntity::set_outer_surface(const SurfaceProperties *outer_surface) {
	this->outer_surface = outer_surface;
}
//------------------------------------------------------------------------------
void SurfaceEntity::set_inner_surface(const SurfaceProperties *inner_surface) {
	this->inner_surface = inner_surface;
}
//------------------------------------------------------------------------------
bool SurfaceEntity::has_inner_surface()const {
	return inner_surface != nullptr;
}
//------------------------------------------------------------------------------
bool SurfaceEntity::has_outer_surface()const {
	return outer_surface != nullptr;
}
//------------------------------------------------------------------------------
const SurfaceProperties* SurfaceEntity::get_outer_surface()const {
	return outer_surface;
}
//------------------------------------------------------------------------------
const SurfaceProperties* SurfaceEntity::get_inner_surface()const {
	return inner_surface;
}
//------------------------------------------------------------------------------
void SurfaceEntity::set_outer_medium(const VolumeProperties* outer_medium) {
	this->outer_medium = outer_medium;
}
//------------------------------------------------------------------------------
void SurfaceEntity::set_inner_medium(const VolumeProperties* inner_medium) {
	this->inner_medium = inner_medium;
}
//------------------------------------------------------------------------------
bool SurfaceEntity::has_inner_medium()const {
	return inner_medium != nullptr;
}
//------------------------------------------------------------------------------
bool SurfaceEntity::has_outer_medium()const {
	return outer_medium != nullptr;
}
//------------------------------------------------------------------------------
const VolumeProperties* SurfaceEntity::get_outer_medium()const {
	return outer_medium;
}
//------------------------------------------------------------------------------
const VolumeProperties* SurfaceEntity::get_inner_medium()const {
	return inner_medium;
}
//------------------------------------------------------------------------------
void SurfaceEntity::set_allowed_frames_to_propagate_to(
	const CartesianFrame* allowed_frame_to_propagate_to
) {	
	this->allowed_frame_to_propagate_to = allowed_frame_to_propagate_to;
}
//------------------------------------------------------------------------------
bool SurfaceEntity::has_restrictions_on_frames_to_propagate_to()const {
	return allowed_frame_to_propagate_to != nullptr;
}

//------------------------------------------------------------------------------
const CartesianFrame* SurfaceEntity::get_allowed_frame_to_propagate_to()const {
	return allowed_frame_to_propagate_to;
}
//------------------------------------------------------------------------------
std::string SurfaceEntity::get_print()const {
	std::stringstream out;
	out << get_frame_print();
	out << get_surface_print();
	return out.str();
}
//------------------------------------------------------------------------------
std::string SurfaceEntity::get_surface_print()const {
	std::stringstream out;
	out << "surface: \n";

	//std::cout << __func__ <<": "<<__LINE__<<"\n";
	if(has_outer_surface())
		out << "| outer surface: " << outer_surface->get_print() << "\n";

	//std::cout << __func__ <<": "<<__LINE__<<"\n";
	if(has_inner_surface())
		out << "| inner surface: " << inner_surface->get_print()  << "\n";

	//std::cout << __func__ <<": "<<__LINE__<<"\n";
	if(has_outer_medium())
		out << "| outer medium: " << outer_medium->get_print()  << "\n";
	
	//std::cout << __func__ <<": "<<__LINE__<<"\n";
	if(has_inner_medium())
		out << "| inner medium: " << inner_medium->get_print() << "\n";
	
	//std::cout << __func__ <<": "<<__LINE__<<"\n";
	if(has_restrictions_on_frames_to_propagate_to())
		out << "| inner frame: " << allowed_frame_to_propagate_to->
			get_path_in_tree_of_frames() << "\n";


	//std::cout << __func__ <<": "<<__LINE__<<"\n";
	return out.str();
}