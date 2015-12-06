#include "HexGridFlower.h"
#include "Tools/Tools.h"
//------------------------------------------------------------------------------
HexGridFlower::HexGridFlower(
	const double outer_hex_grid_radius,
	uint facet_count_on_outer_diameter
) {
	// ensure diagonal count is odd
	if(is_even(facet_count_on_outer_diameter))
		facet_count_on_outer_diameter++;

	const uint facet_row_count_on_flat2flat_diameter = 
		facet_count_on_outer_diameter;

	const double inner_hex_grid_radius = outer_hex_grid_radius*(sqrt(3.0)/2.0);

	const double inner_hex_grid_diameter = 2.0*inner_hex_grid_radius;

	const double outer_hex_facet_radius = inner_hex_grid_diameter/(
			0.5*(3.0*double(facet_row_count_on_flat2flat_diameter) + 1.0)
		);

	const double inner_hex_facet_radius = outer_hex_facet_radius*(sqrt(3.0)/2.0);

	facet_spacing = 2.0*inner_hex_facet_radius;

	hex_prism.set_outer_radius(outer_hex_grid_radius);
	init_grid();
}
//------------------------------------------------------------------------------
void HexGridFlower::init_grid() {
	init_unit_vectors_hex_grid_with_length();

	int sample_radius = 2.0*floor(hex_prism.get_outer_radius()/facet_spacing);

	for(int a=-sample_radius; a<=sample_radius; a++) {
		for(int b=-sample_radius; b<=sample_radius; b++) {

			Vector3D cell_ab = unit_hex_a*a + unit_hex_b*b;

			if(hex_prism.is_inside(&cell_ab))
				grid.push_back(cell_ab);
		}
	}
}
//------------------------------------------------------------------------------
std::vector<Vector3D> HexGridFlower::get_grid()const {
	return grid;
}
//------------------------------------------------------------------------------
void HexGridFlower::init_unit_vectors_hex_grid_with_length() {

	unit_hex_b = Vector3D::unit_x*facet_spacing;
	unit_hex_a = (
		Vector3D::unit_x*0.5+Vector3D::unit_y*sqrt(3.0)/2.0
	)*facet_spacing;
}
//------------------------------------------------------------------------------
double HexGridFlower::get_facet_spacing()const {
	return facet_spacing;
}