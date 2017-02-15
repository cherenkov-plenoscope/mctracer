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
	void trace_back();
	void trace_back_to_object_interaction();
	void trace_back_after_reflection();
	void trace_back_to_boundary_layer();
	void trace_back_to_fresnel_interaction();
	void trace_back_to_surface();
	void trace_back_beyond_boundary_layer(
		const FresnelRefractionAndReflection &fresnel
	);
	void trace_back_to_sky_dome();
};

}//Visual

#endif // __VisualTracer_H_INCLUDED__ 
