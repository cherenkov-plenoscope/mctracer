//=================================
// include guard
#ifndef __VisualTracer_H_INCLUDED__
#define __VisualTracer_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "CameraRay.h"
#include "Config.h"
#include "Core/Random/Random.h"
#include "Core/Photon.h"
#include "Core/Frame.h"
#include "Core/Color.h"
#include "Core/RayAndFrame.h"
#include "Core/PropagationEnvironment.h"
//=================================

namespace Visual {

class Tracer {
public:
	Random::Mt19937 prng;

	const Frame* scenery;
	const Config* config;

	CameraRay* cray;
	Intersection isec;
	Color color;

	static const double wavelength;

	Tracer(
		CameraRay* cray,
		const Frame* scenery,
		const Config* config);
	void back_trace();
	void came_from_object_interaction();
	void reflect_on_surface_and_back_trace_further();
	void reach_boundary_layer();
	void fresnel_refraction_and_reflection();
	void get_absorbed_on_surface();
	void back_trace_beyond_boundary_layer(
		const FresnelRefractionAndReflection &fresnel
	);
	void came_from_sky_dome();
};

}//Visual

#endif // __VisualTracer_H_INCLUDED__ 
