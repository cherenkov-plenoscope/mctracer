//=================================
// include guard
#ifndef __FresnelRefractionAndReflection_H_INCLUDED__
#define __FresnelRefractionAndReflection_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/Vector3D.h"

//=================================
class FresnelRefractionAndReflection{
protected:

	const Vector3D incident_obj_sys;
	const Vector3D normal_obj_sys;
	const double n_from;
	const double n_to;

	double cosI;
	double n_from_over_n_to;
	double sinT2;
	double cosT;
public:
	
	FresnelRefractionAndReflection(
		const Vector3D &incident_obj_sys,
		const Vector3D &normal_obj_sys,
		double n_from,
		double n_to
	);
	
	Vector3D get_reflec_dir_in_object_system()const;
	
	Vector3D get_refrac_dir_in_object_system()const;
	
	double reflection_propability()const;
};
#endif // __FresnelRefractionAndReflection_H_INCLUDED__ 
