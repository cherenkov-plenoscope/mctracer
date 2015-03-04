#include "MmcsCorsikaEvent.h"
//------------------------------------------------------------------------------
MmcsCorsikaEvent::MmcsCorsikaEvent(
		MmcsCorsikaEventHeader event_header,
		MmcsCorsikaPhotonData photon_data,
		MmcsCorsikaSubBlock event_end
){
	this->event_header = event_header;
	this->photon_data = photon_data;
	this->event_end	= event_end;
}
//------------------------------------------------------------------------------
ListOfPropagations* MmcsCorsikaEvent::use_once_more_and_get_mctracer_photons() {
// CORSIKA Coordinate system
// The coordinates in CORSIKA are defined with respect to a Cartesian coordinate
// system with the positive x-axis pointing to the magnetic north, the positive
// y-axis to the west, and the z-axis upwards. 
// The origin is located at sea level. This definition is necessary, 
// because the Earth’s magnetic field is taken into account. By default it is
// implemented for the location of Karlsruhe (49o N, 8oE). The zenith angle
// Theta of a particle trajectory is measured between the particle momen-
// tum vector and the negative z-axis, and the azimuthal angle Phi between the 
// positive x-axis and the x-y-component of the particle momentum vector 
// (i.e. with respect to north) proceeding counterclockwise.
	

	//std::cout << "reuse_counter " << reuse_counter << " max_uses" <<  event_header.x_coordinate_of_core_location_for_scattered_events_in_cm.size() << "\n";

	std::stringstream name_of_event;
	name_of_event << "MmcsEvent_reuse_" << reuse_counter+1;

	ListOfPropagations *cherenkov_photons;
	cherenkov_photons = new ListOfPropagations(name_of_event.str());

	for(uint i=0; i<photon_data.number_of_photons(); i++)
		cherenkov_photons->push_back(get_mctracer_photons(i));

	reuse_counter++;
	return cherenkov_photons;
}
//------------------------------------------------------------------------------
bool MmcsCorsikaEvent::can_be_reused_again()const {
	return reuse_counter < 
		event_header.x_coordinate_of_core_location_for_scattered_events_in_cm.size();
}
//------------------------------------------------------------------------------
Photon* MmcsCorsikaEvent::get_mctracer_photons(const uint i)const {

	Vector3D causal_dir = causal_direction(i);

	Ray ray_running_upwards_from_ground_to_pos_of_production(
		intersection_with_xy_floor_plane(i),
		causal_dir*(-1)
	);

	double ray_parameter_for_production_height = (
		production_height_in_m(i) - intersection_with_xy_floor_plane(i).z()
		)/-causal_dir.z();

	Vector3D causal_support = 
		ray_running_upwards_from_ground_to_pos_of_production.
		PositionOnRay(ray_parameter_for_production_height);

	Photon* cherenkov_photon = 
		new Photon(causal_support, causal_dir, wavelength_in_m(i));	

	cherenkov_photon->set_id(i);

	return cherenkov_photon;
}
//------------------------------------------------------------------------------
Vector3D MmcsCorsikaEvent::causal_direction(const uint i)const {

	const double z = sqrt(
		1.0 -
		photon_data.get_cosine_between_dir_and_world_x_axis(i)*
		photon_data.get_cosine_between_dir_and_world_x_axis(i) -
		photon_data.get_cosine_between_dir_and_world_y_axis(i)*
		photon_data.get_cosine_between_dir_and_world_y_axis(i)
	);

	Vector3D causal_dir(
		double(photon_data.get_cosine_between_dir_and_world_x_axis(i)),
		double(photon_data.get_cosine_between_dir_and_world_y_axis(i)),
		z
	);	

	return causal_dir*-1.0;
}
//------------------------------------------------------------------------------
Vector3D MmcsCorsikaEvent::intersection_with_xy_floor_plane(const uint i)const {
	
	return Vector3D(
		x_pos_on_xy_plane_in_m(i) - x_core_position_in_m(), 
		y_pos_on_xy_plane_in_m(i) - y_core_position_in_m(), 
		0.0
	);
}
//------------------------------------------------------------------------------
double MmcsCorsikaEvent::x_core_position_in_m()const {
	return event_header.
		x_coordinate_of_core_location_for_scattered_events_in_cm.
			at(reuse_counter)*1e-2;
}
//------------------------------------------------------------------------------
double MmcsCorsikaEvent::y_core_position_in_m()const {
	return event_header.
		y_coordinate_of_core_location_for_scattered_events_in_cm.
			at(reuse_counter)*1e-2;
}
//------------------------------------------------------------------------------
double MmcsCorsikaEvent::x_pos_on_xy_plane_in_m(const uint i)const {
	return photon_data.get_x_pos_on_world_x_y_plane_in_cm(i)*1e-2;
}
//------------------------------------------------------------------------------
double MmcsCorsikaEvent::y_pos_on_xy_plane_in_m(const uint i)const {
	return photon_data.get_y_pos_on_world_x_y_plane_in_cm(i)*1e-2;
}
//------------------------------------------------------------------------------
double MmcsCorsikaEvent::production_height_in_m(const uint i)const {
	return photon_data.get_production_height_in_cm(i)*1e-2;
}
//------------------------------------------------------------------------------
double MmcsCorsikaEvent::wavelength_in_m(const uint i)const {
	return photon_data.get_wavelength_in_nm(i)*1e-9;
}
//------------------------------------------------------------------------------
std::string MmcsCorsikaEvent::get_print()const {
	std::stringstream out;
	out << "____MAGCIC_MC_CORSIKA_EVENT____\n";
	out << "\n";
	out << StringTools::place_first_infront_of_each_new_line_of_second(
		" ",
		event_header.get_print()
	);
	out << "\n";
	out << StringTools::place_first_infront_of_each_new_line_of_second(
		" ",
		photon_data.get_statistics_print()
	);
	out << "\n";
	return out.str();
}
//------------------------------------------------------------------------------