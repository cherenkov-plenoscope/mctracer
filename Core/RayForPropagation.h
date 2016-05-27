//=================================
// include guard
#ifndef __RayForPropagation_H_INCLUDED__
#define __RayForPropagation_H_INCLUDED__

//=================================
// forward declared dependencies
class PropagationEnvironment;
//=================================
// included dependencies
#include "Core/Ray.h"
#include "Core/Intersection.h"
#include "PropagationEnvironment.h"
#include "Core/SurfaceEntity.h"
using std::vector;
using std::string;

enum InteractionType { 
	production,
	absorption_in_void,

	absorption_in_medium,
	absorption_on_surface,

	fresnel_reflection_on_surface,
	reflection_on_surface,

	refraction_to_outside,
	refraction_to_inside,

	scattering 
};

//=================================
class RayForPropagation :public Ray{
	friend class TrajectoryFactory;
protected:

	vector<Intersection> intersection_history;
	vector<InteractionType> interaction_type_history;
	uint identifier;
public:
	
	RayForPropagation(const Vec3 support, const Vec3 direction);
	virtual ~RayForPropagation();
	void set_id(const uint nID);
	uint get_id()const;
	void push_back_intersection_and_type_to_propagation_history(
		const Intersection& interact, 
		const InteractionType type
	);
	string get_print()const;
	double get_accumulative_distance()const;
	virtual void propagate_in(PropagationEnvironment env);
	uint get_number_of_interactions_so_far()const;
	const Intersection& get_intersection_at(const uint index)const;
	InteractionType get_final_interaction_type()const;
	const Intersection& get_final_intersection()const;
	virtual double get_time_of_flight()const;
	Vec3 get_final_intersection_incident_vector_in_object_frame()const;
protected:

	void push_back_production_of_ray();
	string get_history_print()const;
	string get_type_print(const InteractionType type)const;
};
#endif // __RayForPropagation_H_INCLUDED__ 