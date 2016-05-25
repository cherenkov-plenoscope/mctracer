//=================================
// include guard
#ifndef __RAYANDFRAME_H_INCLUDED__
#define __RAYANDFRAME_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/Frame.h"
#include "Core/Ray.h"

namespace RayAndFrame {

	bool ray_support_inside_frames_bounding_sphere(const Ray* ray, const Frame *frame);
	bool ray_has_intersection_with_bounding_sphere_of(const Ray* ray, const Frame* frame);
}
#endif // __RAYANDFRAME_H_INCLUDED__ 