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

	std::vector<const Intersection*>* intersection_history;
	std::vector<InteractionType>* interaction_type_history;

	uint identifier_number;

	PropagationEnvironment environment = 
		PropagationEnvironment::default_environment;

	const Intersection* intersection = Intersection::void_intersection;

public:
	RayForPropagation(const RayForPropagation* ray_to_be_carried_on);

	RayForPropagation(const Vector3D support, const Vector3D direction);

	virtual ~RayForPropagation(){};

	void set_id(const uint nID);

	uint get_id()const;

	void push_back_intersection_and_type_to_propagation_history(
		const Intersection* interact, 
		const InteractionType type
	);

	std::string get_print()const;

	double get_accumulative_distance()const;
  	
	virtual void propagate_in(PropagationEnvironment env);

	uint get_number_of_interactions_so_far()const;

	const Intersection* get_intersection_at(const uint index)const;

	InteractionType get_final_interaction_type()const {
		return interaction_type_history->back();
	};
	
	const Intersection* get_final_intersection()const;

	virtual double get_time_of_flight()const;
	
	virtual void delete_history();

	void transform(const HomoTrafo3D *T);
	
	Vector3D get_final_intersection_incident_vector_in_object_frame()const;
protected:
	std::string get_history_print()const;

	RayForPropagation();

	void init_propagation_history();



	void carry_on_propagation_properties_of_ray(
		const RayForPropagation* ray_to_be_carried_on
	);

	std::string get_type_print(const InteractionType type)const;

private:
	void push_back_production_of_ray();

	static const Color* trajectory_col;
	static const Color* absorption_in_void_col;
	static const Color* interaction_col;
};
#endif // __RayForPropagation_H_INCLUDED__ 