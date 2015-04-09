#include "HexGridXy.h"
//------------------------------------------------------------------------------
HexGridXy::HexGridXy(const double outer_radius, const double spacing) {
	init_grid(outer_radius, 0.0, spacing);
}
//------------------------------------------------------------------------------
HexGridXy::HexGridXy(const double outer_radius, const double inner_radius ,const double spacing) {
	init_grid(outer_radius, inner_radius, spacing);
}
//------------------------------------------------------------------------------
void HexGridXy::init_grid(
	const double outer_radius, 
	const double inner_radius,
	const double spacing
) {
	init_unit_vectors_hex_grid_with_length(spacing);

	int sample_radius = 2.0*floor(outer_radius/spacing);

	for(int a=-sample_radius; a<=sample_radius; a++) {
		for(int b=-sample_radius; b<=sample_radius; b++) {

			Vector3D cell_ab = unit_hex_a*a + unit_hex_b*b;

			const double cell_norm = cell_ab.norm();

			if(cell_norm <= outer_radius && cell_norm >= inner_radius)
				grid.push_back(cell_ab);
		}
	}
}
//------------------------------------------------------------------------------
std::vector<Vector3D> HexGridXy::get_grid()const {
	return grid;
}
//------------------------------------------------------------------------------
void HexGridXy::init_unit_vectors_hex_grid_with_length(const double spacing) { 
	unit_hex_b = Vector3D::unit_y*spacing;
	unit_hex_a = (Vector3D::unit_y*0.5+Vector3D::unit_x*sqrt(3.0)/2.0)*spacing;
}
//------------------------------------------------------------------------------
std::string HexGridXy::get_print()const {
	std::stringstream out;

	uint number = 1;
	for(Vector3D vec : grid)
		out << number++ << " " << vec << "\n";

	return out.str();
}
