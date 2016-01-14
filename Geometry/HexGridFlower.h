//=================================
// include guard
#ifndef __HexGridFlower_H_INCLUDED__
#define __HexGridFlower_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/Vector3D.h"
#include "HexagonalPrismZ.h"
#include <iostream> 
#include <string>
#include <sstream>
#include <vector>

//=================================
class HexGridFlower{

	Vector3D unit_hex_a;
	Vector3D unit_hex_b;
	std::vector<Vector3D> grid;
	HexagonalPrismZ hex_prism;
	double facet_spacing;
public:

	HexGridFlower(
		const double outer_hex_grid_radius,
		uint facet_count_on_outer_diameter
	);
	double get_facet_spacing()const;
	std::vector<Vector3D> get_grid()const; 
private:

	void init_unit_vectors_hex_grid_with_length();
	void init_grid();
};
#endif // __HexGridFlower_H_INCLUDED__ 
