//=================================
// include guard
#ifndef __PHOTON_H_INCLUDED__
#define __PHOTON_H_INCLUDED__

//=================================
// forward declared dependencies
class Intersection;
//=================================
// included dependencies
#include "Ray.h"
#include <list>
#include "ListOfInteractions.h"
//======================================================================
//struct PhotonProperties{
	//double wavelength;
	//polarisation?
	//phase?
//};
//=================================
class Photon :public Ray{
protected:

	//PhotonProperties properties;
	double wavelength;
	//polarisation?
	//phase?
	Photon(double new_wavelength);
//=================================================
public:

	Photon(
		double baseX,double baseY,double baseZ,
		double dirX, double dirY, double dirZ,
		double new_wavelength
	);

	Photon(Vector3D support, Vector3D direction,double new_wavelength);

	Photon(Ray prototype_ray_for_photon,double new_wavelength);

	void propagate(
		const CartesianFrame* world, 
		ListOfInteractions* history,
		const GlobalSettings* settings,
		PseudoRandomNumberGenerator* dice
	);

	void propagate(	
		const CartesianFrame* world, 
		ListOfInteractions* history,
		int interaction_count,
		const CartesianFrame* object_reflected_from,
		const GlobalSettings* settings,
		PseudoRandomNumberGenerator* dice
	);

	double get_wavelength()const;

	void disp()const;

	std::string get_string()const;

	friend std::ostream& operator<<(
		std::ostream& os, 
		const Photon& photon_to_be_displayed
	);

private:
	bool reflection_takes_place_in_intesection(
		Intersection* intersection, PseudoRandomNumberGenerator* dice
	);
};
#endif // __PHOTON_H_INCLUDED__ 
