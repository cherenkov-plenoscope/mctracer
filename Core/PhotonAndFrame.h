//=================================
// include guard
#ifndef __PHOTONANDFRAME_H_INCLUDED__
#define __PHOTONANDFRAME_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/Photon.h"
#include "Core/Frame.h"
#include "Core/RayAndFrame.h"
#include "Core/PropagationEnvironment.h"
//=================================
namespace PhotonAndFrame {

	class Propagator {
	public:
		Intersection isec;
		PropagationEnvironment env;		
		Photon* ph;

		Propagator(
			Photon* photon,
			PropagationEnvironment env
		);
		void propagate();
		bool limit_of_interactions_is_not_reached_yet()const;
		void work_on_first_causal_intersection();
		bool absorbed_in_medium_before_reaching_surface()const ;
		void interact_with_object();
		void get_absorbed_in_void_space();
		void reflect_on_surface_and_propagate_on(const InteractionType type);
		void reach_boundary_layer();
		void fresnel_refraction_and_reflection();
		void pass_the_boundary_layer(
			const FresnelRefractionAndReflection &fresnel 
		);
		void propagate_on_after_boundary_layer(
			const FresnelRefractionAndReflection &fresnel
		);
		void get_absorbed_on_surface() ;
	};
}//PhotonAndFrame
#endif // __PHOTONANDFRAME_H_INCLUDED__ 
