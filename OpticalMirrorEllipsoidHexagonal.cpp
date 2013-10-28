#include "OpticalMirrorEllipsoidHexagonal.h"
//==================================================================
OpticalMirrorEllipsoidHexagonal::OpticalMirrorEllipsoidHexagonal( ){
	flag_sensor = false;
	//str_regex="ellipsoid_hexag";        
}
//==================================================================
bool OpticalMirrorEllipsoidHexagonal::set_ellipsoid_hexag( 
double new_long_focal_length, 
double new_short_focal_length,
double new_inner_radius_of_hexagonal_shape)
{
	if(new_long_focal_length <= 0.){
		stringstream out;
		out << "OpticalMirrorEllipsoidHexagonal : long_focal_length <= 0. --> ";
		out << new_long_focal_length << endl;
		cout<<out.str();
		return false;
	}else{
		long_focal_length = new_long_focal_length;
	}
	
	if(new_short_focal_length <= 0.){
		stringstream out;
		out << "OpticalMirrorEllipsoidHexagonal : short_focal_length <= 0. --> ";
		out << new_short_focal_length << endl;
		cout<<out.str();
		return false;
	}else{
		short_focal_length = new_short_focal_length;
	}
	
	if(new_inner_radius_of_hexagonal_shape <= 0.){
		stringstream out;
		out << "OpticalMirrorEllipsoidHexagonal :";
		out << "inner_radius_of_hexagonal_shape <= 0. --> ";
		out << new_inner_radius_of_hexagonal_shape << endl;
		cout<<out.str();
		return false;
	}else{
		MirrorRadius_m = new_inner_radius_of_hexagonal_shape;
	}
	
	double mean_focal_length = 
	(long_focal_length + short_focal_length)/2.0;
	
	if(set_optical_hexag(mean_focal_length,MirrorRadius_m)){
		
	}else{
		return false;
	}
	
	// max norm() radius
	// this is a crude guess for thin mirrors
	radius_of_sphere_enclosing_all_children = MirrorRadius_m*1.1;
	return true;
}
//==================================================================
void OpticalMirrorEllipsoidHexagonal::disp(){
	stringstream out;
	out.str("");
	out<<"ellipsoid hexagonal mirror:"<<name_of_frame<<endl;
	out<<"_________________________________"<<endl;
	out<<get_frame_string();
	out<<get_surface_propertie_prompt();
	out<<get_spherical_hexag_string();
	out<<"_________________________________"<<endl;
	cout<<out.str();
}
//==================================================================
string OpticalMirrorEllipsoidHexagonal::get_spherical_hexag_string(){
	stringstream out;
	out<<"||| ellipsoid, hexag mirror:"<<endl;
	out<<"||| long focal length : "<<long_focal_length<<" [m]"<<endl;
	out<<"||| short focal length: "<<short_focal_length<<" [m]"<<endl;
	out<<"||| inner radius of hexagonal shape: ";
	out<<MirrorRadius_m<<" [m]"<<endl;
	return out.str();
}
//==================================================================
void OpticalMirrorEllipsoidHexagonal::hit
(Vector3D *base,Vector3D *dir, Intersection *intersection)const{
/*!
 * A mirror with the shape of an ellipsoid is described by
 * \f{eqnarray*}{ f(x,y,z) &=& 0
 * \f}
 * where 
 * \f{eqnarray*}{ f(x,y,z) &=& 
	\frac{x^2}{A^2} + \frac{y^2}{B^2} + \frac{(z-C)^2}{C^2} - 1
 * \f}
 * So this is a general tri-axial ellipsoid with its center
 * in (0,0,C). Thus f(0,0,0) is exactly zero, as for all the
 * other mirrors as well. \newline
 * 
 * The intersections of this ellipsoid with a general ray given by 
 * \f{eqnarray*}{ ray &=& base + dir \cdot t \f} 
 * or short:
 * \f{eqnarray*}{ \vec{x}(t) &=& \vec{b} + \vec{d} \cdot t \f}  
 * can be expressed as:
 * 
 *  \f{eqnarray*}{ 0 &=& 
 * \frac{(b_x+t \cdot d_x)^2}{A^2} +
 * \frac{(b_y+t \cdot d_y)^2}{B^2} +
 * \frac{(b_z+t \cdot d_z-C)^2}{C^2}
 * \f}  
 * 
 * When solved for t we end up with:
 */
//==================================================================		
// ellipsoid intersection
//==================================================================
// ellipsiod is given by f(x,y,z) = 0 with
// f(x,y,z) = x^2/A^2 + y^2/B^2 + (z-C)^2/C^2 - 1 
// 
// So this is a general tri-axial ellipsoid with its center
// in (0,0,c). Thus f(0,0,0) is exactly zero, as for all the
// other mirrors as well.
// 
// The intersections of this ellipsoid with a general ray given by
// ray = base + dir * t      or short:  b + d * t
// can be expressed as:
//
// 0=((bx+t*dx)^2)/(A^2) +((by+t*dy)^2)/(B^2) +((bz+t*dz-C)^2)/(C^2)
//
// solve for t:
//
// 0=t^2*()
//      p/m stands for plus/minus
// 
// 
// We only want to define a "long" and "short" focal length, so in 
// our case we define:
//  A = 2*short_focal_length
//  B = 2*long_focal_length
//  c = (A+B)/2   just the mean of the two ... since I don't have any better idea.
 

	// I create some short cuts here ... they will all be optimized away
	// by the compiler I guess ... so I don't care!
	double dx = dir->get_x();
	double dy = dir->get_y();
	double dz = dir->get_z();
	double bx = base->get_x();
	double by = base->get_y();
	double bz = base->get_z();
	
	double A = 2.0*short_focal_length;
	double B = 2.0*long_focal_length;
	double C = (A+B)/2;
	
	double a = (dx*dx)/(A*A) + (dy*dy)/(B*B) +  (dz*dz)/(C*C);
	double b = 2.0*( (bx*dx)/(A*A) + (by*dy)/(B*B) + dz*(bz-C)/(C*C) );
	double c = (bx*bx)/(A*A) + (by*by)/(B*B) +  (bz*bz-2.0*bz*C)/(C*C);
	
	
	double dbl_inner_part_of_square_root_p_q = (b*b) - 4.0*a*c;
	
	if(dbl_inner_part_of_square_root_p_q < 0){
		// no hit at all

	}else{
		// hit in ellipsoid

		//intersection parameters
		double tm;
		double tp;
		double t;
		Vector3D vec_intersec_ellipsoid_tp;
		Vector3D vec_intersec_ellipsoid_tm;

		//intersection flags
		bool hit_valid_tp;
		bool hit_valid_tm;
		bool hit_inside_cylinder_flag;
		//intersection vector
		Vector3D vec_intersec_ellipsoid;

		tp = (-b + sqrt(dbl_inner_part_of_square_root_p_q))/(2.0*a);
		tm = (-b - sqrt(dbl_inner_part_of_square_root_p_q))/(2.0*a);
		t=0.0;

		// intersection points of ray and parabola
		vec_intersec_ellipsoid_tp = *base + *dir *tp;
		vec_intersec_ellipsoid_tm = *base + *dir *tm;
		
		//test wether intersections are inside defined zylinder 
		// or not
		hit_valid_tp = false;
		hit_valid_tm = false;
		
		hit_valid_tp = test_hexa_fit(&vec_intersec_ellipsoid_tp);
		hit_valid_tm = test_hexa_fit(&vec_intersec_ellipsoid_tm);
		
		//we do only want to see the lower part of the 
		//ellipsoid and not the part on the top of it.
		//this is a crude approxymation for thin mirrors.
		
		if(vec_intersec_ellipsoid_tm.get_z()>radius_of_sphere_enclosing_all_children)
			hit_valid_tm = false;
		
		if(vec_intersec_ellipsoid_tp.get_z()>radius_of_sphere_enclosing_all_children)
			hit_valid_tp = false;
		
		// we only want to see objects in front of the camera
		// tp and tm must therefore be positiv
		
		if(tm <= 0.0)
			hit_valid_tm = false;
		
		if(tp <= 0.0)
			hit_valid_tp = false;
			
		hit_inside_cylinder_flag = false;
		if(hit_valid_tp && hit_valid_tm)
		{
			// the tp solution is covered by the tm solution
			t=tm; 
			hit_inside_cylinder_flag=true;
		}else if(hit_valid_tp)
		{	
			//tp is valid only
			t=tp; 
			hit_inside_cylinder_flag=true;
		}else if(hit_valid_tm){
			//tm is valid only
			t=tm; 
			hit_inside_cylinder_flag=true;
		}else{
			//no hit is valid
			return;
		}
		
		if(hit_inside_cylinder_flag){ 
			vec_intersec_ellipsoid = *base + *dir *t;
			// surface normal is given as  ( -dz/dx , -dz/dy , 1 )
			// 
			// z(x,y) = C*sqrt(1-x^2/A^2-y^2/B^2)+C
			//
			// dz/dx = C*1/2*(1-x^2/A^2-y^2/B^2)^(-1/2) * (-2*x/A^2)
			//
			// dz/dy = C*1/2*(1-x^2/A^2-y^2/B^2)^(-1/2) * (-2*y/A^2)
			//
			// normal = ( -dz/dx , -dz/dy , 1 )
			//
			// dbl_surface_normal_factor = 
			// C*1/2*(1-x^2/A^2-y^2/B^2)^(-1/2) 

			double dbl_surface_normal_factor = 
			C*(0.5)*pow(
					1.0
					-pow(vec_intersec_ellipsoid.get_x(),2.0)/(A*A)
					-pow(vec_intersec_ellipsoid.get_y(),2.0)/(B*B)
					,0.5);
					
			Vector3D surface_normal;
			surface_normal.set_vec3D(
			-dbl_surface_normal_factor*
			(2.0*vec_intersec_ellipsoid.get_x()/(A*A)),
			-dbl_surface_normal_factor*
			(2.0*vec_intersec_ellipsoid.get_y()/(B*B)),
			1.0);
	
			surface_normal = surface_normal/surface_normal.norm2();

			// the new intersection feature
			intersection->set_intersection_flag(true);
			intersection->set_pointer_to_intersecting_object(this);
			intersection->set_intersection(
				&vec_intersec_ellipsoid,
				&surface_normal,
				&t
			);
		}
	}
	
	/*
	
	if(discriminant >= 0){
		
		double tp = (-b + sqrt(discriminant))/(2.0*a);
		double tm = (-b - sqrt(discriminant))/(2.0*a);
		// if all hits are behind us, we can already return... nothing to do.
		if ( tm < 0. && tm < 0.) return;
		
		// intersection points of ray and surface
		Vector3D vec_intersec_sphere_tp = *base + *dir *tp;
		Vector3D vec_intersec_sphere_tm = *base + *dir *tm;
		
		//test wether intersections are inside defined cylinder or not
		bool hit_valid_tp = test_hexa_fit(&vec_intersec_sphere_tp);
		bool hit_valid_tm = test_hexa_fit(&vec_intersec_sphere_tm);
		
		//if( hit_valid_tp) cout<<"hit_valid_tp is TRUE"<<endl;
		//if( hit_valid_tm) cout<<"hit_valid_tm is TRUE"<<endl;
		
		double t;
		if(hit_valid_tp && hit_valid_tm){
			//cout<<"hit_valid_tp && hit_valid_tm"<<endl; 
			if(tp > 0.0 && tm <0.0){
				t=tp;
			}else if(tm >0.0 && tp <0.0){
				t=tm;
			}else if(tm >0.0 && tp > 0.0){
				t = tm;
			}
			//t = max(tp,tm);
		}else if(hit_valid_tp && tp > 0.0){
			//cout<<"hit_valid_tp && tp > 0."<<endl; 
			t = tp;
		}else if(hit_valid_tm && tm > 0.0){
			//cout<<"hit_valid_tm && tm > 0.0"<<endl; 
			t = tm;
		}else{
			// nothing was hit .. so we simply return here... 
			//cout<<"nothing was hit .."<<endl;  
			return;
		} 

		Vector3D vec_intersec_sphere = *base + *dir *t;
		
		 // surface normal is given as grad f(x,y,z)
		 // 
		 // In our case of an ellipsoid grad f(x,y,z) is:
		 // grad f(x,y,z) = ( 2x/A^2 , 2y/B^2 , 2(z-C)/C^2 )
		  // 
		 // so we can express n  = grad f / |grad f|
		  //
		  
		Vector3D surface_normal;
		surface_normal.set_vec3D(
			vec_intersec_sphere.get_x()/(A*A),
			vec_intersec_sphere.get_y()/(B*B),
			(vec_intersec_sphere.get_z() - C) / (C*C) 
								);
								
		surface_normal = surface_normal/surface_normal.norm2();
		
		// the new intersection feature
		intersection->set_intersection_flag(true);
		intersection->set_pointer_to_intersecting_object(this);
		intersection->set_intersection(
			&vec_intersec_sphere,
			&surface_normal,
			&t
		);
	}
	*/
}
