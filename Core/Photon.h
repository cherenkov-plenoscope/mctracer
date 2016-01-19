//=================================
// include guard
#ifndef __PHOTON_H_INCLUDED__
#define __PHOTON_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/RayForPropagation.h"
#include "Core/PhotonMcTruth.h"
#include "FresnelRefractionAndReflection.h"

//=================================
class Photon :public RayForPropagation{
protected:

	double wavelength;
	const PhotonMcTruth* mc_truth = &PhotonMcTruth::void_truth;

public:
	Photon(
		const Vector3D support,
		const Vector3D direction,
		const double wavelength
	);

	Photon(
		const Vector3D support,
		const Vector3D direction,
		const double wavelength,
		const PhotonMcTruth* mc_truth
	);

	Photon(const Photon* photon_to_be_carried_on);

	void propagate_in(PropagationEnvironment environment);

	double get_wavelength()const;
	const PhotonMcTruth* get_mc_truth()const;

	std::string get_print()const;

	double get_time_of_flight()const;

	class BadWaveLength : public TracerException {
		using TracerException::TracerException;
	};		

	void delete_history();
private:
	Photon(const double new_wavelength);

	void interact_with_object();

	void get_absorbed_in_void_space();

	void work_on_first_causal_intersection();

	bool limit_of_interactions_is_not_reached_yet()const;

	void get_absorbed_on_surface();

	void get_reflected_on_surface();

	void reflect_on_surface_and_propagate_on(const InteractionType type);

	void reach_boundary_layer();

	void fresnel_refraction_and_reflection();

	void pass_the_boundary_layer(
		const FresnelRefractionAndReflection &fresnel
	);

	void propagate_on_after_boundary_layer(
		const FresnelRefractionAndReflection &fresnel
	);

	double get_time_to_pass_distance_in_refractive_index(
		const double distance_in_medium,
		const double refractive_index_in_medium
	)const;

	void assert_wavelength_is_positive()const;

	bool absorbed_in_medium_before_reaching_surface();
};
#endif // __PHOTON_H_INCLUDED__ 
