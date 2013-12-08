#include "Cylinder.h"
//==================================================================
Cylinder::Cylinder(){
	flag_sensor = false;
	//str_regex = "cylinder";
}
//==================================================================
bool Cylinder::set_cylinder(	
double new_CylinderRadius,
Vector3D vec_start, 
Vector3D vec_end){
	//cout<<"start: "<<vec_start.get_string()<<endl;
	//cout<<"end  : "<<vec_end.get_string()<<endl;
	//===================
	// rotation axis in world frame
	//===================
	Vector3D vec_rotsym_axis_in_world_frame;
	vec_rotsym_axis_in_world_frame = vec_end - vec_start;
	
	//===================
	// set cylinder radius
	//===================
	if(new_CylinderRadius > 0.0){
		CylinderRadius=new_CylinderRadius;
	}else{
		cout<<"set_cylinder -> cylinder radius: ";
		cout<<new_CylinderRadius<<">0.0";
		cout<<" is not true!"<<endl;
		return false;
	}
	//===================
	// set cylinder length
	//===================
	double dbl_new_cylinder_length;
	dbl_new_cylinder_length = vec_rotsym_axis_in_world_frame.norm2();
	CylinderLength = dbl_new_cylinder_length;
	
	//===================
	// new world position
	//===================
	Vector3D vec_new_pos;
	vec_new_pos = vec_start + vec_rotsym_axis_in_world_frame/2.0;
	
	//===================
	// new rotation
	//===================
	// rot axis
	Rotation3D euler_new_rot;
	Vector3D vec_ez; vec_ez.set_unit_vector_z();
	
	if(
	fabs(vec_ez*vec_rotsym_axis_in_world_frame/
	vec_rotsym_axis_in_world_frame.norm2())== 1.0)
	{
		euler_new_rot.set_rotation(vec_ez,0.0);
	}
	else
	{
		Vector3D vec_rot_axis=
		vec_rotsym_axis_in_world_frame.cross_product(vec_ez);
		//cout<<"rot axis: "<<vec_rot_axis.get_string()<<endl;
		// rot angle
		
		double dbl_new_rot_angle_in_rad;
		dbl_new_rot_angle_in_rad = -acos(
		(vec_ez* vec_rotsym_axis_in_world_frame)/ 
		vec_rotsym_axis_in_world_frame.norm2()
		);
		//cout<<"rot angle: "<<dbl_new_rot_angle_in_rad<<endl;
		
		euler_new_rot.
		set_rotation(vec_rot_axis,dbl_new_rot_angle_in_rad);
	}
	//===================
	// reset frame
	//===================
	set_frame(name_of_frame,vec_new_pos, euler_new_rot);
	
	//===================
	// set max radius
	//===================
	radius_of_sphere_enclosing_all_children = sqrt(
		pow(CylinderRadius,2.0)+
		pow((CylinderLength/2.0),2.0)
	);
	
	return true;
}
//==================================================================
bool Cylinder::set_cylinder(
double new_CylinderRadius,
double new_CylinderLength){
	//===================
	// set cylinder radius
	//===================
	if(new_CylinderRadius > 0.0){
		CylinderRadius=new_CylinderRadius;
	}else{
		cout<<"set_cylinder -> cylinder radius: ";
		cout<<new_CylinderRadius<<">0.0";
		cout<<" is not true!"<<endl;
		return false;
	}
	//===================
	// set cylinder length
	//===================
	if(new_CylinderLength > 0.0){
		CylinderLength=new_CylinderLength;
	}else{
		cout<<"set_cylinder -> cylinder length: ";
		cout<<new_CylinderLength<<">0.0";
		cout<<" is not true!"<<endl;
		return false;
	}
	//===================
	// set max radius
	//===================
	radius_of_sphere_enclosing_all_children = sqrt(
		pow(CylinderRadius,2.0)+
		pow((CylinderLength/2.0),2.0)
	);
	return true;
}
//==================================================================
void Cylinder::disp()const{
	stringstream out;
	out.str("");
	out<<"cylinder:"<<name_of_frame<<"_________________________________"<<endl;
	out<<get_frame_string();
	out<<get_surface_propertie_prompt();
	out<<get_cylinder_string();
	out<<"_________________________________"<<endl;
	cout<<out.str();
}
//==================================================================
string Cylinder::get_cylinder_string()const{
	stringstream out;
	out.str("");
	out<<"||| cylinder radius: "<<CylinderRadius<<" [m]"<<endl;
	out<<"||| cylinder length: "<<CylinderLength<<" [m]"<<endl;
	return out.str();
}
//==================================================================
void Cylinder::hit(
Vector3D *base,
Vector3D *dir,
Intersection *intersection)const{
	// When there is a long cylinder id est 
	// CylinderLength >> CylinderRadius
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
	
	// n = cross_product(u,v);
	
	// d = norm((q-p)*n)/norm(n)

	double dbl_denominator =( 	pow(dir->get_x(),2.0) +
								pow(dir->get_y(),2.0)
							);

	double dbl_p = 2.0*( 	base->get_x()*dir->get_x() + 
							base->get_y()*dir->get_y() )/
							dbl_denominator;
							
	double dbl_q =(	pow(base->get_x(),2.0) +
					pow(base->get_y(),2.0) -
					pow(CylinderRadius,2.0) )/
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
		
		if( 	fabs(vec_intersection_minus.get_z())
				<=(CylinderLength/2.0) )
		{
			flag_minus_hit = true;
		}
		
		if( 	fabs(vec_intersection_plus.get_z())
				<=(CylinderLength/2.0) )
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
			vec_surface_normal.set_vec3D(
			vec_intersection.get_x(),
			vec_intersection.get_y(),
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
