#include "Scenery/Primitive/Cylinder.h"
#include <sstream>
#include "math.h"
using std::string;
using std::stringstream;
using std::vector;

//------------------------------------------------------------------------------
Cylinder::Cylinder() {}
//------------------------------------------------------------------------------
Cylinder::Cylinder(
	const string name,
    const Vec3 pos,
    const Rot3 rot
): SurfaceEntity(name, pos, rot) {}
//------------------------------------------------------------------------------
void Cylinder::set_cylinder(
	const double radius,
	const Vec3 start_pos, 
	const Vec3 end_pos
){
	set_cylinder_radius(radius);
	set_cylinder_length(start_pos, end_pos);
	set_position_and_orientation(start_pos, end_pos);
	post_initialize_radius_of_enclosing_sphere();
}
//------------------------------------------------------------------------------
void Cylinder::set_position_and_orientation(
	const Vec3 start_pos, 
	const Vec3 end_pos
){

	Vec3 rotsym_axis = end_pos - start_pos;
	Vec3 new_position_in_mother = start_pos + rotsym_axis/2.0;
	
	Rot3 rotation_in_mother = 
	calculate_new_rotation_in_mother(rotsym_axis);

	set_name_pos_rot(name, new_position_in_mother, rotation_in_mother);
}
//------------------------------------------------------------------------------
Rot3 Cylinder::calculate_new_rotation_in_mother(
	const Vec3 rotsym_axis
)const{

	Rot3 rotation_in_mother;
	Vec3 ez = Vec3::UNIT_Z;
	
	if( rotsym_axis.is_paralell_to_z_axis() ){

		rotation_in_mother.set(ez,0.0);
	}else{

		Vec3 rot_axis_in_mother = rotsym_axis.cross(ez);
		
		double new_rot_angle_in_rad = -acos( 
			(ez* rotsym_axis)/rotsym_axis.norm()
		);

		rotation_in_mother.set(rot_axis_in_mother, new_rot_angle_in_rad);
	}	
	return rotation_in_mother;
}
//------------------------------------------------------------------------------
void Cylinder::assert_start_and_end_point_are_distinct(
	const Vec3 start_pos, const Vec3 end_pos
)const{

	if( start_pos == end_pos ){
		stringstream info;
		info << "Cylinder::" << __func__ << "()\n";
		info << "The start and end point of a cylinder must not be the same!\n";
		info << "Start: " << start_pos.str() << " and end: " << end_pos.str() << ".\n";
		throw BadStartEndPoints(info.str());
	}
}
//------------------------------------------------------------------------------
void Cylinder::set_radius_and_length(const double radius, const double length){

	set_cylinder_radius(radius);
	set_cylinder_length(length);
	post_initialize_radius_of_enclosing_sphere();
}
//------------------------------------------------------------------------------
void Cylinder::set_cylinder_length(
	const Vec3 start_pos, 
	const Vec3 end_pos
){
	assert_start_and_end_point_are_distinct(start_pos, end_pos);
	set_cylinder_length( (end_pos - start_pos).norm());
}
//------------------------------------------------------------------------------
void Cylinder::set_cylinder_length(const double Length){

	if(Length > 0.0){
		this->Length = Length;
	}else{
		stringstream info;
		info << "Cylinder::set_cylinder\n";
		info << "The length of a cylinder must be larger than 0.0m !\n";
		info << "Expected: >0.0, but actual: " << Length << "\n";
		throw BadLength(info.str());
	}
}
//------------------------------------------------------------------------------
void Cylinder::set_cylinder_radius(const double Radius){

	if(Radius > 0.0) {
		this->Radius = Radius;
	}else{
		stringstream info;
		info << "Cylinder::" << __func__ << "()\n";
		info << "The radius of a cylinder must be larger than 0.0m !\n";
		info << "Expected: >0.0, but actual: " << Radius << "\n";
		throw BadRadius(info.str());
	}
}
//------------------------------------------------------------------------------
void Cylinder::post_initialize_radius_of_enclosing_sphere(){

	double half_the_cylinder_length = 0.5*Length;

	bounding_sphere_radius = hypot(
		half_the_cylinder_length,
		Radius
	);
}
//------------------------------------------------------------------------------
string Cylinder::str()const {
	stringstream out;
	out << SurfaceEntity::str();

	out << "cylinder:\n";
	out << "| radius: " << Radius << "m\n";
	out << "| length: " << Length << "m\n";
	out << "| area:   " << 2.0*Radius*M_PI*Length << "m^2\n";
	return out.str();
}
//------------------------------------------------------------------------------
void Cylinder::calculate_intersection_with(
    const Ray* ray, 
    vector<Intersection> *intersections
)const{

	ZaxisCylinderRayIntersectionEquation cylRayEquation(Radius, ray);

	if(cylRayEquation.has_causal_solution()) {

		double v = cylRayEquation.get_ray_parameter_for_intersection();
		Vec3 intersection_vector = ray->get_pos_at(v);

		if(is_in_cylinders_z_bounds(&intersection_vector)) {

			if(ray->get_support() != intersection_vector) {
				
				intersections->emplace_back(
					this,
					intersection_vector,
					get_surface_normal_for_intersection_vec(&intersection_vector),
					v,
					ray->get_direction()
				);
			}			
		}
	}
}
//------------------------------------------------------------------------------
bool Cylinder::is_in_cylinders_z_bounds(const Vec3* vec)const {
	return fabs(vec->z) <= Length/2.0;
}
//------------------------------------------------------------------------------
Vec3 Cylinder::get_surface_normal_for_intersection_vec(const Vec3* vec)const {
	Vec3 surface_normal(vec->x, vec->y, 0.0);
	return surface_normal / surface_normal.norm();
}