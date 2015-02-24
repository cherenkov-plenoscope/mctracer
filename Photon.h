//=================================
// include guard
#ifndef __PHOTON_H_INCLUDED__
#define __PHOTON_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "RayForPropagation.h"
#include "PropagationEnvironment.h"
#include "InteractionSurfaceFinder.h"

//=================================
class Photon :public RayForPropagation{
protected:

	double wavelength;
	//polarisation?
	//phase?
public:
	Photon(
		const Vector3D support,
		const Vector3D direction,
		const double wavelength
	);

	Photon(const Photon* photon_to_be_carried_on);

	void propagate_in(PropagationEnvironment* environment);

	double get_wavelength()const;

	std::string get_print()const;

	friend std::ostream& operator<<(
		std::ostream& os, 
		const Photon& photon_to_be_displayed
	);
private:
	Photon(const double new_wavelength);

	void interaction_with_object(const Intersection* intersec);

	void absorbtion_in_void_space(const Intersection* intersec);
};
#endif // __PHOTON_H_INCLUDED__ 
