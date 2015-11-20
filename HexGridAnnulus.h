//=================================
// include guard
#ifndef __HexGridAnnulus_H_INCLUDED__
#define __HexGridAnnulus_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/Vector3D.h"
#include <iostream> 
#include <string>
#include <sstream>
#include <vector>

//=================================
class HexGridAnnulus{

	Vector3D unit_hex_a;
	Vector3D unit_hex_b;

	std::vector<Vector3D> grid;
public:
	HexGridAnnulus(const double outer_radius, const double spacing);
	HexGridAnnulus(const double outer_radius, const double inner_radius ,const double spacing);
	std::vector<Vector3D> get_grid()const; 
	std::string get_print()const;
private:
	void init_unit_vectors_hex_grid_with_length(const double spacing);
	void init_grid(
		const double outer_radius, 
		const double inner_radius,
		const double spacing
	);
};
#endif // __HexGridAnnulus_H_INCLUDED__ 
