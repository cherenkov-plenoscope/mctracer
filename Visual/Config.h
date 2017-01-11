//=================================
// include guard
#ifndef __VisualConfig_H_INCLUDED__
#define __VisualConfig_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/Color.h"
#include "Core/TracerException.h"
#include "SkyDome.h"
#include "Core/Vec3.h"

//=================================
namespace Visual {
struct Config {
	
	uint max_interactions;

	struct Preview {
		uint cols;
		uint rows;
		uint scale;
	} preview;

	struct Snapshot {
		uint cols;
		uint rows;
		uint rays_per_pixel;
		double focal_length_over_aperture_diameter;
		double image_sensor_size_along_a_row;
	} snapshot;

	struct GlobalIllumination {
		bool on;
		Vec3 incoming_direction;
	} global_illumination;

	struct PhotonTrajectories{
		double radius;
	} photon_trajectories;
	
	SkyDome sky_dome;

	Config();
};
}//Visual
#endif // __VisualConfig_H_INCLUDED__ 
