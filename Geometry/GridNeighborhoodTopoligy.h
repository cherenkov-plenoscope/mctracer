//=================================
// include guard
#ifndef __GridNeighborhoodTopoligy_H_INCLUDED__
#define __GridNeighborhoodTopoligy_H_INCLUDED__

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
class GridNeighborhoodTopoligy{

	const std::vector<Vector3D> *grid;
	double neighborhood_radius;

	std::vector<uint> idx_x;

	std::vector<std::vector<uint>> neigbors;
public:

	GridNeighborhoodTopoligy(
		const std::vector<Vector3D> *grid,
		const double neighborhood_radius
	);

	std::vector<std::vector<uint>> get_neighbor_relations()const;
private:

	void init_idx();
	void sort_idx();
	uint get_candidate_low_x(const double x);	
	uint get_candidate_hig_x(const double x);	
};
#endif // __GridNeighborhoodTopoligy_H_INCLUDED__ 
