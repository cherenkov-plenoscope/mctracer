#include "GridNeighborhoodTopoligy.h"
#include <algorithm>
//------------------------------------------------------------------------------
GridNeighborhoodTopoligy::GridNeighborhoodTopoligy(
	const std::vector<Vec3> *grid,
	const double neighborhood_radius
) {
	this->grid = grid;
	this->neighborhood_radius = neighborhood_radius;

	init_idx();
	sort_idx();

	neigbors.resize(grid->size());

	for(uint a=0; a<grid->size(); a++) {

		std::vector<uint> a_neighbors;

		uint lox = get_candidate_low_x(grid->at(a).x());
		uint hix = get_candidate_hig_x(grid->at(a).x());

		for(uint b=lox; b<hix; b++) {
		//for(uint b=0; b<grid->size(); b++) {
			if(a != b) {
				if(grid->at(a).distance_to(grid->at(b)) <= neighborhood_radius) {
					a_neighbors.push_back(b);
				}
			}
		}

		neigbors.at(a) = a_neighbors;
	}
}
//------------------------------------------------------------------------------
std::vector<std::vector<uint>> GridNeighborhoodTopoligy::get_neighbor_relations()const {
	return neigbors;
}
//------------------------------------------------------------------------------
void GridNeighborhoodTopoligy::init_idx() {

	idx_x.resize(grid->size());

	for(uint i=0; i<grid->size(); i++) {
		idx_x[i] = i;
	}
}
//------------------------------------------------------------------------------
void GridNeighborhoodTopoligy::sort_idx() {		

	std::sort(idx_x.begin(), idx_x.end(),
        [&](const uint a, const uint b) {
            return(grid->at(a).x() < grid->at(b).x());
        }
    );
}
//------------------------------------------------------------------------------
uint GridNeighborhoodTopoligy::get_candidate_low_x(const double x) {	

	std::vector<uint>::const_iterator it;

	it = std::lower_bound(
		idx_x.begin(), idx_x.end(), 
		(x - 1.1*neighborhood_radius), 
		[&](const uint a, const double X) {return(grid->at(a).x() < X);}
	);

	return (it - idx_x.begin());
}
//------------------------------------------------------------------------------
uint GridNeighborhoodTopoligy::get_candidate_hig_x(const double x) {	

	std::vector<uint>::const_iterator it;

	it = std::upper_bound(
		idx_x.begin(), idx_x.end(), 
		(x + 1.1*neighborhood_radius), 
		[&](const double X, const uint a) {return(grid->at(a).x() > X);}
	);

	return (it - idx_x.begin());
}
//std::vector<uint> get_candidates_x()