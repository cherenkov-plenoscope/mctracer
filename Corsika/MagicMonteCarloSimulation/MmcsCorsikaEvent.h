//=================================
// include guard
#ifndef __MMCSCORSIKAEVENT_H_INCLUDE__
#define __MMCSCORSIKAEVENT_H_INCLUDE__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "MmcsCorsikaSubBlock.h"
#include "MmcsCorsikaEventHeader.h"
#include "MmcsCorsikaPhotonData.h"
#include "Core/Photon.h"
//=================================
class MmcsCorsikaEvent {

	MmcsCorsikaEventHeader event_header;
	MmcsCorsikaPhotonData photon_data;
	MmcsCorsikaSubBlock event_end;

	unsigned int reuse_counter = 0;
public:
	MmcsCorsikaEvent() {};

	MmcsCorsikaEvent(
		MmcsCorsikaEventHeader event_header,
		MmcsCorsikaPhotonData photon_data,
		MmcsCorsikaSubBlock event_end
	);

	std::vector<Photon> use_once_more_and_get_photons();
	bool can_be_reused_again()const;

	std::string str()const;
	double get_Az()const{return event_header.azimuth_angle_Phi_in_radian;}
	double get_Zd()const{return event_header.zenith_angle_Theta_in_radian;}

private:
	double x_core_position_in_m()const;
	double y_core_position_in_m()const;
	double x_pos_on_xy_plane_in_m(const unsigned int i)const;
	double y_pos_on_xy_plane_in_m(const unsigned int i)const;
	double wavelength_in_m(const unsigned int i)const;
	double production_height_in_m(const unsigned int i)const;
	Vec3 intersection_with_xy_floor_plane(const unsigned int i)const;
	Photon get_mctracer_photon(const unsigned int i)const;
	Vec3 causal_direction(const unsigned int i)const;
};

#endif // __MMCSCORSIKAEVENT_H_INCLUDE__ 
