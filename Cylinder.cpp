#include "Cylinder.h"
//------------------------------------------------------------------------------
void Cylinder::set_cylinder(
	const double radius,
	const Vector3D start_pos, 
	const Vector3D end_pos
){

	set_cylinder_radius(radius);
	set_cylinder_length(start_pos, end_pos);
	set_position_and_orientation(start_pos, end_pos);
	post_initialize_radius_of_enclosing_sphere();
}
//------------------------------------------------------------------------------
void Cylinder::set_position_and_orientation(
	const Vector3D start_pos, 
	const Vector3D end_pos
){

	Vector3D rotsym_axis = end_pos - start_pos;
	Vector3D new_position_in_mother = start_pos + rotsym_axis/2.0;
	
	Rotation3D rotation_in_mother = 
	calculate_new_rotation_in_mother(rotsym_axis);

	set_frame(name_of_frame, new_position_in_mother, rotation_in_mother);
}
//------------------------------------------------------------------------------
Rotation3D Cylinder::calculate_new_rotation_in_mother(
	const Vector3D rotsym_axis
)const{

	Rotation3D rotation_in_mother;
	Vector3D ez = Vector3D::unit_z;
	
	if( rotsym_axis.is_paralell_to_z_axis() ){

		rotation_in_mother.set(ez,0.0);
	}else{

		Vector3D rot_axis_in_mother = rotsym_axis.CrossProduct(ez);
		
		double new_rot_angle_in_rad = -acos( 
			(ez* rotsym_axis)/rotsym_axis.norm2()
		);

		rotation_in_mother.set(rot_axis_in_mother, new_rot_angle_in_rad);
	}	
	return rotation_in_mother;
}
//------------------------------------------------------------------------------
void Cylinder::assert_start_and_end_point_are_distinct(
	const Vector3D start_pos, const Vector3D end_pos
)const{

	if( start_pos == end_pos ){
		std::stringstream info;
		info << "Cylinder::" << __func__ << "()\n";
		info << "The start and end point of a cylinder must not be the same!\n";
		info << "Start: " << start_pos << " and end: " << end_pos << ".\n";
		throw TracerException(info.str());
	}
}
//------------------------------------------------------------------------------
void Cylinder::set_cylinder(const double radius, const double length){

	set_cylinder_radius(radius);
	set_cylinder_length(length);
	post_initialize_radius_of_enclosing_sphere();
}
//------------------------------------------------------------------------------
void Cylinder::set_cylinder_length(
	const Vector3D start_pos, 
	const Vector3D end_pos
){
	assert_start_and_end_point_are_distinct(start_pos, end_pos);
	set_cylinder_length( (end_pos - start_pos).norm2());
}
//------------------------------------------------------------------------------
void Cylinder::set_cylinder_length(const double Length){

	if(Length > 0.0){
		this->Length = Length;
	}else{
		std::stringstream info;
		info << "Cylinder::set_cylinder\n";
		info << "The length of a cylinder must be larger than 0.0m !\n";
		info << "Expected: >0.0, but actual: " << Length << "\n";
		throw TracerException(info.str());
	}
}
//------------------------------------------------------------------------------
void Cylinder::set_cylinder_radius(const double Radius){

	if(Radius > 0.0){
		this->Radius = Radius;
	}else{
		std::stringstream info;
		info << "Cylinder::" << __func__ << "()\n";
		info << "The radius of a cylinder must be larger than 0.0m !\n";
		info << "Expected: >0.0, but actual: " << Radius << "\n";
		throw TracerException(info.str());
	}
}
//------------------------------------------------------------------------------
void Cylinder::post_initialize_radius_of_enclosing_sphere(){

	double half_the_cylinder_length = 0.5*Length;

	radius_of_sphere_enclosing_all_children = hypot(
		half_the_cylinder_length,
		Radius
	);
}
//------------------------------------------------------------------------------
std::string Cylinder::get_print()const {
	std::stringstream out;
	out << get_frame_print();
	out << get_surface_print();
	out << get_cylinder_print();
	return out.str();
}
//------------------------------------------------------------------------------
std::string Cylinder::get_cylinder_print()const {

	std::stringstream out;
	out << "cylinder:\n";
	out << "| radius: " << Radius << "m\n";
	out << "| length: " << Length << "m\n";
	out << "| area:   " << 2.0*Radius*M_PI*Length << "m^2\n";
	return out.str();
}
//------------------------------------------------------------------------------
const Intersection* Cylinder::calculate_intersection_with(const Ray* ray)const {
	ZaxisCylinderRayIntersectionEquation cylRayEquation(Radius, ray);

	if(cylRayEquation.has_causal_solution()) {

		double v = cylRayEquation.get_ray_parameter_for_intersection();
		Vector3D intersection_vector = ray->PositionOnRay(v);

		if(is_in_cylinders_z_bounds(&intersection_vector)) {

			Intersection* intersec;
			intersec = new Intersection(
				this,
				intersection_vector,
				get_surface_normal_for_intersection_vec(&intersection_vector),
				v,
				ray->Direction()
			);

			return intersec;			
		}

	}
	return empty_intersection();
}
//------------------------------------------------------------------------------
bool Cylinder::is_in_cylinders_z_bounds(const Vector3D* vec)const {
	return fabs(vec->z()) <= Length/2.0;
}
//------------------------------------------------------------------------------
Vector3D Cylinder::get_surface_normal_for_intersection_vec(const Vector3D* vec)const {
	Vector3D surface_normal(vec->x(), vec->y(), 0.0);
	return surface_normal / surface_normal.norm2();
}