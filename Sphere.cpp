#include "Sphere.h"
//------------------------------------------------------------------------------
Sphere::Sphere(){
}
//------------------------------------------------------------------------------
void Sphere::set_sphere(double new_radius){

	if(new_radius <= 0.0){
		std::stringstream info;
		info << "Sphere::set_sphere\n";
		info << "The radius of a sphere must be larger than 0.0m !\n";
		info << "Expected: >0.0, but actual: " << new_radius << "\n";
		throw TracerException(info.str());
	}
	
	radius = new_radius;
	radius_of_sphere_enclosing_all_children = new_radius;
}
//------------------------------------------------------------------------------
std::string Sphere::get_print()const {
	std::stringstream out;
	out << get_frame_print();
	out << get_surface_print();
	out << get_sphere_print();
	return out.str();
}
//------------------------------------------------------------------------------
std::string Sphere::get_sphere_print()const {
	std::stringstream out;
	out << "sphere:\n";
	out << "| radius: " << radius << "m\n";
	out << "| area:   " << 4.0*M_PI*radius*radius << "m^2\n";	
	return out.str();
}
//------------------------------------------------------------------------------
#include "Ray.h"
#include "Intersection.h"
//------------------------------------------------------------------------------
bool Sphere::facing_sphere_from_outside_given_p_m(
	const double v_Plus,
	const double v_Minus
)const {
	return v_Plus >= 0.0 && v_Minus >= 0.0;
}
//------------------------------------------------------------------------------
bool Sphere::facing_away_from_outside_given_p_m(
	const double v_Plus,
	const double v_Minus
)const {
	return v_Plus < 0.0 && v_Minus < 0.0;
}
//------------------------------------------------------------------------------
QuadraticEquation Sphere::get_ray_parameter_equation_for_intersections_with_sphere(
	const Ray* ray
)const {
	// 
	// 		r = sqrt( x^2 + y^2 + z^2 )
	// 		g:  b + v*d
	// 		put g into sphere eq
	// 		r = sqrt((bx+v*dx)^2 + (by+v*dy)^2 + (bz+v*dz)^2 )
	// 		r = sqrt( v^2 (dd) + v 2(bd) + (bb) )
	// 	    r^2 = v^2 (dd) + v 2(bd) + (bb)
	// 	   	0 = v^2 + v 2(bd/dd) + (bb/dd -r^2)
	//		solve quadrativ eqaution in v

	double sup_times_dir = ray->Support() * ray->Direction();
	double dir_times_dir = ray->Direction() * ray->Direction();
	double sup_times_sup = ray->Support() * ray->Support();
	double radius_square = radius*radius;

	QuadraticEquation quadratic_eq_in_v(
		2*( sup_times_dir / dir_times_dir ),
		sup_times_sup/dir_times_dir - radius_square
	);

	return quadratic_eq_in_v;
}
//------------------------------------------------------------------------------
Intersection* Sphere::sphere_intersection_for_ray_parameter(
	const Ray* ray, 
	const double ray_parameter
)const {

	Vector3D intersection_point = ray->PositionOnRay(ray_parameter);
	Vector3D surface_normal = intersection_point/intersection_point.norm2();
	
	Intersection* intersec;
	intersec = new Intersection(
		this,
		intersection_point,
		surface_normal,
		ray_parameter
	);

	return intersec;
}
//------------------------------------------------------------------------------
Intersection* Sphere::calculate_intersection_with(const Ray* ray)const {

	QuadraticEquation rayParamEqForIntersections = 
		get_ray_parameter_equation_for_intersections_with_sphere(ray);

	if(rayParamEqForIntersections.has_valid_solutions()) {

		double vP = rayParamEqForIntersections.plus_solution();
		double vM = rayParamEqForIntersections.minus_solution();

		if(facing_sphere_from_outside_given_p_m(vP, vM))
			return sphere_intersection_for_ray_parameter(ray,vM);
		else if(!facing_away_from_outside_given_p_m(vP, vM))
			return sphere_intersection_for_ray_parameter(ray, vP);
	}

	return empty_intersection();
}