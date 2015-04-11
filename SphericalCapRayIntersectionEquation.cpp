#include "SphericalCapRayIntersectionEquation.h"
//------------------------------------------------------------------------------
SphericalCapRayIntersectionEquation::SphericalCapRayIntersectionEquation(
	const double radius, 
	const Ray* ray
) {
	this->radius = radius;
	
	// sphere intersection with ray
	// z(x,y) = R - sqrt(R^2 -x^2 -y^2) |- R, *(-1)
	// R - z  = sqrt(R^2 -x^2 -y^2)     |pow2
	// (R-z)^2= R^2 - x^2 - y^2         |+x^2 +y^2
	// R^2 -2Rz +z^2 +y^2 +x^2 = R^2    |-R^2
	// x^2 + y^2 + z^2 -2Rz = 0         | x=sx+v*dx, y=sy+v*dy, z=sz+v*dz
	// v^2*(d*d) + v*2(s*d -R*dz) +s*s -2*R*sz = 0
	// v^2 + v 2(s*d - R*dz)/(d*d) + (s*s - 2*R*sz)/(d*d) = 0
	
	const double dir_times_dir = ray->Direction() * ray->Direction();
	const double sup_times_sup = ray->Support() * ray->Support();
	const double sup_times_dir = ray->Support() * ray->Direction();
	const double R_times_dir_z = radius * ray->Direction().z();
	const double R_times_sup_z = radius * ray->Support().z();

	const double P = 2.0 * ( sup_times_dir - R_times_dir_z ) / dir_times_dir;
	const double Q = ( sup_times_sup - 2.0*R_times_sup_z ) / dir_times_dir;

	QuadraticEquation RayParamForSphereCapIntersecEq(P, Q);

	if(RayParamForSphereCapIntersecEq.has_valid_solutions()) {

		_has_solutions = true;

		plus_solution = RayParamForSphereCapIntersecEq.plus_solution();
		minus_solution = RayParamForSphereCapIntersecEq.minus_solution();
	}
}
//------------------------------------------------------------------------------
bool SphericalCapRayIntersectionEquation::has_solutions()const {
	return _has_solutions;
}
//------------------------------------------------------------------------------
double SphericalCapRayIntersectionEquation::get_minus_solution()const {
	return minus_solution;
}
//------------------------------------------------------------------------------
double SphericalCapRayIntersectionEquation::get_plus_solution()const{
	return plus_solution;
}
//------------------------------------------------------------------------------
Vector3D SphericalCapRayIntersectionEquation::get_surface_normal_given_intersection_vector(
	const Vector3D* intersec
)const {
	// surface normal on a sphere
	// z(x,y)= R -( R^2 -x^2 -y^2 )^1/2
	// dz/dx = 1/2( R^2 -x^2 -y^2 )^-1/2 * (-2x)
	// dz/dy = 1/2( R^2 -x^2 -y^2 )^-1/2 * (-2y)
	
	double surf_norm_factor = -pow(
		radius*radius
		-intersec->x()*intersec->x()
		-intersec->y()*intersec->y(),
		-0.5
	);
	
	Vector3D surface_normal(
		intersec->x()*surf_norm_factor,
		intersec->y()*surf_norm_factor,
		1.0
	);

	surface_normal = surface_normal/surface_normal.norm();
	return surface_normal;
}