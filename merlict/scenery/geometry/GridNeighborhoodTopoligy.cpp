// Copyright 2014 Sebastian A. Mueller
#include "merlict/scenery/geometry/GridNeighborhoodTopoligy.h"
#include <algorithm>

namespace merlict {

GridNeighborhoodTopoligy::GridNeighborhoodTopoligy(
    const std::vector<Vec3> *grid,
    const double neighborhood_radius
) {
    this->grid = grid;
    this->neighborhood_radius = neighborhood_radius;
    init_idx();
    sort_idx();
    neigbors.resize(grid->size());
    for (unsigned int a = 0; a < grid->size(); a++) {
        std::vector<unsigned int> a_neighbors;
        unsigned int lox = get_candidate_low_x(grid->at(a).x);
        unsigned int hix = get_candidate_hig_x(grid->at(a).x);
        for (unsigned int b = lox; b < hix; b++) {
        // for(unsigned int b=0; b<grid->size(); b++) {
            if (a != b) {
                if (grid->at(a).distance_to(grid->at(b)) <=
                    neighborhood_radius
                ) {
                    a_neighbors.push_back(b);
                }
            }
        }
        neigbors.at(a) = a_neighbors;
    }
}

std::vector<std::vector<unsigned int>> GridNeighborhoodTopoligy::
    get_neighbor_relations()const {
    return neigbors;
}

void GridNeighborhoodTopoligy::init_idx() {
    idx_x.resize(grid->size());
    for (unsigned int i = 0; i < grid->size(); i++) {
        idx_x[i] = i;
    }
}

void GridNeighborhoodTopoligy::sort_idx() {
    std::sort(idx_x.begin(), idx_x.end(),
        [&](const unsigned int a, const unsigned int b) {
            return(grid->at(a).x < grid->at(b).x);
        });
}

unsigned int GridNeighborhoodTopoligy::get_candidate_low_x(const double x) {
    std::vector<unsigned int>::const_iterator it;
    it = std::lower_bound(
        idx_x.begin(), idx_x.end(),
        (x - 1.1*neighborhood_radius),
        [&](const unsigned int a, const double X) {return(grid->at(a).x < X);});
    return (it - idx_x.begin());
}

unsigned int GridNeighborhoodTopoligy::get_candidate_hig_x(const double x) {
    std::vector<unsigned int>::const_iterator it;
    it = std::upper_bound(
        idx_x.begin(), idx_x.end(),
        (x + 1.1*neighborhood_radius),
        [&](const double X, const unsigned int a) {return(grid->at(a).x > X);});
    return (it - idx_x.begin());
}

}  // namespace merlict
