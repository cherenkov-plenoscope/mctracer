#include "FresnelRefractionAndReflection.h"
//------------------------------------------------------------------------------
FresnelRefractionAndReflection::FresnelRefractionAndReflection(
	const Vector3D &_incident_obj_sys,
	const Vector3D &_normal_obj_sys,
	const double _n_from,
	const double _n_to
) : incident_obj_sys(_incident_obj_sys),
	normal_obj_sys(_normal_obj_sys),
	n_from(_n_from),
	n_to(_n_to)
{
	//std::cout << "i: " << incident_obj_sys << ", n: "<< normal_obj_sys << "from: "<<_n_from << " to: "<<n_to <<std::endl;

	cosI = normal_obj_sys*incident_obj_sys*(-1.0);
	n_from_over_n_to = n_from/n_to;
	sinT2 = n_from_over_n_to*n_from_over_n_to*(1.0 - cosI*cosI);
	cosT = sqrt(1.0 - sinT2);
}
//------------------------------------------------------------------------------
Vector3D FresnelRefractionAndReflection::get_reflec_dir_in_object_system()const {
	return incident_obj_sys + normal_obj_sys*cosI*2.0;
}
//------------------------------------------------------------------------------
Vector3D FresnelRefractionAndReflection::get_refrac_dir_in_object_system()const {
	return incident_obj_sys*n_from_over_n_to +
	 	normal_obj_sys*(n_from_over_n_to*cosI - cosT);
}
//------------------------------------------------------------------------------
double FresnelRefractionAndReflection::reflection_propability()const {

	if(sinT2 > 1.0)
		return 1.0; // total internal reflection

	const double rOrth = (n_from*cosI - n_to*cosT) / (n_from*cosI + n_to*cosT);
	const double rPar = (n_to*cosI - n_from*cosT) / (n_to*cosI + n_from*cosT);
	return (rOrth*rOrth + rPar*rPar) / 2.0;
}
//------------------------------------------------------------------------------
