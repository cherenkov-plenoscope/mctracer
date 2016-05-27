#include "SurfaceWithOuterPrismBound.h"
//------------------------------------------------------------------------------
void SurfaceWithOuterPrismBound::add_causeal_intersection(
		const TwoSolutionSurfaceRayEquation* eq,
		const PrismZ* outer_bound,
		const Ray *ray,
		vector<Intersection> *intersections
)const {

	const Vec3 plus_intersec = ray->get_pos_at(eq->get_plus_solution());
	const Vec3 minus_intersec = ray->get_pos_at(eq->get_minus_solution()); 	

	const bool p_is_inside = outer_bound->is_inside(&plus_intersec);
	const bool m_is_inside = outer_bound->is_inside(&minus_intersec);

	const bool p_is_causal = eq->get_plus_solution() > 0.0 && plus_intersec.
		norm_is_less_equal_than(radius_of_sphere_enclosing_all_children);

	const bool m_is_causal = eq->get_minus_solution() > 0.0 &&	minus_intersec.
		norm_is_less_equal_than(radius_of_sphere_enclosing_all_children);

	const bool p_and_m_are_inside = p_is_inside && m_is_inside;
	const bool p_and_m_are_causal = p_is_causal && m_is_causal;

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

		Vec3 causal_intersec = ray->get_pos_at(causal_solution);

		if(ray->get_support() != causal_intersec) {
			
			intersections->push_back(
				Intersection(
					this,
					causal_intersec,
					eq->get_surface_normal_given_intersection_vector(&causal_intersec),
					causal_solution,
					ray->get_direction()
				)
			);
		}
	}
}