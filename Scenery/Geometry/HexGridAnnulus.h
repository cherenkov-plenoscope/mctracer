//=================================
// include guard
#ifndef __HexGridAnnulus_H_INCLUDED__
#define __HexGridAnnulus_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/Vec3.h"
#include <iostream> 
#include <string>
#include <sstream>
#include <vector>

//=================================
class HexGridAnnulus{

	Vec3 unit_hex_a;
	Vec3 unit_hex_b;

	std::vector<Vec3> grid;
public:
	HexGridAnnulus(const double outer_radius, const double spacing);
	HexGridAnnulus(const double outer_radius, const double inner_radius ,const double spacing);
	std::vector<Vec3> get_grid()const; 
	std::string str()const;
private:
	void init_unit_vectors_hex_grid_with_length(const double spacing);
	void init_grid(
		const double outer_radius, 
		const double inner_radius,
		const double spacing
	);
};
#endif // __HexGridAnnulus_H_INCLUDED__ 
