//=================================
// include guard
#ifndef __InteractionSurfaceFinder_H_INCLUDED__
#define __InteractionSurfaceFinder_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <string>
#include <sstream>
#include "Ray.h"
#include "Intersection.h"
#include "SurfaceProperties.h"

//=================================
class InteractionSurfaceFinder {
	bool is_outer_surface;
	const SurfaceProperties* surf = nullptr;
public:
	InteractionSurfaceFinder(const Ray* ray, const Intersection* intersec);
	bool has_surface_interaction()const;
	bool has_surface_interaction_on_outer_surface()const;
	const SurfaceProperties* get_interaction_surface()const;
private:
	bool ray_is_in_hemisphere_with_surface_normal(
		const Ray* ray, 
		const Intersection* intersec
	)const;
}; 
#endif // __InteractionSurfaceFinder_H_INCLUDED__ 