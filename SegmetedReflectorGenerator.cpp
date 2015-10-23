#include "SegmetedReflectorGenerator.h"
#include "Core/Function/ConstantFunction.h"
//------------------------------------------------------------------------------
void SegmetedReflectorGenerator::init_facet_surface() {
	mirror_colour = new Color(255,255,255);
	outer_mirror_reflection = new Function::Constant(0.8, Function::Limits(200e-9, 1200e-9));
	inner_mirror_colour = new Color(64,64,64);
}
//------------------------------------------------------------------------------
void SegmetedReflectorGenerator::set_hybrid_geometry(const double alpha){
	AssertionTools::value_with_name_is_in_min_max_given_context(
		alpha, 
		"DaviesCotton_to_parabolic_mixing_factor",
		0.0,
		1.0,
		"SegmetedReflectorGenerator::set_hybrid_geometry()\n"
		"0.0 is Davies Cotton and 1.0 is parabolic geometry." 
	);
	DaviesCotton_to_parabolic_mixing_factor = alpha;
}
//------------------------------------------------------------------------------
void SegmetedReflectorGenerator::set_focal_length(const double _focal_length) {
	AssertionTools::value_with_name_is_greater_zero_given_context(
		_focal_length,
		"focal_length",
		"SegmetedReflectorGenerator::set_focal_length()\n"
		"Parallel incoming light is focused in this distance."
	);
	focal_length = _focal_length;
}
//------------------------------------------------------------------------------
void SegmetedReflectorGenerator::set_facet_spacing(const double _facet_spacing) {
	AssertionTools::value_with_name_is_greater_zero_given_context(
		_facet_spacing,
		"facet_spacing",
		"SegmetedReflectorGenerator::set_facet_spacing()\n"
		"The distance between mirror facet centers in x-y-plane."
	);
	facet_spacing = _facet_spacing;
}
//------------------------------------------------------------------------------
void SegmetedReflectorGenerator::set_max_outer_diameter(const double _max_outer_diameter) {
	AssertionTools::value_with_name_is_greater_zero_given_context(
		_max_outer_diameter,
		"max_outer_diameter",
		"SegmetedReflectorGenerator::set_max_outer_diameter()\n"
		"Max outer reflector diameter."
	);
	max_outer_diameter = _max_outer_diameter;
}
//------------------------------------------------------------------------------
void SegmetedReflectorGenerator::set_min_inner_diameter(const double _min_inner_diameter) {
	AssertionTools::value_with_name_is_greater_zero_given_context(
		_min_inner_diameter,
		"min_inner_diameter",
		"SegmetedReflectorGenerator::set_min_inner_diameter()\n"
		"Min inner reflector diameter."
	);
	min_inner_diameter = _min_inner_diameter;
}
//------------------------------------------------------------------------------
Frame* SegmetedReflectorGenerator::get_reflector() {
	init_focal_point();
	init_facet_xy_positions();
	init_facet_z_positions();
	optimize_reflector_z_pos();
	init_facet_orientations();
	init_facet_surface();
	init_facet_radius();
	init_facets();
	init_reflector();

	return reflector;
}
//------------------------------------------------------------------------------
std::string SegmetedReflectorGenerator::get_print()const {
	std::stringstream out;
	out << "SegmetedReflectorGenerator:\n";
	out << "focal_length: " << focal_length << "m\n";
	out << "number of facets: " << facet_positions.size() << "\n";
	out << "max Aperture diameter: " << max_outer_diameter << "\n";
	out << "Hybrid: ";
	out << DaviesCotton_to_parabolic_mixing_factor << " Davies Cotton + ";
	out << 1.0 - DaviesCotton_to_parabolic_mixing_factor << " Parabolid\n";

	for(Frame* facet : facets) {
		out << "___" << facet->get_name_of_frame() << "___\n";
		out << "  pos: " << *facet->get_position_in_mother() << "\n";
		out << "  rot: " << *facet->get_rotation_in_mother() << "\n";
		out << "  bounding radius: " << facet->get_radius_of_sphere_enclosing_all_children() << "\n";
	}
	return out.str();
}
//------------------------------------------------------------------------------
void SegmetedReflectorGenerator::init_facet_xy_positions() {

	const double max_reflector_radius = max_outer_diameter/2.0;
	const double min_reflector_radius = min_inner_diameter/2.0;

	HexGridXy hex_grid(
		max_reflector_radius, 
		min_reflector_radius, 
		facet_spacing
	);

    facet_positions = hex_grid.get_grid();
}
//------------------------------------------------------------------------------
void SegmetedReflectorGenerator::init_facet_z_positions() {

	for(uint i=0; i<facet_positions.size(); i++) {

		facet_positions.at(i).set(
			facet_positions.at(i).x(),
			facet_positions.at(i).y(),
			get_z_pos_given_dist_to_optical_axis(facet_positions.at(i).norm())
		);
	}
}
//------------------------------------------------------------------------------
double SegmetedReflectorGenerator::get_z_pos_given_dist_to_optical_axis(
	const double dist_facet_xy_to_optical_axis
)const {

	const double z_Davies_Cotton = 
		focal_length -
		sqrt( pow(focal_length, 2.0) - pow(dist_facet_xy_to_optical_axis, 2.0));

	const double z_parabolic = 
		1.0/(4.0*focal_length)*pow(dist_facet_xy_to_optical_axis, 2.0);

	return DaviesCotton_to_parabolic_mixing_factor*z_Davies_Cotton + 
		(1.0 - DaviesCotton_to_parabolic_mixing_factor)*z_parabolic;
}
//------------------------------------------------------------------------------
void SegmetedReflectorGenerator::init_facet_orientations() {

	for(uint i=0; i<facet_positions.size(); i++)
		if(is_center_position(facet_positions.at(i)))
			facet_orientations.push_back(Rotation3D::null);
		else
			facet_orientations.push_back(get_rotation_of_facet(i));
}
//------------------------------------------------------------------------------
Rotation3D SegmetedReflectorGenerator::get_rotation_of_facet(const uint i)const {
	
	Vector3D focal_point_to_facet_pos = 
		focal_point - facet_positions.at(i);
		
	Vector3D rotation_axis = 
		focal_point_to_facet_pos.cross(Vector3D::unit_z);

	double angle_between_EZ_and_focal_point_to_mirror_pos =
		Vector3D::unit_z.get_angle_in_between_in_rad(focal_point_to_facet_pos);

	double rot_angle = 0.5*angle_between_EZ_and_focal_point_to_mirror_pos;

	return Rotation3D(rotation_axis, -rot_angle);	
}
//------------------------------------------------------------------------------
bool SegmetedReflectorGenerator::is_center_position(const Vector3D pos) {	
	return pos.x() == 0.0 && pos.y() == 0.0;
}
//------------------------------------------------------------------------------
void SegmetedReflectorGenerator::init_focal_point() {
	focal_point = optical_axis*focal_length;
}
//------------------------------------------------------------------------------
void SegmetedReflectorGenerator::optimize_reflector_z_pos() {

	uint iteration_conter = 0;
	double deviation = 0.0;

	do{
		abort_if_too_many_iterations(++iteration_conter);

		deviation = focal_length - get_average_image_distances_of_facets();

		move_all_facets_in_z(-deviation*0.5);
	}while( fabs(deviation) > focal_length*1e-6);

}
//------------------------------------------------------------------------------
void SegmetedReflectorGenerator::move_all_facets_in_z(const double movement) {

	for(uint i=0; i<facet_positions.size(); i++) {

		facet_positions.at(i).set(
			facet_positions.at(i).x(),
			facet_positions.at(i).y(),
			facet_positions.at(i).z() + movement
		);
	}	
}
//------------------------------------------------------------------------------
void SegmetedReflectorGenerator::abort_if_too_many_iterations(
	const uint iteration_conter
) {
	if(iteration_conter > 100) {
		std::stringstream info;
		info << "SegmetedReflectorGenerator::" << __func__ << "()\n";
		info << "Exceeded max number of 100 iterations. ";
		info << "Can not reach best reflector z position. ";
		throw TracerException(info.str());
	}
}
//------------------------------------------------------------------------------
double SegmetedReflectorGenerator::get_average_image_distances_of_facets() {
	uint number_of_facets = 0;
	double sum_of_image_distances = 0.0;

	for(Vector3D facet_pos: facet_positions) {
		number_of_facets++;
		sum_of_image_distances = sum_of_image_distances + 
			(facet_pos - focal_point).norm();
	}

	return sum_of_image_distances/double(number_of_facets);
}
//------------------------------------------------------------------------------
void SegmetedReflectorGenerator::init_facets() {

	for(uint i=0; i<facet_positions.size(); i++) {

		SphereCapWithHexagonalBound* facet = new SphereCapWithHexagonalBound;
		facets.push_back(facet);
		
		facet->set_name_pos_rot(
			get_name_of_facet(i),
			facet_positions.at(i),
			facet_orientations.at(i)
		);
		
		facet->set_outer_color(mirror_colour);
		facet->set_inner_color(inner_mirror_colour);
		facet->set_outer_reflection(outer_mirror_reflection);

		facet->set_curvature_radius_and_outer_hex_radius(
			focal_length*2.0,
			facet_radius
		);		
	}	
}
//------------------------------------------------------------------------------
std::string SegmetedReflectorGenerator::get_name_of_facet(const uint i)const {
	std::stringstream facet_name;
	facet_name << "mirrro_ID_" << i;
	return facet_name.str();
}
//------------------------------------------------------------------------------
void SegmetedReflectorGenerator::init_facet_radius() {
	double facet_diameter_peak2peak = 2.0/sqrt(3.0)*facet_spacing;
	facet_radius = 0.5*facet_diameter_peak2peak*facet_fill_factor;
}
//------------------------------------------------------------------------------
void SegmetedReflectorGenerator::init_reflector() {

	reflector = new Frame("Reflector", Vector3D::null, Rotation3D::null);

	for(Frame* facet : facets)
		reflector->set_mother_and_child(facet);

	reflector->cluster_using_helper_frames();
}
//------------------------------------------------------------------------------