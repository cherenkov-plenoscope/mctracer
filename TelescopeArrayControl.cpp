#include "TelescopeArrayControl.h"
//------------------------------------------------------------------------------
void TelescopeArrayControl::move_all_to_Az_Zd(const double Az_Rad, const double Zd_Rad) {

	for(Frame* telescope : telescopes_in_world)
		telescope->move_to_Az_Zd_relative_to_mother(Az_Rad, Zd_Rad);
}
//------------------------------------------------------------------------------
std::string TelescopeArrayControl::get_print()const {

	std::stringstream out;

	out << "___Telescope_Array_Control___\n";

	uint i=0;
	for(Frame* telescope : telescopes_in_world) {
		out << i << " " << telescope->get_name_of_frame() << "\t";
		out << "Az: " << Rad2Deg(telescope->get_Az_relative_to_mother()) << ",\t";
		out << "Zd: " << Rad2Deg(telescope->get_Zd_relative_to_mother()) << " deg";
		out << "\n";
	}

	return out.str();
}
//------------------------------------------------------------------------------
void TelescopeArrayControl::add_to_telescope_array(Frame *telescope) {
	telescopes_in_world.push_back(telescope);
}
//------------------------------------------------------------------------------