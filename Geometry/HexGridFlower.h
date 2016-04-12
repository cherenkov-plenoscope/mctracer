//=================================
// include guard
#ifndef __HexGridFlower_H_INCLUDED__
#define __HexGridFlower_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/Vec3.h"
#include "HexagonalPrismZ.h"
#include <iostream> 
#include <string>
#include <sstream>
#include <vector>

//=================================
class HexGridFlower{

	Vec3 unit_hex_a;
	Vec3 unit_hex_b;
	std::vector<Vec3> grid;
	HexagonalPrismZ hex_prism;
	double facet_spacing;
public:

	HexGridFlower(
		const double outer_hex_grid_radius,
		uint facet_count_on_outer_diameter
	);
	double get_facet_spacing()const;
	std::vector<Vec3> get_grid()const; 
private:

	void init_unit_vectors_hex_grid_with_length();
	void init_grid();
};
#endif // __HexGridFlower_H_INCLUDED__ 
