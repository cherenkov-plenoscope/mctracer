#include "Triangle.h"
//------------------------------------------------------------------------------
void Triangle::set_corners_in_xy_plane(
	const double Ax, const double Ay,
	const double Bx, const double By,  
	const double Cx, const double Cy
) {
	A = Vector3D(Ax, Ay, 0.0);
	B = Vector3D(Bx, By, 0.0);
	C = Vector3D(Cx, Cy, 0.0);
	
	assert_edge_length_is_non_zero((A-B).norm(), "AB");
	assert_edge_length_is_non_zero((C-A).norm(), "AC");
	assert_edge_length_is_non_zero((C-B).norm(), "CB");

	post_initialize_radius_of_enclosing_sphere();
}
//------------------------------------------------------------------------------
void Triangle::set_normal_and_3_vertecies(
	const Vector3D normal,
	Vector3D a,
	Vector3D b,
	Vector3D c
) {
	// correct for offset
	Vector3D pos = (a + b + c)/3.0;
	a = a - pos;
	b = b - pos;
	c = c - pos;

	Rotation3D rot;

	if(normal != Vector3D::unit_z) {

		// transformation to make surface normal match z-axis
		Vector3D rot_axis = Vector3D::unit_z.cross(normal);
		double rotation_angle = Vector3D::unit_z.get_angle_in_between_in_rad(normal);

		rot = Rotation3D(rot_axis, rotation_angle);

		HomoTrafo3D trafo;
		trafo.set_transformation(rot, Vector3D::null);
		
		HomoTrafo3D trafo_inv = trafo.inverse();
		// transform a b c
		a = trafo_inv.get_transformed_orientation(a);
		b = trafo_inv.get_transformed_orientation(b);
		c = trafo_inv.get_transformed_orientation(c);
	}else{
		rot = Rotation3D::null;
	}

	set_name_pos_rot(name_of_frame, pos, rot);

	set_corners_in_xy_plane(
		a.x(), a.y(),
		b.x(), b.y(),
		c.x(), c.y()
	);
}
//------------------------------------------------------------------------------
void Triangle::assert_edge_length_is_non_zero(
	const double edge_length, 
	const std::string edge
)const {
	if(edge_length <= 0.0) {
		std::stringstream out;
		out << "Triangle::" << __func__ <<"():\n";
		out << "Expected edge length of " << edge << " > 0.0, but actual: ";
		out << "edge length of " << edge << " = " << edge_length << "m.\n";
		throw TracerException(out.str());
	}
}
//------------------------------------------------------------------------------
void Triangle::post_initialize_radius_of_enclosing_sphere() {

	std::vector<double> dist_corner_to_base;
	
	dist_corner_to_base.push_back( A.norm() );
	dist_corner_to_base.push_back( B.norm() );
	dist_corner_to_base.push_back( C.norm() );

	radius_of_sphere_enclosing_all_children = *max_element(	
		dist_corner_to_base.begin(),
		dist_corner_to_base.end() 
	);
}
//------------------------------------------------------------------------------
std::string Triangle::get_print()const{

	std::stringstream out;
	out << SurfaceEntity::get_print();

	out << "triangle:\n";
	out << "| A: " << A << "\n";
	out << "| B: " << B << "\n";
	out << "| C: " << C << "\n";
	return out.str();	
}
//------------------------------------------------------------------------------
double Triangle::sign(
	const Vector3D &P1, const Vector3D &P2, const Vector3D &P3
)const {
    return (P1.x()-P3.x()) * (P2.y()-P3.y())-(P2.x()-P3.x()) * (P1.y()-P3.y());
}
//------------------------------------------------------------------------------
bool Triangle::is_inside_triangle(const Vector3D &intersec_vec)const {
    bool bA, bB, bC;

    bA = sign(intersec_vec, A, B) < 0.0;
    bB = sign(intersec_vec, B, C) < 0.0;
    bC = sign(intersec_vec, C, A) < 0.0;

    return ((bA == bB) && (bB == bC));
}
//------------------------------------------------------------------------------
#include "Core/Ray.h"
#include "Core/Intersection.h"
//------------------------------------------------------------------------------
const Intersection* Triangle::calculate_intersection_with(const Ray* ray)const {

	XyPlaneRayIntersectionEquation xyPlaneRayEquation(ray);

	if( xyPlaneRayEquation.has_causal_solution() ){
		
		double v = xyPlaneRayEquation.get_ray_parameter_for_intersection();
		Vector3D intersection_vector = ray->PositionOnRay(v);		

		if(	is_inside_triangle(intersection_vector) ) {

			Intersection* intersec;
			intersec = new Intersection(
				this,
				intersection_vector,
				xyPlaneRayEquation.get_plane_normal_vector(),
				v,
				ray->Direction()
			);

			return intersec;
		}
	}
	return empty_intersection();	
}