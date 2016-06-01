#include "DistanceMeter.h"
#include "Core/RayAndFrame.h"
//------------------------------------------------------------------------------
DistanceMeter::DistanceMeter(const Ray* ray, const Frame* world) {
	const Intersection intersec = RayAndFrame::first_intersection(ray, world);

	if(intersec.does_intersect()) {
		does_DistanceMeter_face_an_object = true;
		distance_to_closest_object = intersec.get_intersection_distance();
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