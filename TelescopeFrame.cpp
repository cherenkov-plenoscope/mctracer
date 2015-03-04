#include "TelescopeFrame.h"
//------------------------------------------------------------------------------
void TelescopeFrame::move_to_Az_Zd_in_Rad(const double _Az_Rad, const double _Zd_Rad) {

	Az_Rad = _Az_Rad;
	Zd_Rad = _Zd_Rad;

	rot_in_mother.set(0.0, Zd_Rad, Deg2Rad(180.0) - Az_Rad);

	T_frame2mother.set_transformation(rot_in_mother, pos_in_mother);	
	post_initialize_me_and_all_my_children();
}
//------------------------------------------------------------------------------
void TelescopeFrame::move_to_Az_Zd_in_Deg(const double Az_Deg, const double Zd_Deg) {
	move_to_Az_Zd_in_Rad(Deg2Rad(Az_Deg), Deg2Rad(Zd_Deg));
}
//------------------------------------------------------------------------------
std::string TelescopeFrame::get_pointing_print()const {
	Vector3D optical_axis_in_telescope_frame(0.0,0.0,1.0);
	Vector3D optical_axis_in_world = 
		T_frame2world.get_transformed_orientation(optical_axis_in_telescope_frame);

	Vector3D Zaxis_in_world_frame(0.0,0.0,1.0);

	double zenith_distance = Zaxis_in_world_frame.get_angle_in_between_in_rad(
		optical_axis_in_world
	);

	Vector3D Xaxis_in_world_frame(1.0,0.0,0.0);

	std::stringstream out;
	out << "Az: " << Rad2Deg(Az_Rad) << ", Zd: " << Rad2Deg(zenith_distance) << " [Deg]";
	return out.str();
}
//------------------------------------------------------------------------------