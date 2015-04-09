#include "OpticalMirrorEllipsoidHexagonal.h"
//------------------------------------------------------------------------------
OpticalMirrorEllipsoidHexagonal::OpticalMirrorEllipsoidHexagonal( ){      
}
//------------------------------------------------------------------------------
void OpticalMirrorEllipsoidHexagonal::set_ellipsoid_hexag( 
	double long_focal_length, 
	double short_focal_length,
	double inner_radius_of_hexagonal_shape
) {
	assert_named_variable_is_positiv(long_focal_length,"long focal length");
	this->long_focal_length = long_focal_length;
	
	assert_named_variable_is_positiv(short_focal_length,"short focal length");
	this->short_focal_length = short_focal_length;

	assert_named_variable_is_positiv(inner_radius_of_hexagonal_shape,"inner hexagon radius");
	this->MirrorRadius_m = inner_radius_of_hexagonal_shape;
	
	double mean_focal_length = 
	(long_focal_length + short_focal_length)/2.0;
	
	set_optical_hexag(mean_focal_length,MirrorRadius_m);

	// max norm() radius
	// this is a crude guess for thin mirrors
	radius_of_sphere_enclosing_all_children = MirrorRadius_m*1.1;
}
//------------------------------------------------------------------------------
void OpticalMirrorEllipsoidHexagonal::assert_named_variable_is_positiv(
	const double variable_to_test, const std::string name_of_variable
)const{
	if(variable_to_test <= 0.0) {
		std::stringstream info;
		info << "OpticalMirrorEllipsoidHexagonal\n";
		info << "The '" << name_of_variable << "' must be positiv!\n";
		info << "Expected: >0.0, but actual: " << variable_to_test << "\n";
		throw TracerException(info.str());
	}
}
//------------------------------------------------------------------------------
void OpticalMirrorEllipsoidHexagonal::disp() {
	stringstream out;
	out << "ellipsoid hexagonal mirror:" << name_of_frame << "\n";
	out << "_________________________________\n";
	out << get_frame_print();
	out << get_surface_print();
	out << get_ellipsoid_hexag_print();
	out << "_________________________________\n";
	cout << out.str();
}
//------------------------------------------------------------------------------
std::string OpticalMirrorEllipsoidHexagonal::get_ellipsoid_hexag_print()const {
	stringstream out;
	out << "||| ellipsoid, hexag mirror:\n";
	out << "||| long focal length : " << long_focal_length << " m\n";
	out << "||| short focal length: " << short_focal_length << " m\n";
	out << "||| inner radius of hexagonal shape: " << MirrorRadius_m << " m\n";
	return out.str();
}
//------------------------------------------------------------------------------
void OpticalMirrorEllipsoidHexagonal::hit(
	Vector3D *base,Vector3D *dir, Intersection *intersection
)const{
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
	double dx = dir->x();
	double dy = dir->y();
	double dz = dir->z();
	double bx = base->x();
	double by = base->y();
	double bz = base->z();
	
	double A = 2.0*short_focal_length;
	double B = 2.0*long_focal_length;
	double C = (A+B)/2;
	
	double a = (dx*dx)/(A*A) + (dy*dy)/(B*B) +  (dz*dz)/(C*C);
	double b = 2.0*( (bx*dx)/(A*A) + (by*dy)/(B*B) + dz*(bz-C)/(C*C) );
	double c = (bx*bx)/(A*A) + (by*by)/(B*B) +  (bz*bz-2.0*bz*C)/(C*C);
	
	
	double inner_part_of_square_root_p_q = (b*b) - 4.0*a*c;
	
	if(inner_part_of_square_root_p_q < 0){
		// no hit at all
		return;
	}else{
		// hit in ellipsoid

		//intersection parameters
		double tm;
		double tp;
		double t;

		Vector3D intersec_ellipsoid_tp;
		Vector3D intersec_ellipsoid_tm;

		//intersection flags
		bool hit_valid_tp;
		bool hit_valid_tm;
		bool hit_inside_cylinder_flag;
		//intersection vector
		Vector3D intersec_ellipsoid;

		tp = (-b + sqrt(inner_part_of_square_root_p_q))/(2.0*a);
		tm = (-b - sqrt(inner_part_of_square_root_p_q))/(2.0*a);
		t=0.0;

		// intersection points of ray and parabola
		intersec_ellipsoid_tp = *base + *dir *tp;
		intersec_ellipsoid_tm = *base + *dir *tm;
		
		//test wether intersections are inside defined zylinder 
		// or not
		hit_valid_tp = false;
		hit_valid_tm = false;
		
		hit_valid_tp = test_hexa_fit(&intersec_ellipsoid_tp);
		hit_valid_tm = test_hexa_fit(&intersec_ellipsoid_tm);
		
		//we do only want to see the lower part of the 
		//ellipsoid and not the part on the top of it.
		//this is a crude approxymation for thin mirrors.
		
		if(intersec_ellipsoid_tm.z()>radius_of_sphere_enclosing_all_children)
			hit_valid_tm = false;
		
		if(intersec_ellipsoid_tp.z()>radius_of_sphere_enclosing_all_children)
			hit_valid_tp = false;
		
		// we only want to see objects in front of the camera
		// tp and tm must therefore be positiv
		
		if(tm <= 0.0)
			hit_valid_tm = false;
		
		if(tp <= 0.0)
			hit_valid_tp = false;
			
		hit_inside_cylinder_flag = false;
		if(hit_valid_tp && hit_valid_tm){
			// the tp solution is covered by the tm solution
			t=tm; 
			hit_inside_cylinder_flag=true;
		}else if(hit_valid_tp){	
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
			intersec_ellipsoid = *base + *dir *t;
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
			// surface_normal_factor = 
			// C*1/2*(1-x^2/A^2-y^2/B^2)^(-1/2) 

			double surface_normal_factor = 
			C*(0.5)*pow(
					1.0
					-pow(intersec_ellipsoid.x(),2.0)/(A*A)
					-pow(intersec_ellipsoid.y(),2.0)/(B*B)
					,0.5
					);
					
			Vector3D surface_normal;
			surface_normal.set(
			-surface_normal_factor*
			(2.0*intersec_ellipsoid.x()/(A*A)),
			-surface_normal_factor*
			(2.0*intersec_ellipsoid.y()/(B*B)),
			1.0);
	
			surface_normal = surface_normal/surface_normal.norm();

			// the new intersection feature
			intersection->set_intersection_flag(true);
			intersection->set_pointer_to_intersecting_object(this);
			intersection->set_intersection(
				&intersec_ellipsoid,
				&surface_normal,
				&t
			);
		}
	}
}
