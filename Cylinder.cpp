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
	Vector3D ez; ez.set_unit_vector_z();
	
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
void Cylinder::disp()const{

	std::stringstream out;
	out << "cylinder:" << name_of_frame;
	out << "_________________________________\n";
	out << get_frame_string();
	out << get_surface_propertie_prompt();
	out << get_cylinder_string();
	out << "_________________________________\n";
	std::cout << out.str();
}
//------------------------------------------------------------------------------
std::string Cylinder::get_cylinder_string()const{

	std::stringstream out;
	out << "||| cylinder radius: " << Radius << " m\n";
	out << "||| cylinder length: " << Length << " m\n";
	return out.str();
}
//------------------------------------------------------------------------------
void Cylinder::hit(
	Vector3D *base,
	Vector3D *dir,
	Intersection *intersection
)const{
	// When there is a long cylinder id est 
	// Length >> Radius
	// then max radius is also big but the projected intersection 
	// surface is still small.
	// To speed up the hit calculation we first calculate the 
	// distance of the ray relative to the z-Axis which is the 
	// rotational axis of each cylinder in its own frame.
	
	// g: rotation axis
	// g: p + t*u
	// p=(0,0,0)^T u=(0,0,1)^T
	
	// h: incomming ray
	// h: q + s*v
	// q=base, v=dir
	
	// n = CrossProduct(u,v);
	
	// d = norm((q-p)*n)/norm(n)

	double dbl_denominator =( 	pow(dir->x(),2.0) +
								pow(dir->y(),2.0)
							);

	double dbl_p = 2.0*( 	base->x()*dir->x() + 
							base->y()*dir->y() )/
							dbl_denominator;
							
	double dbl_q =(	pow(base->x(),2.0) +
					pow(base->y(),2.0) -
					pow(Radius,2.0) )/
					dbl_denominator;
	
	double dbl_inner_part_of_square_root_p_q = 
	pow((dbl_p/2.0),2.0) - dbl_q;
			
	//cout<<"dbl_inner_part_of_square_root_p_q ";
	//cout<<dbl_inner_part_of_square_root_p_q<<endl;
	if(dbl_inner_part_of_square_root_p_q>=0.0)
	{
		//cout<<"dbl_inner_part_of_square_root_p_q ";
		//cout<<dbl_inner_part_of_square_root_p_q<<endl;
		//at least one hit
		
		double dbl_lambda_plus = -(dbl_p/2.0) + 
		sqrt(dbl_inner_part_of_square_root_p_q);

		double dbl_lambda_minus= -(dbl_p/2.0) - 
		sqrt(dbl_inner_part_of_square_root_p_q);

		Vector3D vec_intersection_minus;
		vec_intersection_minus = *base +(*dir)*dbl_lambda_minus; 

		Vector3D vec_intersection_plus;
		vec_intersection_plus = *base +(*dir)*dbl_lambda_plus; 

		Vector3D vec_intersection;
		Vector3D vec_surface_normal;
		double dbl_lambda;
		
		bool flag_minus_hit = false;
		//bool flag_plus_hit = false;
		
		if( 	fabs(vec_intersection_minus.z())
				<=(Length/2.0) )
		{
			flag_minus_hit = true;
		}
		
		if( 	fabs(vec_intersection_plus.z())
				<=(Length/2.0) )
		{
			//flag_plus_hit = true;
		}
		
		/*
		bool flag_source_outside = false;
		if(dbl_lambda_minus>=0.0 && dbl_lambda_plus>=0.0){
			flag_source_outside = true;
		}else if(dbl_lambda_minus<0.0 && dbl_lambda_plus>=0.0){
			flag_source_outside = false;
		}else if(dbl_lambda_minus<0.0 && dbl_lambda_plus<0.0){
			flag_source_outside = true;
		}else{
		}*/
		
		//bool flag_hit = false;
		
		if(dbl_lambda_minus>=0.0 && dbl_lambda_plus>=0.0
		&& flag_minus_hit)
		{
			dbl_lambda = dbl_lambda_minus;
			vec_intersection = *base + (*dir)*dbl_lambda;
			vec_surface_normal.set(
			vec_intersection.x(),
			vec_intersection.y(),
			0.0);
			
			// the new intersection feature
			intersection->set_intersection_flag(true);
			intersection->set_pointer_to_intersecting_object(this);
			intersection->set_intersection(
			&vec_intersection_minus,
			&vec_surface_normal,
			&dbl_lambda_plus	);	
		}

	}else{
		// no hit at all
	}
}
