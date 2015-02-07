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
#include "PseudoRandomNumberGenerator.h"
//=================================
class Photon :public Ray{
protected:

	double wavelength;
	//polarisation?
	//phase?
	Photon(double new_wavelength);
public:
	Photon(Vector3D support, Vector3D direction, double new_wavelength);

	Photon(Ray prototype_ray_for_photon, double new_wavelength);

	void propagate(
		const CartesianFrame* world, 
		ListOfInteractions* history,
		const GlobalSettings* settings,
		PseudoRandomNumberGenerator* dice
	);

	void propagate(	
		const CartesianFrame* world, 
		ListOfInteractions* history,
		uint interaction_count,
		const GlobalSettings* settings,
		PseudoRandomNumberGenerator* dice
	);

	double get_wavelength()const;

	void print()const;

	std::string get_print()const;

	friend std::ostream& operator<<(
		std::ostream& os, 
		const Photon& photon_to_be_displayed
	);

private:
	bool reflection_takes_place_in_intesection(
		Intersection* intersection, PseudoRandomNumberGenerator* dice
	);

	void reflect_on_surface(
		const CartesianFrame* world, 
		ListOfInteractions* history,
		uint interaction_count,
		const GlobalSettings* settings,
		PseudoRandomNumberGenerator* dice,
		Intersection* intersection
	);

	void refract_on_surface(
		const CartesianFrame* world, 
		ListOfInteractions* history,
		uint interaction_count,
		const GlobalSettings* settings,
		PseudoRandomNumberGenerator* dice,
		Intersection* intersection
	);
};
#endif // __PHOTON_H_INCLUDED__ 
