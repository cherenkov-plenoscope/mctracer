//=================================
// include guard
#ifndef __RayForPropagation_H_INCLUDED__
#define __RayForPropagation_H_INCLUDED__

//=================================
// forward declared dependencies
class PropagationEnvironment;
//=================================
// included dependencies
#include "Ray.h"

class RayForPropagation :public Ray{
protected:
	std::vector<const Intersection*>* propagation_history;
	uint identifier_number;
public:
	RayForPropagation(const RayForPropagation* ray_to_be_expanded);

	RayForPropagation(const Vector3D support, const Vector3D direction);

	~RayForPropagation();

	void set_id(const uint nID);

	uint get_id()const;

	void push_back_to_propagation_history(const Intersection* intersec);

	const std::vector<const Intersection*>* get_propagation_history()const;

	std::string get_print()const;

	double get_accumulative_distance()const;
  	
	friend std::ostream& operator<<(
		std::ostream& os,
		const Ray& ray_to_be_displayed
	);


	virtual void propagate_in(PropagationEnvironment* env);
protected:
	std::string get_rayforpropagation_print()const;

	RayForPropagation();

	void init_propagation_history();

	void delete_propagation_history();

	void expand_propagation_properties_of_ray(
		const RayForPropagation* ray_to_be_expanded
	);
};
#endif // __RayForPropagation_H_INCLUDED__ 