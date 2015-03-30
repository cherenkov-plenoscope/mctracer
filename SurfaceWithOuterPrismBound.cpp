#include "SurfaceWithOuterPrismBound.h"
//------------------------------------------------------------------------------
const Intersection* SurfaceWithOuterPrismBound::get_causeal_intersection(
		const TwoSolutionSurfaceRayEquation* eq,
		const PrismZ* outer_bound,
		const Ray *ray
)const {

	Vector3D plus_intersec = ray->PositionOnRay(eq->get_plus_solution());
	Vector3D minus_intersec = ray->PositionOnRay(eq->get_minus_solution()); 	

	bool p_is_inside = outer_bound->is_inside(&plus_intersec);
	bool m_is_inside = outer_bound->is_inside(&minus_intersec);

	bool p_is_causal = eq->get_plus_solution() > 0.0 && 
		plus_intersec.norm2_is_less_equal_than(radius_of_sphere_enclosing_all_children);

	bool m_is_causal = eq->get_minus_solution() > 0.0 &&
		minus_intersec.norm2_is_less_equal_than(radius_of_sphere_enclosing_all_children);

	bool p_and_m_are_inside = p_is_inside && m_is_inside;
	bool p_and_m_are_causal = p_is_causal && m_is_causal;

	double causal_solution;

	bool is_inside_cylinder = false;

	if(p_and_m_are_inside) {

		if(p_and_m_are_causal) {

			if(eq->get_minus_solution() >= eq->get_plus_solution()) {
				causal_solution = eq->get_plus_solution(); 
				is_inside_cylinder = true;
			}else{
				causal_solution = eq->get_minus_solution(); 
				is_inside_cylinder = true;
			}
		}else if(p_is_causal) {

			causal_solution = eq->get_plus_solution(); 
			is_inside_cylinder = true;
		}else if(m_is_causal) {

			causal_solution = eq->get_minus_solution(); 
			is_inside_cylinder = true;
		}
	}else if(p_is_inside && p_is_causal) {	
			causal_solution = eq->get_plus_solution(); 
			is_inside_cylinder = true;
	}else if(m_is_inside && m_is_causal) {
			causal_solution = eq->get_minus_solution(); 
			is_inside_cylinder = true;
	}else{
		causal_solution = eq->get_minus_solution();
	}

	if(is_inside_cylinder) {

		Vector3D causal_intersec = ray->PositionOnRay(causal_solution);

		Intersection* intersec;
		intersec = new Intersection(
			this,
			causal_intersec,
			eq->get_surface_normal_given_intersection_vector(&causal_intersec),
			causal_solution,
			ray->Direction()
		);

		return intersec;	
	}

	return empty_intersection();
}