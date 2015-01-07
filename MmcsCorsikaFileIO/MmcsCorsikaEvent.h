//=================================
// include guard
#ifndef __MMCSCORSIKAEVENT_H_INCLUDE__
#define __MMCSCORSIKAEVENT_H_INCLUDE__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "MmcsCorsikaSubBlock.h"
#include "MmcsCorsikaPhotonData.h"
#include "ListOfPropagations.h"
#include "Photon.h"
//=================================
class MmcsCorsikaEvent {

MmcsCorsikaSubBlock event_header;
MmcsCorsikaPhotonData photon_data;
MmcsCorsikaSubBlock event_end;

public:
	MmcsCorsikaEvent(
		MmcsCorsikaSubBlock event_header,
		MmcsCorsikaPhotonData photon_data,
		MmcsCorsikaSubBlock event_end
	);

	ListOfPropagations* transform_to_mcTracer_photons()const;
private:
	double x_pos_on_xy_plane_in_m(const uint i)const;
	double y_pos_on_xy_plane_in_m(const uint i)const;
	double wavelength_in_m(const uint i)const;
	double production_height_in_m(const uint i)const;
	Vector3D intersection_with_xy_floor_plane(const uint i)const;
	Photon* get_mcTracer_photon(const uint i)const;
	Vector3D causal_direction(const uint i)const;
};

#endif // __MMCSCORSIKAEVENT_H_INCLUDE__ 
