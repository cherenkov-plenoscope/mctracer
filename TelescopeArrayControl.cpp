#include "TelescopeArrayControl.h"
//------------------------------------------------------------------------------
void TelescopeArrayControl::move_all_to_Az_Zd(const double Az_Rad, const double Zd_Rad) {

	for(Frame* telescope : telescopes_in_world) {

		telescope->rot_in_mother.set(0.0, Zd_Rad, Deg2Rad(180.0) - Az_Rad);

		telescope->T_frame2mother.set_transformation(
			telescope->rot_in_mother, 
			telescope->pos_in_mother
		);

		telescope->post_init_me_and_all_my_children();
	}
}
//------------------------------------------------------------------------------
std::string TelescopeArrayControl::get_print()const {

	std::stringstream out;

	out << "___Telescope_Array_Control___\n";

	uint i=0;
	for(Frame* telescope : telescopes_in_world) {
		out << i << " " << telescope->get_name_of_frame() << "\t";
		out << "Az: " << Rad2Deg(Az_of_frame(telescope)) << ",\t";
		out << "Zd: " << Rad2Deg(Zd_of_frame(telescope)) << " deg";
		out << "\n";
	}

	return out.str();
}
//------------------------------------------------------------------------------
double TelescopeArrayControl::Az_of_frame(Frame* telescope)const {
	return Deg2Rad(180.0) - telescope->get_rotation_in_mother()->get_rot_z();
}
//------------------------------------------------------------------------------
double TelescopeArrayControl::Zd_of_frame(Frame* telescope)const {

	Vector3D optical_axis_in_telescope_frame = Vector3D::unit_z;

	Vector3D optical_axis_in_world = telescope->T_frame2world.
		get_transformed_orientation(optical_axis_in_telescope_frame);

	Vector3D Zaxis_in_world_frame = Vector3D::unit_z;

	return Zaxis_in_world_frame.get_angle_in_between_in_rad(
		optical_axis_in_world
	);
}
//------------------------------------------------------------------------------
void TelescopeArrayControl::add_to_telescope_array(Frame *telescope) {
	telescopes_in_world.push_back(telescope);
}
//------------------------------------------------------------------------------