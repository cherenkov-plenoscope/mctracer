#include "DistanceMeter.h"
//------------------------------------------------------------------------------
DistanceMeter::DistanceMeter(const Ray* ray, const Frame* world) {
	const Intersection* closest_intersec = ray->get_first_intersection_in(world);

	if(closest_intersec->does_intersect()) {
		does_DistanceMeter_face_an_object = true;
		distance_to_closest_object = 
			closest_intersec->get_intersection_distance();
	}

	delete closest_intersec;
}
//------------------------------------------------------------------------------
bool DistanceMeter::does_face_an_object()const {
	return does_DistanceMeter_face_an_object;
}
//------------------------------------------------------------------------------
double DistanceMeter::get_distance_to_closest_object()const {
	return distance_to_closest_object;
}