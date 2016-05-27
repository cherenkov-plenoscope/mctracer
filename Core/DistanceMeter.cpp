#include "DistanceMeter.h"
#include "Core/RayAndFrame.h"
//------------------------------------------------------------------------------
DistanceMeter::DistanceMeter(const Ray* ray, const Frame* world) {
	RayAndFrame::CausalIntersection causal_intersection(ray, world);
	const Intersection closest_intersec = causal_intersection.closest_intersection;

	if(closest_intersec.does_intersect()) {
		does_DistanceMeter_face_an_object = true;
		distance_to_closest_object = 
			closest_intersec.get_intersection_distance();
	}
}
//------------------------------------------------------------------------------
bool DistanceMeter::does_face_an_object()const {
	return does_DistanceMeter_face_an_object;
}
//------------------------------------------------------------------------------
double DistanceMeter::get_distance_to_closest_object()const {
	return distance_to_closest_object;
}